#include "MyGIFViewer.h"

#include <filesystem>
#include <fstream>
#include <iostream>

constexpr bool SINGLE_STEP_MODE = false;

bool MyGIFViewer::create() {
  // init SDL
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0) {
    std::cerr << "SDL_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }

  bool created = _createWindow(640, 400);
  if (!created) {
    std::cerr << "SDL Window create failed." << std::endl;
    return false;
  }

  _drawBackground(true);
  return true;
}

void MyGIFViewer::mainLoop() {
  SDL_Event evt = {};
  while (evt.type != SDL_QUIT) {
    if (SDL_PollEvent(&evt)) {
      _onSDLEvent(evt);
    } else {
      _tick();
    }
  }  // end of while
}

void MyGIFViewer::shutdown() {
  mGIF.reset();

  if (mMainWindow) {
    SDL_DestroyWindow(mMainWindow);
    mMainWindow = nullptr;
  }
  SDL_Quit();
}

bool MyGIFViewer::_openGIF(const std::string& szFileName) {
  // load file to memory blob
  std::filesystem::path filePath(szFileName);
  if (!std::filesystem::exists(filePath)) {
    std::cerr << szFileName << " does NOT exist...." << std::endl;
    return false;
  }

  std::ifstream fs(szFileName, std::ios::binary);
  if (!fs.is_open()) {
    std::cerr << szFileName << " file open FAILED." << std::endl;
    return false;
  }

  std::streampos begin = fs.tellg();
  fs.seekg(0, std::ios::end);
  std::streampos end = fs.tellg();
  fs.seekg(0, std::ios::beg);

  size_t fsize = end - begin;

  std::vector<uint8_t> fileData(fsize);
  fs.read((char*)fileData.data(), fsize);

  // create gif object
  mGIF = std::make_unique<FAnimatedGIF>();
  if (mGIF->loadFromMemory(fileData.data(), fsize)) {
    SDL_SetWindowSize(mMainWindow, mGIF->getWidth(), mGIF->getHeight());
    _drawBackground(true);

    std::cout << szFileName << "\n\t width = " << mGIF->getWidth() << std::endl
              << "\t height = " << mGIF->getHeight() << std::endl;

    return true;
  }

  mGIF.reset();
  std::cerr << szFileName << " open failed..." << std::endl;
  return false;
}

bool MyGIFViewer::_createWindow(int width, int height) {
  mMainWindow = SDL_CreateWindow("My GIF Viewer", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, width, height,
                                 SDL_WINDOW_UTILITY);

  return mMainWindow != nullptr;
}

void MyGIFViewer::_onSDLEvent(const SDL_Event& evt) {
  if (evt.type == SDL_DROPFILE) {
    _openGIF(evt.drop.file);
  }

  if (evt.type == SDL_KEYDOWN) {
    bNextStep = true;
  }
}

void MyGIFViewer::_tick() {
  if (!mGIF) return;

  constexpr int DEFAULT_FRAME_DELAY = 1000 / 24;
  if (SINGLE_STEP_MODE) {
    if (bNextStep) {
      mGIF->playFrame(DEFAULT_FRAME_DELAY);
      _drawFrame();
      bNextStep = false;
    }
  } else {
    Uint32 now = SDL_GetTicks();
    Uint32 deltaTime = now - mLastFrameTime;

    if (deltaTime >= mFrameDelay) {
      mFrameDelay = mGIF->playFrame(DEFAULT_FRAME_DELAY);
      _drawFrame();
      mLastFrameTime = now;
    }
  }
}

void MyGIFViewer::_drawFrame() {
  auto width = mGIF->getWidth();
  auto height = mGIF->getHeight();
  auto gifFrame = mGIF->getFrameBuffer();

  SDL_Surface* surface = SDL_GetWindowSurface(mMainWindow);
  uint8_t* surfaceBuffer = (uint8_t*)(surface->pixels);

  int ret = SDL_LockSurface(surface);
  if (ret >= 0) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int i = y * width + x;
        const auto& gif = gifFrame[i];
        uint8_t* pixel = surfaceBuffer + i * surface->format->BytesPerPixel;

        uint32_t& t = *(uint32_t*)pixel;
        if (gif.A > 0)
          t = SDL_MapRGBA(surface->format, gif.R, gif.G, gif.B, gif.A);
        else
          t = _getCheckerColor(x, y);
      }  // end of for(x)
    }    // end of for(y)

    SDL_UnlockSurface(surface);
  }

  SDL_UpdateWindowSurface(mMainWindow);
}

void MyGIFViewer::_drawBackground(bool present) {
  SDL_Surface* surface = SDL_GetWindowSurface(mMainWindow);
  uint8_t* surfaceBuffer = (uint8_t*)(surface->pixels);

  int ret = SDL_LockSurface(surface);
  if (ret >= 0) {
    for (int y = 0; y < surface->h; y++) {
      for (int x = 0; x < surface->w; x++) {
        int i = y * surface->w + x;
        uint8_t* pixel = surfaceBuffer + i * surface->format->BytesPerPixel;
        uint32_t& t = *(uint32_t*)pixel;
        t = _getCheckerColor(x, y);
      }  // end of for(x)
    }    // end of for(y)

    SDL_UnlockSurface(surface);
  }

  if (present) SDL_UpdateWindowSurface(mMainWindow);
}

uint32_t MyGIFViewer::_getCheckerColor(int x, int y) {
  int width = 1;
  int height = 1;
  SDL_GetWindowSize(mMainWindow, &width, &height);

  const float size = (float)std::min(width, height);
  const float P = size / 15;
  constexpr uint32_t oddColor = 0xFF202020;
  constexpr uint32_t evenColor = 0xFF404040;
  float c = sinf(P * x / size) * cosf(P * y / size);
  return c < 0 ? oddColor : evenColor;
}
