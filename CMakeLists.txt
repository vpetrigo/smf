cmake_minimum_required(VERSION 3.21)

project(smf VERSION 0.2.2)

# options
# SMF config include file
set(SMF_CONFIG_FILE "" CACHE STRING "SMF config include file")
# Enable/disable hierarchical state machine support
option(SMF_ANCESTOR_SUPPORT "Enable SMF hierarchical state feature" OFF)
option(SMF_INITIAL_TRANSITION "Enable SMF initial transition feature" OFF)
option(SMF_ENABLE_EXAMPLES "Enable examples" OFF)
option(SMF_INSTALL_LIBRARY "Install library enable" ON)

if (SMF_ENABLE_EXAMPLES)
    add_subdirectory(examples/)
endif ()

if (SMF_INITIAL_TRANSITION AND NOT SMF_ANCESTOR_SUPPORT)
    message(FATAL_ERROR "SMF_INITIAL_TRANSITION cannot be enabled without SMF_ANCESTOR_SUPPORT")
endif ()

add_library(smf STATIC src/smf.c include/smf.h)
target_include_directories(smf PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_compile_definitions(smf PUBLIC
        $<$<BOOL:${SMF_CONFIG_FILE}>:SMF_CONFIG=${SMF_CONFIG_FILE}>
        $<$<BOOL:${SMF_ANCESTOR_SUPPORT}>:CONFIG_SMF_ANCESTOR_SUPPORT=1>
        $<$<BOOL:${SMF_INITIAL_TRANSITION}>:CONFIG_SMF_INITIAL_TRANSITION=1>)
target_compile_options(smf PRIVATE
        $<$<OR:$<STREQUAL:$<C_COMPILER_ID>,GNU>,$<STREQUAL:$<C_COMPILER_ID>,Clang>>:-Wall -Werror -Wextra -Wpedantic -Wconversion>
        $<$<STREQUAL:$<C_COMPILER_ID>,MSVC>:/W3 /WX>)
add_library(smf::smf ALIAS smf)

# Install support
if (SMF_INSTALL_LIBRARY)
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
endif ()

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
