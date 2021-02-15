#include "AnimatedGIF.h"

#include <iostream>

FAnimatedGIF::~FAnimatedGIF() { close(); }

static int _GIF_InputFunc(GifFileType* gifFile, GifByteType* buffer,
                          int length) {
  uint8_t* fileData = (uint8_t*)(gifFile->UserData);
  memcpy(buffer, fileData, length);
  gifFile->UserData = fileData + length;
  return length;
}

bool FAnimatedGIF::loadFromMemory(uint8_t* buffer, size_t size) {
  int gifError = 0;
  mGIF = DGifOpen(buffer, _GIF_InputFunc, &gifError);
  if (mGIF == nullptr) {
    std::cerr << "GIF file open failed, error code: "
              << GifErrorString(gifError) << std::endl;
    return false;
  }

  gifError = DGifSlurp(mGIF);
  if (gifError != GIF_OK) {
    std::cerr << "GIF file load failed, error code: "
              << GifErrorString(gifError) << std::endl;
    return false;
  }

  mFrameBuffer.resize(mGIF->SWidth * mGIF->SHeight);
  _clearFrameBuffer(mGIF->SColorMap, true);
  return true;
}

void FAnimatedGIF::close() {
  if (mGIF) {
    int error = 0;
    DGifCloseFile(mGIF, &error);
    mGIF = nullptr;
  }
  mFrameBuffer.clear();
}

int FAnimatedGIF::playFrame(int defaultFrameDelay) {
  if (!mGIF) return defaultFrameDelay;

  const SavedImage& image = mGIF->SavedImages[mCurrentFrame];
  const auto& id = image.ImageDesc;
  int frameWidth = getWidth();
  ColorMapObject* colorMap =
      image.ImageDesc.ColorMap ? image.ImageDesc.ColorMap : mGIF->SColorMap;

  // handle GCB
  int delayTime = 0;
  int transparentColor = -1;

  for (int i = 0; i < image.ExtensionBlockCount; i++) {
    const ExtensionBlock& eb = image.ExtensionBlocks[i];
    if (eb.Function == GRAPHICS_EXT_FUNC_CODE) {
      GraphicsControlBlock gcb;
      if (DGifExtensionToGCB(eb.ByteCount, eb.Bytes, &gcb) != GIF_ERROR) {
        delayTime = gcb.DelayTime * 10;  // 1/100 second
        if (gcb.TransparentColor != NO_TRANSPARENT_COLOR)
          transparentColor = gcb.TransparentColor;

        switch (gcb.DisposalMode) {
          case DISPOSAL_UNSPECIFIED:
            // No disposal specified. The decoder is not required to take any
            // action.
            break;
          case DISPOSE_DO_NOT:
            // Do not dispose. The graphic is to be left in place.
            mDoNotDispose = true;
            break;
          case DISPOSE_BACKGROUND:
            //  Restore to background color. The area used by the graphic must
            //  be restored to the background color.
            if (!mDoNotDispose)  // MY HACK!!!
              _GCB_Background(id.Left, id.Top, id.Width, id.Height, colorMap,
                              transparentColor != NO_TRANSPARENT_COLOR);
            break;
          case DISPOSE_PREVIOUS:
            // Restore to previous. The decoder is required to restore the area
            // overwritten by the graphic with what was there prior to rendering
            // the graphic.
            std::cout << "DISPOSE_PREVIOUS" << std::endl;
            break;
        }
      }
    }  // end of if
  }    // end of for

  // first frame -- draw the background
  if (mCurrentFrame == 0) {
    _clearFrameBuffer(mGIF->SColorMap,
                      transparentColor != NO_TRANSPARENT_COLOR);
  }

  // decode current image to frame buffer
  for (int y = id.Top; y < id.Top + id.Height; y++) {
    for (int x = id.Left; x < id.Left + id.Width; x++) {
      int p = y * frameWidth + x;
      int i = (y - id.Top) * id.Width + x - id.Left;
      int c = image.RasterBits[i];
      Pixel& out = mFrameBuffer[p];

      const GifColorType& colorEntry = colorMap->Colors[c];
      if (mDoNotDispose) {
        if (c != transparentColor) {
          out.R = colorEntry.Red;
          out.G = colorEntry.Green;
          out.B = colorEntry.Blue;
          out.A = 255;
        }
      } else {
        out.R = colorEntry.Red;
        out.G = colorEntry.Green;
        out.B = colorEntry.Blue;
        out.A = (c == transparentColor ? 0 : 255);
      }
    }
  }  // end of y

  // next frame
  mCurrentFrame++;
  if (mCurrentFrame >= mGIF->ImageCount) {
    mDoNotDispose = false;
    mCurrentFrame = 0;
    mLoopCount++;
    std::cout << "loop count = " << mLoopCount << std::endl;
  }

  return delayTime == 0 ? defaultFrameDelay : delayTime;
}

int FAnimatedGIF::getWidth() const {
  if (mGIF)
    return mGIF->SWidth;
  else
    return 1;
}

int FAnimatedGIF::getHeight() const {
  if (mGIF)
    return mGIF->SHeight;
  else
    return 0;
}

const FAnimatedGIF::Pixel* FAnimatedGIF::getFrameBuffer() const {
  return mFrameBuffer.data();
}

void FAnimatedGIF::_clearFrameBuffer(ColorMapObject* colorMap,
                                     bool bTransparent) {
  Pixel bg = {0, 0, 0, 255};

  if (colorMap && mGIF->SBackGroundColor >= 0 &&
      mGIF->SBackGroundColor < colorMap->ColorCount) {
    const GifColorType& colorEntry =
        mGIF->SColorMap->Colors[mGIF->SBackGroundColor];
    uint8_t alpha = bTransparent ? 0 : 255;
    bg = {colorEntry.Red, colorEntry.Green, colorEntry.Blue, alpha};
  }

  for (auto& pixel : mFrameBuffer) pixel = bg;
}

void FAnimatedGIF::_GCB_Background(int left, int top, int width, int height,
                                   ColorMapObject* colorMap,
                                   bool bTransparent) {
  const GifColorType& colorEntry = colorMap->Colors[mGIF->SBackGroundColor];
  Pixel bg = {colorEntry.Red, colorEntry.Green, colorEntry.Blue,
              bTransparent ? 0 : 255};

  int frameWidth = getWidth();
  for (int y = top; y < top + height; y++) {
    for (int x = left; x < left + width; x++) {
      int p = y * frameWidth + x;
      mFrameBuffer[p] = bg;
    }
  }  // end of y
}
