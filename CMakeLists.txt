# The name of our project is "PhysgameAndCatch". CMakeLists files in this project can
# refer to the root source directory of the project as ${PhysgameAndCatch_SOURCE_DIR} and
# to the root binary directory of the project as ${PhysgameAndCatch_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (PhysgameAndCatch)

option(PhysDebug "Compile with no optimizations and debug symbols" OFF)
if(${PhysDebug})
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Debug Specific Initialiations")
		set(CommonBuildFlags "-g")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Debug Initialiations")
		set(CommonBuildFlags " -fPIC -g")
	endif(${WIN32})
else(${PhysDebug})
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Specific Initialiations")
		set(CommonBuildFlags " -O2 -s")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Initialiations")
		set(CommonBuildFlags " -fPIC -O2 -s ")
	endif(${WIN32})
endif(${PhysDebug})

set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR})

#Add subprojects
add_subdirectory (physgameengine)
add_subdirectory (EngineDemo)

#add_subdirectory (Catch!) 
#add_subdirectory (physgamelauncher)
