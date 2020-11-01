cmake_minimum_required(VERSION 3.1.0)

project(xsim_dds)

include_directories(../)
include_directories(./)
include_directories($ENV{ThirdPart_5}/boost/include)
include_directories($ENV{HuaruSDK}/include)

link_directories($ENV{ThirdPart_5}/boost/lib/vc140/x64)

link_directories($ENV{HuaruSDK}/lib/x64)

add_executable(xsim_dds main.cpp)