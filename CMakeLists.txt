cmake_minimum_required(VERSION 3.8)

project(smf)

# options
# SMF config include file
set(SMF_CONFIG_FILE "" CACHE STRING "SMF config include file")
# Enable/disable hierarchical state machine support
option(SMF_ANCESTOR_SUPPORT "Enable SMF hierarchical state feature" OFF)

add_library(smf STATIC src/smf.c include/smf.h)
target_include_directories(smf PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_definitions(smf PUBLIC
    $<$<BOOL:${SMF_CONFIG_FILE}>:SMF_CONFIG=${SMF_CONFIG_FILE}>
    $<$<BOOL:${SMF_ANCESTOR_SUPPORT}>:CONFIG_SMF_ANCESTOR_SUPPORT>)
target_compile_options(smf PRIVATE
    $<$<OR:$<STREQUAL:$<C_COMPILER_ID>,GNU>,$<STREQUAL:$<C_COMPILER_ID>,Clang>>:-Wall -Werror -Wextra -Wpedantic -Wconversion>
    $<$<STREQUAL:$<C_COMPILER_ID>,MSVC>:/Wall /WX>)
