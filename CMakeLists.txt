# CMakeList.txt : CMake project for pong, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.23)

project ("RayShacer" LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(SDL_ttf REQUIRED)
    set(TTF_LIBS SDL2_ttf)
else()
    find_package(SDL2_ttf CONFIG REQUIRED)
	set(TTF_LIBS SDL2_ttf::SDL2_ttf)
endif()

set(GAME_SOURCES
	"main.cpp"
	"main.hpp"
	"Window/RenderWindow.cpp"
	"Window/RenderWindow.hpp"
	"GameManager/GameManager.cpp"
	"GameManager/GameManager.hpp"
	"Button/Button.cpp"
	"Button/Button.hpp"
	"Texture/TextDict.cpp"
	"Texture/TextDict.hpp"
)

set( GPU_SOURCES
	"Rendering/Rendering.cu"
)

add_library(gpu_stuff STATIC ${GPU_SOURCES})
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

# Request that particles be built with -std=c++11
# As this is a public compile feature anything that links to 
# particles will also build with -std=c++11
set_property(TARGET gpu_stuff PROPERTY CUDA_STANDARD 26)
# We need to explicitly state that we need all CUDA files in the 
# particle library to be built with -dc as the member functions 
# could be called by other libraries and executables
set_target_properties(gpu_stuff PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

# Add source to this project's executable.
add_executable (RayShacer ${GAME_SOURCES})
set_property(TARGET RayShacer PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET RayShacer PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)
target_link_libraries(RayShacer PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image ${TTF_LIBS} SDL2::SDL2main gpu_stuff)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")

if(APPLE)
  # We need to add the default path to the driver (libcuda.dylib) as an rpath,
  # so that the static cuda runtime can find it at runtime.
  set_property(TARGET RayShacer PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
endif()


file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}")