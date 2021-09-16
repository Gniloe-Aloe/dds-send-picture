project(helloworld LANGUAGES C CXX)


cmake_minimum_required(VERSION 3.5)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

#find_package(OpenCV REQUIRED)

#include_directories("/usr/local/include/opencv4/opencv2/")


add_executable(ddscxxHelloworldPublisher publisher.cpp)
add_executable(ddscxxHelloworldSubscriber subscriber.cpp)

#my timer and picture libs
add_library(TIMER STATIC timer.cpp timer.hpp)
add_library(PICTURE STATIC picture.cpp picture.hpp)
add_library(BASE64 STATIC base64.cpp base64.hpp)

# my opencv 
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


# Link both executables to idl data type library and ddscxx.
target_link_libraries(ddscxxHelloworldPublisher CycloneDDS-CXX::ddscxx
                                                TIMER
                                                PICTURE
                                                BASE64
                                                ${OpenCV_LIBS}
                                                pthread
                                                )

target_link_libraries(ddscxxHelloworldSubscriber CycloneDDS-CXX::ddscxx
                                                 TIMER
                                                 PICTURE
                                                 BASE64
                                                 ${OpenCV_LIBS}
                                                 pthread
                                                 )

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(ddscxxHelloworldPublisher PRIVATE -fno-analyzer)
    target_compile_options(ddscxxHelloworldSubscriber PRIVATE -fno-analyzer)
  endif()
endif()

#set_property(TARGET ddscxxHelloworldPublisher PROPERTY CXX_STANDARD 17)
#set_property(TARGET ddscxxHelloworldSubscriber PROPERTY CXX_STANDARD 17)

set_target_properties(
    ddscxxHelloworldPublisher PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
   )

   set_target_properties(
    ddscxxHelloworldSubscriber PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
   )
