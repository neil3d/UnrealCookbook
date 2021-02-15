
#pragma once
#include <SDL2/SDL.h>

#include <memory>

#include "AnimatedGIF.h"

class MyGIFViewer {
 public:
  MyGIFViewer() = default;
  ~MyGIFViewer() = default;

  bool create();
  void mainLoop();
  void shutdown();

 private:
  bool _openGIF(const std::string& szFileName);
  bool _createWindow(int width, int height);
  void _onSDLEvent(const SDL_Event& evt);
  void _tick();
  void _drawFrame();
  void _drawBackground(bool present);
  uint32_t _getCheckerColor(int x, int y);

 private:
  Uint32 mLastFrameTime = 0;
  Uint32 mFrameDelay = 0;
  bool bNextStep = false;

  std::unique_ptr<FAnimatedGIF> mGIF;
  SDL_Window* mMainWindow = nullptr;

 public:
  MyGIFViewer(const MyGIFViewer&) = delete;
  MyGIFViewer& operator=(const MyGIFViewer&) = delete;
};