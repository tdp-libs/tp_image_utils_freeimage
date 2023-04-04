#include "tp_image_utils_freeimage/LoadImages.h"
#include "tp_image_utils_freeimage/SaveImages.h"

#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/SaveImages.h"

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
  tpWarning() << "FreeImage error: " << (msg?msg:n) << ", Format: " << (error?error:n);
}
}

//##################################################################################################
void init()
{
  FreeImage_SetOutputMessage(freeImageError);
  FreeImage_Initialise();
  FreeImage_SetPluginEnabled(FIF_JPEG, true);

  tp_image_utils::loadImage_         = loadImage;
  tp_image_utils::loadImageFromData_ = loadImageFromData;
  tp_image_utils::imagePaths_        = imagePaths;
  tp_image_utils::loadImages_        = loadImages;

  tp_image_utils::saveImage_         = saveImage;
  tp_image_utils::saveImageToData_   = saveImageToData;
  tp_image_utils::saveJPEGToData_    = saveJPEGToData;
  tp_image_utils::saveWebPToData_    = saveWebPToData;
}
}

