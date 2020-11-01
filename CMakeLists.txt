cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG $ENV{HuaruSDK})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG $ENV{HuaruSDK})

project(dds_demo)


add_subdirectory(open_dds_sub)
add_subdirectory(open_dds_pub)
add_subdirectory(xsim_dds)