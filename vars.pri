TARGET = tp_image_utils_freeimage
TEMPLATE = lib

DEFINES += tp_image_utils_FREEIMAGE_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_image_utils_freeimage/Globals.h

SOURCES += src/LoadImages.cpp
HEADERS += inc/tp_image_utils_freeimage/LoadImages.h

SOURCES += src/SaveImages.cpp
HEADERS += inc/tp_image_utils_freeimage/SaveImages.h
