PROJECT_PATH:=$(call my-dir)
LOCAL_PATH:=$(call my-dir)/../../../src

include $(CLEAR_VARS)
LOCAL_MODULE    := libgata

LOCAL_CFLAGS    := -g -Werror -fno-strict-aliasing -fsigned-char
LOCAL_CPPFLAGS := -fpermissive

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../inc

LOCAL_SRC_FILES  :=	graphic/gles20_impl/Gles20Factory.cpp \
					graphic/gles20_impl/Gles20Graphic.cpp \
					graphic/gles20_impl/Gles20Image.cpp \
					3rdparty/jsoncpp/json_reader.cpp \
					3rdparty/jsoncpp/json_value.cpp \
					3rdparty/jsoncpp/json_writer.cpp \
					3rdparty/tinyxml/tinystr.cpp \
					3rdparty/tinyxml/tinyxml.cpp \
					3rdparty/tinyxml/tinyxmlerror.cpp \
					3rdparty/tinyxml/tinyxmlparser.cpp \
					game/Game.cpp \
					graphic/GraphicMain.cpp \
					graphic/Transform.cpp \
					gui/ButtonWidget.cpp \
					gui/StandardButtonWidget.cpp \
					gui/Widget.cpp \
					sprite/Sprite.cpp \
					utils/MyUtils.cpp \
					utils/tga.cpp \
					utils/pvr.cpp \
					utils/UtfString.cpp 

include $(BUILD_STATIC_LIBRARY)
