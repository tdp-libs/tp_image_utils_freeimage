#ifndef tp_image_utils_freeimage_FreeImageErrors_h
#define tp_image_utils_freeimage_FreeImageErrors_h

#include "tp_image_utils_freeimage/Globals.h" // IWYU pragma: keep

namespace tp_image_utils_freeimage
{

//##################################################################################################
void initErrors();

//##################################################################################################
void resetErrors();

//##################################################################################################
std::vector<std::string>& errors();

}

#endif
