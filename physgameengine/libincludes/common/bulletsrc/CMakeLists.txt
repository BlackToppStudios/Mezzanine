cmake_minimum_required (VERSION 2.8)
project (Bullet)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
set (${PROJECT_NAME}_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/bullet/")
message ("Putting Bullet binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# For this to work right, the bullet CMake.list, needs to be adjusted just a little bit
# I added "add_definitions( ${CommonBuildFlags} )" near the top just after the project command
# addtionally I had to "SET (LIBRARY_OUTPUT_PATH ${Bullet_BINARY_DIR})" near the end of the output

add_subdirectory ( bulletsvnsrc )

message ("${PROJECT_NAME} - End")

