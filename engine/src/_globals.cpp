#include "_globals.h"

SDL_Event event;
//const Uint8 *gKeyboardState = SDL_GetKeyboardState(NULL);

string GO_Utils::string_generate_random_alphanumeric(uint n)
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

string GO_Utils::string_to_lower(string& str)
{
  string output = str;
  transform(str.begin(), str.end(), output.begin(), ::tolower);

  return output;
}

string GO_Utils::string_to_upper(string& str)
{
  string output = str;
  transform(str.begin(), str.end(), output.begin(), ::toupper);

  return output;
}

SDL_Surface* GO_Utils::sdl_cargar_img(std::string s)
{
  SDL_Surface* img = NULL;
  img = IMG_Load(s.c_str());
  SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA8888, 0);

  return img;
}

bool GO_Utils::validateIdentifier(string identifier)
{
  for(string::iterator it = identifier.begin(); it != identifier.end(); ++it)
    if(!isalnum(*it) and (*it) != '_')
      return false;

  return true;
}

GLfloat vector3f_t::precision = 0.01f;

/* NO FUNCIONA!
void GO_Utils::glhLookAtf2(glm::mat4& matrix, vector3f& eyePosition3D, vector3f& center3D, vector3f& upVector3D )
{
  vector3f forward, side, up;
  glm::mat4 matrix2;//, resultMatrix;

  forward[0] = center3D[0] - eyePosition3D[0];
  forward[1] = center3D[1] - eyePosition3D[1];
  forward[2] = center3D[2] - eyePosition3D[2];
  forward = forward.normalize();

  //------------------
  //Side = forward x up
  upVector3D = GO_Utils::glComputeNormalOfPlane(side, forward);
  side = side.normalize();

  //------------------
  //Recompute up as: up = side x forward
  forward = GO_Utils::glComputeNormalOfPlane(up, side);
  //------------------
  matrix2[0][0] = side[0];
  matrix2[1][0] = side[1];
  matrix2[2][0] = side[2];
  matrix2[3][0] = 0.0;
  //------------------
  matrix2[0][1] = up[0];
  matrix2[1][1] = up[1];
  matrix2[2][1] = up[2];
  matrix2[3][1] = 0.0;
  //------------------
  matrix2[0][2] = -forward[0];
  matrix2[1][2] = -forward[1];
  matrix2[2][2] = -forward[2];
  matrix2[3][2] = 0.0;
  //------------------
  matrix2[0][3] = matrix2[1][3] = matrix2[2][3] = 0.0;
  matrix2[3][3] = 1.0;
  //------------------
  matrix = matrix * matrix2; //MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
  matrix = glm::translate(matrix, glm::vec3(-eyePosition3D.x, -eyePosition3D.y, -eyePosition3D.z));
  //glhTranslatef2(resultMatrix, -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
  //------------------
  //memcpy(matrix, resultMatrix, 16 * sizeof(float));
  //matrix = resultMatrix;
}

vector3f GO_Utils::glComputeNormalOfPlane( vector3f& vec1, vector3f& vec2)
{
  vector3f out;

  out[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
  out[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
  out[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

  return out;
}*/
