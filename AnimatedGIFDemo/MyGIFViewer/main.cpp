#include <iostream>
#include <string>

#include "MyGIFViewer.h"

int main(int argc, char* argv[]) {
  std::cout << "Drag and drop a .gif file to the window..." << std::endl;
  MyGIFViewer viewer;
  if (viewer.create()) {
    viewer.mainLoop();
    viewer.shutdown();
    return 0;
  }

  return -1;
}
