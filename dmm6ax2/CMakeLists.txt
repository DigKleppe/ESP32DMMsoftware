# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
#add_definitions(-D ESP32 -D ARDUINO=200 -D ARDUINO_ARCH_ESP32) 
#set(COMPONENT_PRIV_REQUIRES  "tft spidriver spiffs DVMdisplay")
set(COMPONENT_PRIV_REQUIRES  "DVMdisplay gui freertos")
list(APPEND EXTRA_COMPONENT_DIRS components/gui components/lvgl_esp32_drivers components/lvgl_esp32_drivers/lvgl_tft)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(dmm)
