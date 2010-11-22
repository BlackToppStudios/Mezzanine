cmake_minimum_required (VERSION 2.8)
project (Bullet)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
set (${PROJECT_NAME}_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/bullet/")
set (PROJECT_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/bullet/")
set (LIBRARY_OUTPUT_PATH "${physgame_ROOT_DIR}data/${BuildPlatform}/bullet/")
message ("Putting Bullet binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# For this to work right, the bullet CMake.list, needs to be adjusted just a little bit
# I added "add_definitions( -fPIC -O3 -s )" near the top just after the project command
# addtionally I had to "SET (LIBRARY_OUTPUT_PATH ${Bullet_BINARY_DIR})" near the end of the output

add_subdirectory ( bulletsvnsrc )





###
# Deprecated code below, Pending Bullet completion for removal

#The main purpose of this build is to create a single libBullet.a to be used with physgame
set(${PROJECT_NAME}_LibraryFiles
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/Extras/ConvexDecomposition/libConvexDecomposition.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/Extras/GIMPACTUtils/libGIMPACTUtils.a"

	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/Extras/Serialize/BulletFileLoader/libBulletFileLoader.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a"

	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletCollision/libBulletCollision.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletDynamics/libBulletDynamics.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/libBulletMultiThreaded.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletSoftBody/libBulletSoftBody.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/LinearMath/libLinearMath.a"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/MiniCL/libMiniCL.a"

	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/GpuSoftBodySolvers/CPU/libBulletSoftBodySolvers_CPU.a"
#	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/libBulletSoftBodySolvers_OpenCL.a"
)

set( ${PROJECT_NAME}_LibraryNames
	"ConvexDecomposition"
	"GIMPACTUtils"

	"BulletFileLoader"
	"BulletWorldImporter"

	"BulletCollision"
	"BulletDynamics"
	"BulletMultiThreaded"
	"BulletSoftBody"
	"LinearMath"
	"MiniCL"

	"BulletSoftBodySolvers_CPU"
#	"BulletSoftBodySolvers_OpenCL"
)

set(${PROJECT_NAME}_LibraryDirs
	"${${PROJECT_NAME}_BINARY_DIR}/Extras/ConvexDecomposition/"
	"${${PROJECT_NAME}_BINARY_DIR}/Extras/GIMPACTUtils/"

	"${${PROJECT_NAME}_BINARY_DIR}/Extras/Serialize/BulletFileLoader/"
	"${${PROJECT_NAME}_BINARY_DIR}/Extras/Serialize/BulletWorldImporter/"

	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletCollision/"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletDynamics/"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletSoftBody/"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/LinearMath/"
	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/MiniCL/"

	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/GpuSoftBodySolvers/CPU/"
#	"${${PROJECT_NAME}_BINARY_DIR}/bulletsvnsrc/src/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/"
)
message ("${PROJECT_NAME} - End")

