cmake_minimum_required(VERSION 3.22)

project (day_7)

find_package(Threads)

# Get paths and files.
set(RootDir "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(UtilityDir "${RootDir}/utility")
file(GLOB_RECURSE SourceFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories ("${UtilityDir}")
add_executable(day_7 "${SourceFiles}")
set_target_properties( day_7
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${RootDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RootDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RootDir}/bin"
  VS_DEBUGGER_WORKING_DIRECTORY "${RootDir}/bin"
)