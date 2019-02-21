#include "tp_image_utils_freeimage/LoadImages.h"
#include "tp_image_utils/LoadImages.h"
#include "tp_image_utils/Scale.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/JSONUtils.h"
#include "tp_utils/FileUtils.h"

#include <FreeImage.h>

namespace tp_image_utils_freeimage
{

namespace
{

//##################################################################################################
struct CleanImage
{
  FIBITMAP* i{nullptr};
  ~CleanImage()
  {
    if(i)
      FreeImage_Unload(i);
  }
};

//##################################################################################################
tp_image_utils::ColorMap comvertToColorMap(const CleanImage& tmp)
{
  if(!tmp.i)
    return tp_image_utils::ColorMap();

  CleanImage image;
  image.i = FreeImage_ConvertTo32Bits(tmp.i);

  if(!image.i)
    return tp_image_utils::ColorMap();

  auto w = FreeImage_GetWidth(image.i);
  auto h = FreeImage_GetHeight(image.i);

  if(w<1 || h<1)
    return tp_image_utils::ColorMap();

  tp_image_utils::ColorMap dst{w, h};
  TPPixel* d = dst.data();
  for(size_t y=0; y<h; y++)
  {
    //From the FreeImage documentation
    //In FreeImage, FIBITMAP are based on a coordinate system that is upside down relative to usual
    //graphics conventions. Thus, the scanlines are stored upside down, with the first scan in
    //memory being the bottommost scan in the image.
    const uint8_t* s = static_cast<uint8_t*>(FreeImage_GetScanLine(image.i, int((h-1)-y)));

    if(!s)
      return tp_image_utils::ColorMap();

    const uint8_t* sMax = s+(w*4);
    for(; s<sMax; d++, s+=4)
    {
      d->r = s[2];
      d->g = s[1];
      d->b = s[0];
      d->a = s[3];
    }
  }

  return dst;
}

}

//##################################################################################################
tp_image_utils::ColorMap loadImage(const std::string& path)
{
  CleanImage tmp;
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
  tmp.i = FreeImage_Load(format, path.c_str());
  return comvertToColorMap(tmp);
}

//##################################################################################################
tp_image_utils::ColorMap loadImageFromData(const std::string& data)
{
  CleanImage tmp;
  FIMEMORY* stream = FreeImage_OpenMemory(const_cast<BYTE*>(reinterpret_cast<const BYTE*>(data.data())),
                                          DWORD(data.size()));
  FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(stream);
  tmp.i = FreeImage_LoadFromMemory(format, stream);
  FreeImage_CloseMemory(stream);
  return comvertToColorMap(tmp);
}

//##################################################################################################
std::vector<tp_image_utils::ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes)
{
  std::vector<tp_image_utils::ColorMap> result;

  int64_t currentBytes=0;
  for(const auto& imagePath : imagePaths(path))
  {
    tp_image_utils::ColorMap dst = loadImage(imagePath);
    if(dst.size()<1)
      continue;

    currentBytes += int64_t(dst.size()) * 4;

    if(currentBytes>maxBytes)
      break;

    result.push_back(dst);

    std::vector<std::string> parts;
    tpSplit(parts, imagePath, '/', tp_utils::SplitBehavior::KeepEmptyParts);

    names.push_back(parts.back());
  }

  return result;
}

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& path)
{
  return tp_utils::listFiles(path, tp_image_utils::imageTypesSet());
}

}
