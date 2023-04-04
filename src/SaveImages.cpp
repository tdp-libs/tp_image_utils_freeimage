#include "tp_image_utils_freeimage/SaveImages.h"
#include "tp_image_utils/ColorMap.h"

#include "tp_utils/DebugUtils.h"

#include <FreeImage.h>

namespace tp_image_utils_freeimage
{

namespace
{

//##################################################################################################
struct CleanImage
{
  TP_NONCOPYABLE(CleanImage);
  FIBITMAP* i{nullptr};

  CleanImage() = default;

  ~CleanImage()
  {
    if(i)
      FreeImage_Unload(i);
  }
};


//##################################################################################################
bool convertImage(const tp_image_utils::ColorMap& image, CleanImage& tmp, bool includeAlpha)
{
  if(image.size() < 1 || image.width()<1 || image.height()<1)
    return false;

  tmp.i = FreeImage_Allocate(int(image.width()), int(image.height()), includeAlpha?32:24);

  if(!tmp.i)
    return false;

  size_t w = FreeImage_GetWidth(tmp.i);
  size_t h = FreeImage_GetHeight(tmp.i);

  if(w!=image.width() || h!=image.height())
    return false;

  const TPPixel* s = image.constData();
  for(size_t y=0; y<h; y++)
  {
    //From the FreeImage documentation
    //In FreeImage, FIBITMAP are based on a coordinate system that is upside down relative to usual
    //graphics conventions. Thus, the scanlines are stored upside down, with the first scan in
    //memory being the bottommost scan in the image.
    auto d = static_cast<uint8_t*>(FreeImage_GetScanLine(tmp.i, int((h-1)-y)));

    if(includeAlpha)
    {
      uint8_t* dMax = d+(w*4);
      for(; d<dMax; s++, d+=4)
      {
        d[2] = s->r;
        d[1] = s->g;
        d[0] = s->b;
        d[3] = s->a;
      }
    }
    else
    {
      uint8_t* dMax = d+(w*3);
      for(; d<dMax; s++, d+=3)
      {
        d[2] = s->r;
        d[1] = s->g;
        d[0] = s->b;
      }
    }
  }

  return true;
}

}

//##################################################################################################
bool saveImage(const std::string& path, const tp_image_utils::ColorMap& image)
{
  if(path.empty())
    return false;

  FREE_IMAGE_FORMAT format = FIF_PNG;

  bool includeAlpha=false;

  if(tpEndsWith(path, ".png"))
  {
    format = FIF_PNG;
    includeAlpha = true;
  }
  else if(tpEndsWith(path, ".jpg"))
  {
    format = FIF_JPEG;
    includeAlpha = false;
  }
  else if(tpEndsWith(path, ".jpeg"))
  {
    format = FIF_JPEG;
    includeAlpha = false;
  }
  else
    return false;

  CleanImage tmp;

  if(!convertImage(image, tmp, includeAlpha))
    return false;

  return FreeImage_Save(format, tmp.i, path.c_str());
}

//##################################################################################################
std::string saveImageToData(const tp_image_utils::ColorMap& image,
                            bool includeAlpha,
                            FREE_IMAGE_FORMAT format,
                            int flags)
{
  CleanImage tmp;

  if(!convertImage(image, tmp, includeAlpha))
  {
    tpWarning() << "Failed to convert image!";
    return std::string();
  }

  FIMEMORY* stream = FreeImage_OpenMemory();
  if(!FreeImage_SaveToMemory(format, tmp.i, stream, flags))
  {
    tpWarning() << "Failed to save image to data!";
    return std::string();
  }

  FreeImage_SeekMemory(stream, 0L, SEEK_SET);
  DWORD sizeBytes = 0;
  BYTE* data = nullptr;
  FreeImage_AcquireMemory(stream, &data, &sizeBytes);
  std::string results(reinterpret_cast<const char*>(data), sizeBytes);
  FreeImage_CloseMemory(stream);

  return results;
}

//##################################################################################################
std::string saveImageToData(const tp_image_utils::ColorMap& image)
{
  FREE_IMAGE_FORMAT format = FIF_PNG;
  int flags = 0;
  return saveImageToData(image, true, format, flags);
}

//##################################################################################################
std::string saveJPEGToData(const tp_image_utils::ColorMap& image, int quality)
{
  FREE_IMAGE_FORMAT format = FIF_JPEG;
  int flags = 0;

  flags |= JPEG_OPTIMIZE;

#if 1
  // The lower 7 bits can be used to pass in the quality directly
  // https://github.com/leapmotion/FreeImage/blob/master/Source/FreeImage/PluginJPEG.cpp#L1540
  flags |= quality & 0x7F;
#else
  if(quality<=10)
    flags |= JPEG_QUALITYBAD;

  else if(quality<=25)
    flags |= JPEG_QUALITYAVERAGE;

  else if(quality<=50)
    flags |= JPEG_QUALITYAVERAGE;

  else if(quality<=75)
    flags |= JPEG_QUALITYNORMAL;

  else
    flags |= JPEG_QUALITYSUPERB;
#endif

  return saveImageToData(image, true, format, flags);
}

//##################################################################################################
std::string saveWebPToData(const tp_image_utils::ColorMap& image, int quality)
{
  FREE_IMAGE_FORMAT format = FIF_WEBP;
  int flags = 0;

  // The lower 7 bits can be used to pass in the quality directly
  // https://github.com/imazen/freeimage/blob/master/Source/FreeImage/PluginWebP.cpp#L481
  flags |= quality & 0x7F;

  return saveImageToData(image, true, format, flags);
}

}
