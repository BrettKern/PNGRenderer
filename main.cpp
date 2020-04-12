#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char * argv[])
{

  ifstream infile;
  infile.open ( "test.png", ios::in|ios::binary|ios::ate );
  char* oData = 0;
  infile.seekg(0, ios::end); //set pointer to the end
  int size = infile.tellg(); //get the length of the file
  infile.seekg(0, ios::beg);//set pointer back to start

  oData = new char;
  infile.read(oData, 1);

  int imageWidth = 640;
  int imageHeight = 480;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {    std::cout << "SDL Could not initialize!" << std::endl;    }
  else
  {    std::cout << "SUCCESS" << std::endl;    }

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

//compile without command line
/*

g++ main.cpp -IC:\Dev\i686-w64-mingw32\include -LC:\Dev\i686-w64-mingw32\lib -o main.exe -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o SDLMain

*/
//compile WITH command line
/*

g++ main2.cpp -IC:\Dev\i686-w64-mingw32\include -LC:\Dev\i686-w64-mingw32\lib -o main.exe -w -lmingw32 -lSDL2main -lSDL2

*/

//NOTE the difference between them is the -Wl,-subsystem,windows
