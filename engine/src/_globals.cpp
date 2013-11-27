#include "_globals.h"

SDL_Event event;
const Uint8 *gKeyboardState = SDL_GetKeyboardState(NULL);

string generate_random_alphanumeric_string(uint n)
{
  static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

  if(n == 0) n = 8;
  string s; s.resize(n);

  for(uint i = 0; i < n; ++i)
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

  return s;
}

SDL_Surface* sdl_cargar_img(std::string s)
{
  SDL_Surface* img = NULL;
  img = IMG_Load(s.c_str());
  SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA8888, 0);

  return img;
}

bool gValidateIdentifier(string identifier)
{
  for(string::iterator it = identifier.begin(); it != identifier.end(); it++)
    if(!isalnum(*it) and (*it) != '_')
      return false;

  return true;
}
