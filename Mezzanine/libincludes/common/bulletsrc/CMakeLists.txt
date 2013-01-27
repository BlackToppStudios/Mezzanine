cmake_minimum_required (VERSION 2.8)
project (Bullet)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
#set (${PROJECT_NAME}_BINARY_DIR "${Mezzanine_ROOT_DIR}data/${BuildPlatform}/bullet/")
#message ( STATUS "Putting Bullet binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# For this to work right, the bullet CMake.list, needs to be adjusted just a little bit
# I added "add_definitions( ${CommonBuildFlags} )" near the top just after the project command.

# Also a .patch file describing the changes in more detail was generated for the CMake changes for version 2.80

add_subdirectory ( bulletsvnsrc )

message ("${PROJECT_NAME} - End")

