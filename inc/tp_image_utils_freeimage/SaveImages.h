#ifndef tp_image_utils_freeimage_SaveImages_h
#define tp_image_utils_freeimage_SaveImages_h

#include "tp_image_utils_freeimage/Globals.h"

#include "tp_image_utils/ColorMap.h"
#include "tp_image_utils/ByteMap.h"

#include <vector>

namespace tp_image_utils_freeimage
{

//##################################################################################################
void saveImage(const std::string& path, const tp_image_utils::ColorMap& image);

//##################################################################################################
std::string saveImageToData(const tp_image_utils::ColorMap& image);

}

#endif
