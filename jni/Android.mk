LOCAL_PATH := $(call my-dir)

# ============================================================================ #
# Prebuilt Static Libraries
# ============================================================================ #

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := curl/curl-android-$(TARGET_ARCH_ABI)/lib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := curl/openssl-android-$(TARGET_ARCH_ABI)/lib/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := curl/openssl-android-$(TARGET_ARCH_ABI)/lib/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := dobby
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

# ============================================================================ #
# Main Shared Library Module
# ============================================================================ #

include $(CLEAR_VARS)
LOCAL_MODULE := MEOW

# Compiler flags
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++11 -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -Wall
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/xdl \
    $(LOCAL_PATH)/include/ImGui \
    $(LOCAL_PATH)/include/ImGui/backends \
    $(LOCAL_PATH)/include/ImGui/font \
    $(LOCAL_PATH)/include/Hook/Dobby \
    $(LOCAL_PATH)/include/Hook/KittyMemory \
    $(LOCAL_PATH)/curl/curl-android-$(TARGET_ARCH_ABI)/include \
    $(LOCAL_PATH)/curl/openssl-android-$(TARGET_ARCH_ABI)/include \
    $(LOCAL_PATH)/libzip \
    $(LOCAL_PATH)/AxiomRuntime \
    $(LOCAL_PATH)/AxiomRuntime/includes \
    $(LOCAL_PATH)/Il2cpp \
    $(LOCAL_PATH) 

# Source files
FILE_LIST := \
    $(wildcard $(LOCAL_PATH)/*.cpp*) \
    $(wildcard $(LOCAL_PATH)/src/*.cpp*) \
    $(wildcard $(LOCAL_PATH)/src/xdl/*.c*) \
    $(wildcard $(LOCAL_PATH)/src/ImGui/*.cpp*) \
    $(wildcard $(LOCAL_PATH)/src/ImGui/backends/*.cpp*) \
    $(wildcard $(LOCAL_PATH)/src/Hook/KittyMemory/*.cpp*) \
    $(wildcard $(LOCAL_PATH)/src/Utils/*.cpp*) \
    $(LOCAL_PATH)/include/Hook/KittyMemory/SubstrateDebug.cpp \
    $(LOCAL_PATH)/include/Hook/KittyMemory/SubstrateHook.cpp \
    $(LOCAL_PATH)/include/Hook/KittyMemory/SymbolFinder.cpp \
    xhook/xhook.c \
    AxiomRuntime/enc/md5.cpp \
    xhook/xh_core.c \
    xhook/xh_elf.c \
    xhook/xh_jni.c \
    Oxorany/oxorany.cpp \
    xhook/xh_log.c \
    xhook/xh_util.c \
    xhook/xh_version.c \
    Il2cpp/Il2cpp.cpp \
    Il2cpp/il2cpp-class.cpp \
    Il2cpp/Util.cpp \
    include/IL2CppSDKGenerator/IL2Cpp/Fake_dlfcn.cpp \
    include/IL2CppSDKGenerator/IL2Cpp/Il2Cpp.cpp \
    include/IL2CppSDKGenerator/IL2Cpp/plthook_elf.cpp \
    include/IL2CppSDKGenerator/IL2Cpp/Tools.cpp \
    $(wildcard $(LOCAL_PATH)/libzip/*.c)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# Libraries to link
LOCAL_STATIC_LIBRARIES := libcurl libssl libcrypto libdobby
LOCAL_LDLIBS := -llog -landroid -lz -lEGL -lGLESv2 -lGLESv3

# Enable C++ exceptions
LOCAL_CPP_FEATURES := exceptions

# Build shared library
include $(BUILD_SHARED_LIBRARY)