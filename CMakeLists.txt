cmake_minimum_required(VERSION 3.8)

project(smf VERSION 0.1.0)

# options
# SMF config include file
set(SMF_CONFIG_FILE "" CACHE STRING "SMF config include file")
# Enable/disable hierarchical state machine support
option(SMF_ANCESTOR_SUPPORT "Enable SMF hierarchical state feature" OFF)

add_library(smf STATIC src/smf.c include/smf.h)
target_include_directories(smf PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_compile_definitions(smf PUBLIC
    $<$<BOOL:${SMF_CONFIG_FILE}>:SMF_CONFIG=${SMF_CONFIG_FILE}>
    $<$<BOOL:${SMF_ANCESTOR_SUPPORT}>:CONFIG_SMF_ANCESTOR_SUPPORT>)
target_compile_options(smf PRIVATE
    $<$<OR:$<STREQUAL:$<C_COMPILER_ID>,GNU>,$<STREQUAL:$<C_COMPILER_ID>,Clang>>:-Wall -Werror -Wextra -Wpedantic -Wconversion>
    $<$<STREQUAL:$<C_COMPILER_ID>,MSVC>:/Wall /WX>)

# Install support
include(GNUInstallDirs)
install(TARGETS smf
    EXPORT smf-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES include/smf.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT smf-targets
    FILE smf-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# CMake FindPackage support
include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
