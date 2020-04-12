#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>

using namespace std;

#define headerarray[] = {137, 80, 78, 71, 13, 10, 26, 10};

class Image {
  int width = -1;
  int height = -1;
public:
  void readIHDR(ifstream& file);
};

void Image::readIHDR(ifstream& file) {
  //first eight bytes of a PNG file should always contain 137 80 78 71 13 10 26 10
  uint8_t data = 0;
  file.read(reinterpret_cast<char*>(&data), sizeof(data));
  cout << (int)data << endl;
}


int main(int argc, char * argv[])
{

  ifstream infile;
  infile.open ( "test.png", ios::in|ios::binary|ios::ate );

  //get file size
  infile.seekg(0, ios::end); //set pointer to the end
  int size = infile.tellg(); //get the length of the file
  cout << "file size in bytes: " << size << endl;
  infile.seekg(0, ios::beg);//set pointer back to start

  Image image;
  image.readIHDR(infile);

  uint8_t data = 0;
  infile.read(reinterpret_cast<char*>(&data), sizeof(data));
  cout << (int)data << endl;

  //basic data for the SDL renderer

  int imageWidth = 640;
  int imageHeight = 480;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    return EXIT_FAILURE;


  //draw a temp SDL window
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

      if (SDL_CreateWindowAndRenderer(imageWidth, imageHeight, 0, &window, &renderer) == 0) {
          SDL_bool done = SDL_FALSE;

          while (!done) {
              SDL_Event event;

              SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderClear(renderer);

              SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
              SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
              SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
              SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
              SDL_RenderPresent(renderer);

              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                      done = SDL_TRUE;
                  }
              }
          }
      }

      if (renderer) {
          SDL_DestroyRenderer(renderer);
      }
      if (window) {
          SDL_DestroyWindow(window);
      }
  }


  SDL_Quit();
  return EXIT_SUCCESS;
}
