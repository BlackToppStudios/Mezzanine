# The name of our project is "PhysgameAndCatch". CMakeLists files in this project can
# refer to the root source directory of the project as ${PhysgameAndCatch_SOURCE_DIR} and
# to the root binary directory of the project as ${PhysgameAndCatch_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (PhysgameAndCatch)

option(PhysDebug "Compile with no optimizations and debug symbols" OFF)

add_subdirectory (physgameengine)
#add_subdirectory (Catch!) 
#add_subdirectory (EngineDemo)
#add_subdirectory (physgamelauncher)
