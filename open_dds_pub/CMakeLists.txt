cmake_minimum_required(VERSION 3.1.0)

project(open_dds_pub)



include_directories(../)
include_directories(./)
include_directories(./)
include_directories($ENV{ThirdPart_5}/boost/include)
include_directories($ENV{HuaruSDK}/include)
include_directories($ENV{DDS_ROOT})
include_directories($ENV{ACE_ROOT})
include_directories($ENV{TAO_ROOT})

link_directories($ENV{ThirdPart_5}/boost/lib/vc140/x64)
link_directories($ENV{ACE_ROOT}/lib/)
link_directories($ENV{DDS_ROOT}/lib/)
link_directories($ENV{HuaruSDK}/lib/x64)

aux_source_directory(. DIR_SRCS)
aux_source_directory(../open_dds_idl DIR_SRCS)

message(warning ${PROJECT_NAME})

add_executable(open_dds_pub ${DIR_SRCS})