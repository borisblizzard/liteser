cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(liteser)

include("../../../hltypes/android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
)

add_library(liteser STATIC ${CppSrc})

include_directories(AFTER "../../include/liteser")
include_directories(AFTER "../../../hltypes/xlxml/include")
include_directories(AFTER "../../../hltypes/include")
include_directories(AFTER "../../../gtypes/include")

add_definitions(
	-DLITESER_EXPORTS
)

target_link_libraries(
	liteser
)
