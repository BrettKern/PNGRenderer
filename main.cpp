#include <iostream>

#include <SDL2/SDL.h>

int main(int argc, char * argv[])
{
    std::cout << "TEST" << std::endl;
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "SDL Could not initialize!" << std::endl;
  }
  else
  {
    std::cout << "SUCCESS" << std::endl;
  }

  system( "pause");
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
