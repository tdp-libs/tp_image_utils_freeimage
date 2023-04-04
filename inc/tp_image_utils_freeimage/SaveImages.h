#ifndef tp_image_utils_freeimage_SaveImages_h
#define tp_image_utils_freeimage_SaveImages_h

#include "tp_image_utils_freeimage/Globals.h"

namespace tp_image_utils
{
class ColorMap;
}

namespace tp_image_utils_freeimage
{

//##################################################################################################
bool saveImage(const std::string& path, const tp_image_utils::ColorMap& image);

//##################################################################################################
std::string saveImageToData(const tp_image_utils::ColorMap& image);

//##################################################################################################
std::string saveJPEGToData(const tp_image_utils::ColorMap& image, int quality);

//##################################################################################################
std::string saveWebPToData(const tp_image_utils::ColorMap& image, int quality);

}

#endif
