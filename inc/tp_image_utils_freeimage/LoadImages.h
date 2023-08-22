#ifndef tp_image_utils__freeimage_LoadImages_h
#define tp_image_utils__freeimage_LoadImages_h

#include "tp_image_utils_freeimage/Globals.h" //IWYU pragma: keep

#include "tp_image_utils/ColorMap.h"

#include <FreeImage.h>

#include <vector>

namespace tp_image_utils_freeimage
{

//##################################################################################################
tp_image_utils::ColorMap loadImage(const std::string& path);

//##################################################################################################
tp_image_utils::ColorMap loadImage(const std::string& path, const std::function<void(FIBITMAP*)>& closure);

//##################################################################################################
tp_image_utils::ColorMap loadImageFromData(const std::string& data);

//##################################################################################################
std::vector<tp_image_utils::ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes=1073741824);

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& path);

}

#endif
