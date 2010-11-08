# The name of our project is "Physgame". CMakeLists files in this project can
# refer to the root source directory of the project as ${PhysgameEngine_SOURCE_DIR} and
# to the root binary directory of the project as ${PhysgameEngine_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (PhysgameEngine)

message ("This is a test")
message ( ${PhysgameEngine_SOURCE_DIR}  )
set (PhysgameEngine_SOURCE_DIR "${PhysgameEngine_SOURCE_DIR}/src/")
set (PhysgameEngine_BINARY_DIR "${PhysgameEngine_BINARY_DIR}/bin/")
message ( ${PhysgameEngine_SOURCE_DIR}  )
message ( ${PhysgameEngine_BINARY_DIR}  )

#just testing a command.
#configure_file(test copy.txt COPYFILE)


