cmake_minimum_required (VERSION 2.8)
project (Ogre)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
set (${PROJECT_NAME}_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/ogre/")
message ("Putting Bullet binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# For this to work right, the Ogre CMake.list, needs to be adjusted just a little bit
# addtionally I had to "SET (LIBRARY_OUTPUT_PATH ${Ogre_BINARY_DIR})" near the end of the output

add_subdirectory ( ogre )

message ( "${PROJECT_NAME} - End" )
