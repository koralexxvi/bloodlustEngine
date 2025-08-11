cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(BloodlustEngine VERSION 0.2)

include(CMakePackageConfigHelpers)
include(FetchContent)
include(GNUInstallDirs)

option(isProduction "Create production build" false)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/lib)

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(glm REQUIRED)

include(sources.cmake)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_options(
	${PROJECT_NAME} PRIVATE
	-fno-exceptions -fno-math-errno -Werror -Wdeprecated -Wall -Wextra -Wpedantic -Wconversion
	-Wimplicit-fallthrough -Wmissing-prototypes -Wmissing-variable-declarations -Wpadded
	-Wredundant-parens -Wswitch-default -Wunreachable-code-aggressive -Wunaligned-access
)

if(isProduction)
	target_compile_definitions(${PROJECT_NAME} PRIVATE BL_PRODUCTION)
	target_compile_options(${PROJECT_NAME} PRIVATE -O3)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -O0 -ggdb -ftrapv)
endif()

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${SDL3_LIBRARIES} SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf glm::glm
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(includePath "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(FILES ${HEADERS_MAIN} DESTINATION ${includePath})
install(FILES ${HEADERS_DATATYPE} DESTINATION ${includePath}/datatype)
install(FILES ${HEADERS_OBJECT} DESTINATION ${includePath}/object)
install(FILES ${HEADERS_RENDER} DESTINATION ${includePath}/render)

install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${CMAKE_PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 0)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION 0)
set_property(
	TARGET ${PROJECT_NAME}
	APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION
)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	VERSION "${CMAKE_PROJECT_VERSION}"
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
