#include "tp_image_utils_freeimage/LoadImages.h"
#include "tp_image_utils_freeimage/SaveImages.h"
#include "tp_image_utils_freeimage/FreeImageErrors.h"

#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/SaveImages.h"

#include <FreeImage.h>

namespace tp_image_utils_freeimage
{

//##################################################################################################
void init()
{
  initErrors();
  FreeImage_Initialise();
  FreeImage_SetPluginEnabled(FIF_JPEG, true);
  FreeImage_SetPluginEnabled(FIF_WEBP, true);

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

