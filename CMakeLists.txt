set(PRODUCTION false)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(GLM_BUILD_TESTS OFF)

cmake_minimum_required(VERSION 3.20)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

project(BloodlustEngine VERSION 0.2)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
include(GNUInstallDirs)
include(sources.cmake)

add_compile_options(
	-std=c++${CMAKE_CXX_STANDARD} -fno-exceptions
	-Werror -Wdeprecated -Wall -Wextra -Wpedantic -Wconversion
	-Wimplicit-fallthrough -Wmissing-prototypes -Wmissing-variable-declarations
	-Wpadded -Wredundant-parens -Wswitch-default -Wunreachable-code-aggressive
	-Wunaligned-access -Wno-empty-translation-unit
)

if(PRODUCTION)
	add_compile_definitions(BL_PRODUCTION)
	add_compile_options(-O3)
else()
	add_compile_options(-O0 -ggdb -ftrapv)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(
	${PROJECT_NAME} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${SDL3_LIBRARIES} SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf glm::glm
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} STATIC IMPORTED)
set_target_properties(
	${PROJECT_NAME}::${PROJECT_NAME} PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
)
