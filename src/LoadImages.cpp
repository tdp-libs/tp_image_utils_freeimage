#include "tp_image_utils_freeimage/LoadImages.h"
#include "tp_image_utils_freeimage/FreeImageErrors.h"

#include "tp_image_utils/LoadImages.h"

#include "tp_utils/FileUtils.h"
#include "tp_utils/DebugUtils.h"

namespace tp_image_utils_freeimage
{

namespace
{

//##################################################################################################
struct CleanImage
{
  TP_NONCOPYABLE(CleanImage);
  FIBITMAP* i{nullptr};

  CleanImage()=default;

  ~CleanImage()
  {
    if(i)
      FreeImage_Unload(i);
  }
};

//##################################################################################################
tp_image_utils::ColorMap convertToColorMap(const CleanImage& tmp, std::vector<std::string>& errors)
{
  if(!tmp.i)
    return tp_image_utils::ColorMap();

  CleanImage image;

  switch(FreeImage_GetImageType(tmp.i))
  {
    case FIT_BITMAP: [[fallthrough]];
    case FIT_RGB16:  [[fallthrough]];
    case FIT_RGBA16: [[fallthrough]];
    case FIT_RGBF:   [[fallthrough]];
    case FIT_RGBAF:
    {
      image.i = FreeImage_ConvertTo32Bits(tmp.i);
      break;
    }
    default:
    {
      CleanImage standard;
      standard.i = FreeImage_ConvertToStandardType(tmp.i);

      if(!standard.i)
      {
        errors.push_back("Failed to convert to standard type.");
        return tp_image_utils::ColorMap();
      }

      image.i = FreeImage_ConvertTo32Bits(standard.i);
      break;
    }
  }

  if(!image.i)
  {
    errors.push_back("Failed to convert to 32 bit.");
    return tp_image_utils::ColorMap();
  }

  auto w = FreeImage_GetWidth(image.i);
  auto h = FreeImage_GetHeight(image.i);

  if(w<1 || h<1)
  {
    errors.push_back("Image size too small, w: " + std::to_string(w) + " h: " + std::to_string(h));
    return tp_image_utils::ColorMap();
  }

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
    {
      errors.push_back("Failed to read scanline for row: " + std::to_string(y));
      return tp_image_utils::ColorMap();
    }

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

//##################################################################################################
void removeFalseErrors(const tp_image_utils::ColorMap& image, std::vector<std::string>& errors)
{
  // Kludge to remove a specific error that is produced for some seemingly valid JPEG files. Even
  // when the error presents the image loads fine.
  if(errors.size() == 1 && image.size()>0 && tpStartsWith(errors.front(), "FreeImage error: Not a JPEG file: starts with"))
  {
    errors.clear();
    return;
  }
}

}

//##################################################################################################
tp_image_utils::ColorMap loadImage(const std::string& path, std::vector<std::string>& errors)
{
  resetErrors();
  CleanImage tmp;
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
  tmp.i = FreeImage_Load(format, path.c_str());
  errors.insert(errors.end(), tp_image_utils_freeimage::errors().begin(), tp_image_utils_freeimage::errors().end());
  tp_image_utils::ColorMap image = convertToColorMap(tmp, errors);
  removeFalseErrors(image, errors);
  return image;
}

//##################################################################################################
tp_image_utils::ColorMap loadImage(const std::string& path, const std::function<void(FIBITMAP*)>& closure)
{
  resetErrors();

  CleanImage tmp;
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
  tmp.i = FreeImage_Load(format, path.c_str());

  if(tmp.i)
    closure(tmp.i);

  return convertToColorMap(tmp, errors());
}

//##################################################################################################
tp_image_utils::ColorMap loadImageFromData(const std::string& data, std::vector<std::string>& errors)
{
  resetErrors();

  CleanImage tmp;
  auto* stream = FreeImage_OpenMemory(const_cast<BYTE*>(reinterpret_cast<const BYTE*>(data.data())),
                                      DWORD(data.size()));
  FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(stream);
  tmp.i = FreeImage_LoadFromMemory(format, stream);
  FreeImage_CloseMemory(stream);
  errors.insert(errors.end(), tp_image_utils_freeimage::errors().begin(), tp_image_utils_freeimage::errors().end());
  tp_image_utils::ColorMap image = convertToColorMap(tmp, errors);
  removeFalseErrors(image, errors);
  return image;
}

//##################################################################################################
std::vector<tp_image_utils::ColorMap> loadImages(const std::string& path, std::vector<std::string>& names, int64_t maxBytes)
{
  std::vector<tp_image_utils::ColorMap> result;

  int64_t currentBytes=0;
  for(const auto& imagePath : imagePaths(path))
  {
    tp_image_utils::ColorMap dst = loadImage(imagePath, errors());
    if(dst.size()<1)
      continue;

    currentBytes += int64_t(dst.size()) * 4;

    if(currentBytes>maxBytes)
      break;

    result.push_back(dst);

    std::vector<std::string> parts;
    tpSplit(parts, imagePath, '/', TPSplitBehavior::KeepEmptyParts);

    names.push_back(parts.back());
  }

  return result;
}

//##################################################################################################
std::vector<std::string> imagePaths(const std::string& directory)
{
  return tp_utils::listFiles(directory, tp_image_utils::imageTypesSet());
}

}
