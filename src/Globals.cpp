#include "tp_image_utils_freeimage/LoadImages.h"
#include "tp_image_utils_freeimage/SaveImages.h"

#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/SaveImages.h"

namespace tp_image_utils_freeimage
{
//##################################################################################################
void init()
{
  tp_image_utils::loadImage_         = loadImage;
  tp_image_utils::loadImageFromData_ = loadImageFromData;
  tp_image_utils::imagePaths_        = imagePaths;
  tp_image_utils::loadImages_        = loadImages;

  tp_image_utils::saveImage_         = saveImage;
  tp_image_utils::saveImageToData_   = saveImageToData;
}
}

