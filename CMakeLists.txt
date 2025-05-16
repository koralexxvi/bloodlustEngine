set(PRODUCTION false)

cmake_minimum_required(VERSION 3.20)
project(BloodlustEngine VERSION 0.1)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(
	CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++20 -fno-exceptions -Werror -Wdeprecated \
	-Wall -Wextra -Wpedantic -Wconversion -Wimplicit-fallthrough \
	-Wmissing-prototypes -Wmissing-variable-declarations -Wpadded \
	-Wredundant-parens -Wswitch-default -Wunreachable-code-aggressive \
	-Wunaligned-access \
	-Wno-empty-translation-unit -Wno-unused-parameter"
)
set(SOURCES "")

if(PRODUCTION)
	set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "-O0 -ggdb -ftrapv ${CMAKE_CXX_FLAGS}")
endif()

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

function(addSourcesRecursive directory)
	file(GLOB paths "${directory}/*")
	foreach(path ${paths})
		if(IS_DIRECTORY ${path})
			addSourcesRecursive(${path})
		else()
			cmake_path(GET path EXTENSION extension)
			if(extension STREQUAL ".cpp" OR extension STREQUAL ".c")
				list(APPEND SOURCES ${path})
			endif()
		endif()
	endforeach()
	set(SOURCES ${SOURCES} PARENT_SCOPE)
endfunction()
addSourcesRecursive(src)

# add_executable(${PROJECT_NAME} ${SOURCES})
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(
	${PROJECT_NAME}
	${SDL3_LIBRARIES}
	SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf
)
