cmake_minimum_required (VERSION 2.8)
project (Ogre)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
set (${PROJECT_NAME}_BINARY_DIR "${Mezzanine_ROOT_DIR}data/${BuildPlatform}/ogre/")
message ( STATUS "Putting Ogre binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# Long list of changes necessary to get ogre properly merged with our project.
# 

add_subdirectory ( ogresvnsrc )

message ("${PROJECT_NAME} - End")

