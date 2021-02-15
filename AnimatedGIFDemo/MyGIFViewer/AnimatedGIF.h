#pragma once
#include <string>
#include <vector>

#include "giflib/gif_lib.h"

/**
 * GIFLIB decoder wrapper class
 *
 */
class FAnimatedGIF {
 public:
  struct Pixel {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
  };
  FAnimatedGIF() = default;
  ~FAnimatedGIF();

  bool loadFromMemory(uint8_t* buffer, size_t size);
  void close();

  /**
   * @return frame delay in milliseconds
   */
  int playFrame(int defaultFrameDelay);

  int getWidth() const;
  int getHeight() const;
  const Pixel* getFrameBuffer() const;

 private:
  void _clearFrameBuffer(ColorMapObject* colorMap, bool bTransparent);
  void _GCB_Background(int left, int top, int width, int height,
                       ColorMapObject* colorMap, bool bTransparent);

 private:
  GifFileType* mGIF = nullptr;
  int mCurrentFrame = 0;
  int mLoopCount = 0;
  std::vector<Pixel> mFrameBuffer;
  bool mDoNotDispose = false;

 public:
  FAnimatedGIF(const FAnimatedGIF&) = delete;
  FAnimatedGIF& operator=(const FAnimatedGIF&) = delete;
};