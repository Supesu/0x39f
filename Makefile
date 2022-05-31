include .env

CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(IMGUI_INCLUDE_PATH) -I$(EVE_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

EVE_SOURCE= $(wildcard Eve/Source/**/*.cpp) ${wildcard Eve/Source/Core/**/*.cpp}
IKIGAI_SOURCE= $(wildcard Ikigai/**/*.cpp) ${wildcard Ikigai/*.cpp}
EXTERNAL_SOURCE= $(wildcard External/**/*.cpp)

Ikigai: Ikigai/*.cpp Ikigai/*.hpp $(EVE_SOURCE)
	g++ $(CFLAGS) $(LDFLAGS) -o Build/Release/macOS/bin/Ikigai $(EVE_SOURCE) $(IKIGAI_SOURCE) $(EXTERNAL_SOURCE)

.PHONY: test clean

test: a.out
	./a.out
