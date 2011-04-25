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
		set(CommonBuildFlags " -O2 -s ")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Initialiations")
		set(CommonBuildFlags " -fPIC -O2 -s")
	endif(${WIN32})
endif(${PhysDebug})

option(PhysProfile "Executables will gather runtime statistics." OFF)
if(${PhysProfile})
	message ("${PROJECT_NAME} - Enabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -pg")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=1")
else(${PhysProfile})
	message ("${PROJECT_NAME} - Disabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=0")
endif(${PhysProfile})

option(PhysXML "Enable XML and XML serialization supprt." ON)
if(${PhysXML})
	message ("${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=1")
else(${PhysXML})
	message ("${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=0")
endif(${PhysXML})

if(${PhysDebug})
	message ("${PROJECT_NAME} - Enabling PHYSDEBUG Messages")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=1")
else(${PhysDebug})
	message ("${PROJECT_NAME} - Disabling PHYSDEBUG Messages")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=0")
endif(${PhysDebug})



set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR})

#Add subprojects
add_subdirectory (physgameengine)
add_subdirectory (EngineDemo)
add_subdirectory (Catch!) 
add_subdirectory (UnitTests) 


