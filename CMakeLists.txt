# CMakeList.txt : CMake project for pong, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project ("RayShacer")

if(NOT PLATFORM_NAME)
	set(PLATFORM_NAME "SDL2")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(GAME_SOURCES
	"main.cpp"
	"main.hpp"
	"Window/RenderWindow.cpp"
	"Window/RenderWindow.hpp"
	"GameManager/GameManager.cpp"
	"GameManager/GameManager.hpp"
	"Button/Button.cpp"
	"Button/Button.hpp"
	"World/World.cpp" 
	"Texture/TextDict.cpp"
	"Texture/TextDict.hpp"
 "Utility/vec3.cpp" "Utility/ivec3.cpp")
add_executable (RayShacer ${GAME_SOURCES})

# Add source to this project's executable.
target_link_libraries(RayShacer PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2::SDL2main )


file(COPY assets DESTINATION "${CMAKE_BINARY_DIR}")