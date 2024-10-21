#include "tp_image_utils_freeimage/FreeImageErrors.h"

#include "tp_utils/DebugUtils.h"

#include <FreeImage.h>

namespace tp_image_utils_freeimage
{
namespace
{
//##################################################################################################
void freeImageError(FREE_IMAGE_FORMAT fif, const char* msg)
{
  const char* error = FreeImage_GetFormatFromFIF(fif);
  const char* n="null";
  errors().push_back("FreeImage error: " + std::string(msg?msg:n) + ", Format: " + (error?error:n));
}
}

//##################################################################################################
void initErrors()
{
  FreeImage_SetOutputMessage(freeImageError);
}

//##################################################################################################
void resetErrors()
{
  errors().clear();
}

//##################################################################################################
std::vector<std::string>& errors()
{
  thread_local static std::vector<std::string> errors;
  return errors;
}

}
