set(PRODUCTION false)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/extern)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(GLM_BUILD_TESTS OFF)

cmake_minimum_required(VERSION 3.20)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

project(BloodlustEngine VERSION 0.1)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

set(
	CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++20 -fno-exceptions \
	-Werror -Wdeprecated -Wall -Wextra -Wpedantic -Wconversion \
	-Wimplicit-fallthrough -Wmissing-prototypes -Wmissing-variable-declarations \
	-Wpadded -Wredundant-parens -Wswitch-default -Wunreachable-code-aggressive \
	-Wunaligned-access -Wno-empty-translation-unit"
)

if(PRODUCTION)
	set(CMAKE_CXX_FLAGS "-O3 -DBL_PRODUCTION ${CMAKE_CXX_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "-O0 -ggdb -ftrapv ${CMAKE_CXX_FLAGS}")
endif()

set(SOURCES "")
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

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(
	${PROJECT_NAME}
	${SDL3_LIBRARIES}
	SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf glm::glm
)
