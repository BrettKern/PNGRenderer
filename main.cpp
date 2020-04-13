#include <iostream>
#include <fstream>
#include <array>
#include <SDL2/SDL.h>

using namespace std;

class Image {
  bool BigEndian;
  uint32_t Width = -1;
  uint32_t Height = -1;
  uint8_t BitDepth = -1;
  uint8_t ColorType = -1;
  uint8_t CompressionMethod = -1;
  uint8_t FilterMethod = -1;
  uint8_t InterlaceMethod = -1;
public:
  void CheckByteOrder();
  void readPNGSignature(ifstream& file);
  void readIHDRChunk(ifstream& file);
  void printImageStats();
  bool nextChunk(ifstream& file);//returns true on IEND
  uint32_t getWidth();
  uint32_t getHeight();
};

uint32_t swapEndian(uint32_t word){
  uint32_t b0, b1, b2, b3;

  b0 = (word & 0x000000ff) << 24u;
  b1 = (word & 0x0000ff00) << 8u;
  b2 = (word & 0x00ff0000) >> 8u;
  b3 = (word & 0xff000000) >> 24u;

  return b0 | b1 | b2 | b3;
}

void Image::readPNGSignature(ifstream& file) {
  //first eight bytes of a PNG file should always contain 137 80 78 71 13 10 26 10
  uint8_t data[8];
  int headerarray[] = {137, 80, 78, 71, 13, 10, 26, 10};

  for (int i = 0; i < 8; i++){
    file.read(reinterpret_cast<char*>(&data[i]), sizeof(data[0]));
    std::cout << "read in: " << (int)data[i] << endl;
    if ((int)data[i] != headerarray[i])
    {
      cout << "[System] The input file does not fit the PNG format standards" << endl;
      exit(EXIT_FAILURE);
    }
  }
  cout << "[System] file has passed PNGSignature test" << endl;
}

void Image::readIHDRChunk(ifstream& file){
  //4 byte unsigned integer giving the number of bytes int he chunk dat field
  //4 byte chunk type code
  //chunk data of length designated above
  //4 byte crc (ran over the chunk type code and data field but not the length field)
  uint32_t chunkLength;
  file.read(reinterpret_cast<char*>(&chunkLength), sizeof(chunkLength));
  chunkLength = swapEndian(chunkLength);
  cout << "[System] chunk length: " << chunkLength << endl;

  char IHDRTypeCode[] = {73, 72, 68, 82};

  uint8_t typeCode[4];
  cout << "[System] chunk code: ";
  for (int i = 0; i < 4; i++)
  {
    file.read(reinterpret_cast<char*>(&typeCode[i]), sizeof(typeCode[i]));
    cout << (char)typeCode[i];
    if (IHDRTypeCode[i] != (char)typeCode[i]) exit(EXIT_FAILURE);
  }
  cout << endl;

  //no need to iterate over 13 bytes of chunk data, IHDR always contains 13 bytes
  file.read(reinterpret_cast<char*>(&this->Width), sizeof(this->Width));
  this->Width = swapEndian(this->Width);
  file.read(reinterpret_cast<char*>(&this->Height), sizeof(this->Height));
  this->Height = swapEndian(this->Height);
  if (this->Width == 0 || this->Height == 0)
  {
    cout << "[ERROR] Width and Height must be non zero values" << endl;
    exit(EXIT_FAILURE);
  }
  file.read(reinterpret_cast<char*>(&this->BitDepth), sizeof(this->BitDepth));
  file.read(reinterpret_cast<char*>(&this->ColorType), sizeof(this->ColorType));
  file.read(reinterpret_cast<char*>(&this->CompressionMethod), sizeof(this->CompressionMethod));
  if (this->CompressionMethod != 0)
  {
    cout << "[ERROR] Compression Method must be of type 0" << endl;
    exit(EXIT_FAILURE);
  }
  file.read(reinterpret_cast<char*>(&this->FilterMethod), sizeof(this->FilterMethod));
  if (this->FilterMethod != 0)
  {
    cout << "[ERROR] Filter Method must be of type 0" << endl;
    exit(EXIT_FAILURE);
  }
  file.read(reinterpret_cast<char*>(&this->InterlaceMethod), sizeof(this->InterlaceMethod));

  uint32_t crc;
  file.read(reinterpret_cast<char*>(&crc), sizeof(crc)); //ignore crc checks until later
}

void Image::printImageStats(){
  cout << "Width: " << this->Width << endl;
  cout << "Height: " << this->Height << endl;
  cout << "BitDepth: " << (int)this->BitDepth << endl;
  cout << "ColorType: " << (int)this->ColorType << endl;
  cout << "CompressionMethod: " << (int)this->CompressionMethod << endl;
  cout << "FilterMethod: " << (int)this->FilterMethod << endl;
  cout << "InterlaceMethod: " << (int)this->InterlaceMethod << endl;
}

void Image::CheckByteOrder(){
  int num = 1;

  if (*(char *)&num == 1)
  {
      printf("Little-Endian\n");
      this->BigEndian = false;
  }
  else
  {
      printf("Big-Endian\n");
      this->BigEndian = true;
  }
}

bool Image::nextChunk(ifstream& file){
  //expressions for each possible chunk type to compare against
  std::array<uint8_t, 4> typeCodePLTE = {'P', 'L', 'T', 'E'};
  std::array<uint8_t, 4> typeCodesRGB = {'s', 'R', 'G', 'B'};
  std::array<uint8_t, 4> typeCodeIEND = {'I', 'E', 'N', 'D'};
  std::array<uint8_t, 4> typeCodeIDAT = {'I', 'D', 'A', 'T'};
  std::array<uint8_t, 4> typeCodepHYs = {'p', 'H', 'Y', 's'};
  std::array<uint8_t, 4> typeCodegAMA = {'g', 'A', 'M', 'A'};

  uint32_t chunkLength;
  file.read(reinterpret_cast<char*>(&chunkLength), sizeof(chunkLength));
  chunkLength = swapEndian(chunkLength);

  std::array<uint8_t, 4> typeCode;
  for (int i = 0; i < 4; i++)
  {
    file.read(reinterpret_cast<char*>(&typeCode[i]), sizeof(typeCode[i]));
  }

  uint8_t temp;

  //this is temporary, each individual chunk code check must handle the reading of the data and crc itself
  for (int i = 0; i < chunkLength + 4; i++)
  {
    file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
  }

  //implementations for handling each different chunk type
  if (typeCode == typeCodePLTE){
    cout << "PLTE" << endl;
    return false;
  } else if (typeCode == typeCodesRGB){
    cout << "sRGB" << endl;;
    return false;
  } else if (typeCode == typeCodeIDAT) {
    //deflate algorithm to get raw pixels then write these to a texture for sdl
    cout << "IDAT" << endl;
    return false;
  } else if (typeCode == typeCodegAMA) {
    cout << "gAMA" << endl;
    return false;
  } else if (typeCode == typeCodepHYs) {
    cout << "pHYs" << endl;
    return false;
  } else if (typeCode == typeCodeIEND) {
    cout << "IEND";
    return true;
  }



}

uint32_t Image::getWidth(){
  return this->Width;
}
uint32_t Image::getHeight(){
  return this->Height;
}

int main(int argc, char * argv[])
{
  ifstream infile;
  infile.open ( "test2.png", ios::in|ios::binary|ios::ate );

  //get file size in bytes
  infile.seekg(0, ios::end); //set pointer to the end
  int size = infile.tellg(); //get the length of the file
  cout << "[System] file size in bytes: " << size << endl;
  infile.seekg(0, ios::beg);//set pointer back to start

  //class object to handle all image operations from the file
  Image image;
  image.CheckByteOrder(); //little-endian vs big-endian
  image.readPNGSignature(infile);//8 byte signature at start of all PNGs
  image.readIHDRChunk(infile);//IHDR chunk is first chunk in all PNGs
  image.printImageStats();

  bool lastChunk = false;

  while(!lastChunk)
  {
    lastChunk = image.nextChunk(infile);
  }
  cout << "Ending of file" << endl;

  //image.nextChunk(infile);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    return EXIT_FAILURE;


  //draw a temp SDL window
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(image.getWidth(), image.getHeight(), 0, &window, &renderer) == 0) {
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
