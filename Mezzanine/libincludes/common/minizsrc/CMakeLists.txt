PROJECT(miniz)
cmake_minimum_required(VERSION 2.8)

set(miniz_SOURCE MiniZ/miniz.c MiniZ/miniz_zip.c MiniZ/miniz_tinfl.c MiniZ/miniz_tdef.c)

add_library(miniz ${miniz_SOURCE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/MiniZ/)