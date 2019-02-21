#include "tp_image_utils_freeimage/SaveImages.h"
#include "tp_image_utils/ColorMap.h"

#include "tp_utils/Globals.h"

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
bool convertImage(const tp_image_utils::ColorMap& image, CleanImage& tmp)
{
  if(image.size() < 1 || image.width()<1 || image.height()<1)
    return false;

  tmp.i = FreeImage_Allocate(int(image.width()), int(image.height()), 32);

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
    uint8_t* d = static_cast<uint8_t*>(FreeImage_GetScanLine(tmp.i, int((h-1)-y)));
    uint8_t* dMax = d+(w*4);
    for(; d<dMax; s++, d+=4)
    {
      d[2] = s->r;
      d[1] = s->g;
      d[0] = s->b;
      d[3] = s->a;
    }
  }

  return true;
}

}

//##################################################################################################
void saveImage(const std::string& path, const tp_image_utils::ColorMap& image)
{
  if(path.empty())
    return;

  FREE_IMAGE_FORMAT format = FIF_PNG;

  if(tpEndsWith(path, ".png"))
    format = FIF_PNG;
  else if(tpEndsWith(path, ".jpg"))
    format = FIF_JPEG;
  else if(tpEndsWith(path, ".jpeg"))
    format = FIF_JPEG;
  else
    return;

  CleanImage tmp;

  if(!convertImage(image, tmp))
    return;

  FreeImage_Save(format, tmp.i, path.c_str());
}

//##################################################################################################
std::string saveImageToData(const tp_image_utils::ColorMap& image)
{
  FREE_IMAGE_FORMAT format = FIF_PNG;

  CleanImage tmp;

  if(!convertImage(image, tmp))
    return std::string();

  FIMEMORY* stream = FreeImage_OpenMemory();
  FreeImage_SaveToMemory(format, tmp.i, stream);
  DWORD sizeBytes = 0;
  BYTE* data = nullptr;
  FreeImage_AcquireMemory(stream, &data, &sizeBytes);
  std::string results(reinterpret_cast<const char*>(data), sizeBytes);
  FreeImage_CloseMemory(stream);

  return results;
}

}
