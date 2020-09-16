project(findText)
cmake_minimum_required(VERSION 3.16)

set( EXECUTABLE_OUTPUT_PATH ../bin)
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
#if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
#	add_link_options("-mwindows")
#endif()

#add_link_options("-Wl,--large-address-aware")

add_executable(dbgDump src/dbgLog.cc src/dbgAc97.cc src/main.cc src/wavwrite.cc)


target_link_libraries(dbgDump -lexshit -lstdshit)
