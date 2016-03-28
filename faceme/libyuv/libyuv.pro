#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T19:46:32
#
#-------------------------------------------------
# This is needed for using syberos-application module
include(../syberos.pri)

TEMPLATE = lib
TARGET = yuv

QT       -= gui
QT       -= core

LIBS += -ljpeg

#指定目标文件(obj)的存放目录
OBJECTS_DIR += ./tmp

INCLUDEPATH += ./include
INCLUDEPATH += ./include/libyuv


HEADERS += \
    include/libyuv/basic_types.h \
    include/libyuv/compare.h \
    include/libyuv/convert_argb.h \
    include/libyuv/convert_from.h \
    include/libyuv/convert.h \
    include/libyuv/cpu_id.h \
    include/libyuv/format_conversion.h \
    include/libyuv/mjpeg_decoder.h \
    include/libyuv/planar_functions.h \
    include/libyuv/rotate_argb.h \
    include/libyuv/rotate.h \
    include/libyuv/scale_argb.h \
    include/libyuv/scale.h \
    include/libyuv/version.h \
    include/libyuv/video_common.h \
    include/libyuv.h \
    source/rotate_priv.h \
    source/row.h

SOURCES += \
    source/compare.cc \
    source/convert_argb.cc \
    source/convert_from.cc \
    source/convert.cc \
    source/cpu_id.cc \
    source/format_conversion.cc \
    source/mjpeg_decoder.cc \
    source/planar_functions.cc \
    source/rotate_argb.cc \
    source/rotate_neon.cc \
    source/rotate.cc \
    source/row_common.cc \
    source/row_neon.cc \
    source/row_posix.cc \
    source/row_win.cc \
    source/scale_argb.cc \
    source/scale.cc \
    source/video_common.cc

target.path = $$INSTALL_DIR/libs/
headers.path = /usr/include/eim/libyuv/

INSTALLS += target headers
