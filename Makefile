# If RACK_DIR is not defined when calling the Makefile, default to the SDK location
RACK_DIR ?= /path/to/Rack-SDK

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS += -Isrc
CXXFLAGS += -Isrc

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
# SOURCES += $(wildcard src/*.cpp)
SOURCES += $(shell find src -name "*.cpp")

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

run:
	"/C/Program Files/VCV/Rack2Free/Rack.exe" -d -u "/C/Users/djand/AppData/Local/Rack2" -s "/C/Program Files/VCV/Rack2Free" 2>&1 | cat

copy:
	cp "C:\Dev\DanielDaviesVCV\dist\FreezerLabs" "C:\Users\djand\AppData\Local\Rack2\plugins" -r

copy-pro:
	cp "C:\Dev\DanielDaviesVCV\dist\FreezerLabs" "C:\Users\djand\AppData\Local\Rack2\plugins-win-x64" -r

build-and-run:
	make install && make copy && make run