# © Copyright 2010 - 2011 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

cmake_minimum_required (VERSION 2.8)
project (UnitTests)
message ("${PROJECT_NAME} - Begin")

###############################################################################
#Variables

#add_definitions( ${CommonBuildFlags} )
#target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}")
message ( STATUS "Looking build resources in: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_DATA_DIR "${${PROJECT_NAME}_SOURCE_DIR}data/")
message ( STATUS "Base Directory for data input: ${${PROJECT_NAME}_DATA_DIR}")

set (${PROJECT_NAME}_SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}src/")
message ( STATUS "Looking for source code in: ${${PROJECT_NAME}_SRC_DIR}")

set (${PROJECT_NAME}_TEST_DIR "${${PROJECT_NAME}_SOURCE_DIR}tests/")
message ( STATUS "Looking for individual tests per files in: ${${PROJECT_NAME}_TEST_DIR}")

set (${PROJECT_NAME}_MEZZANINE_DIR "${ProjectRootDir}Mezzanine/")
message ( STATUS "Base for Mezzanine Unit Tests: ${${PROJECT_NAME}_MEZZANINE_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_DIR "${Mezzanine_BINARY_DIR}/bin/")
message ( STATUS "Mezzanine Unit Tests Binaries: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_SRC "${${PROJECT_NAME}_MEZZANINE_DIR}src/")
message ( STATUS "Generated Mezzanine Unit Tests source and headers: ${${PROJECT_NAME}_MEZZANINE_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Base Directory for Executable Test binaries and test data output: ${${PROJECT_NAME}_BINARY_DIR}")

set (Mezzanine_LIBINCLUDE_DIR ${ProjectRootDir}/Mezzanine/libincludes/)

###############################################################################
# Sources
File(GLOB ${PROJECT_NAME}_test "${${PROJECT_NAME}_TEST_DIR}"  "${${PROJECT_NAME}_TEST_DIR}*.h")

message( STATUS "Looking for Tests")
foreach(CurrentFileName ${${PROJECT_NAME}_test})
    String(REGEX MATCH "/([^/]*)\\.h$" DirtyName "${CurrentFileName}")
    String(LENGTH ${DirtyName} DirtyNameLength)
    MATH(EXPR TargetLength "${DirtyNameLength} - 3")
    String(SUBSTRING "${DirtyName}" "1" "${TargetLength}" TargetName)
    #message (${TargetLength})
    #message (${DirtyName})
    #message("${TargetName}")
    list(APPEND FoundTests ${TargetName})
endforeach(CurrentFileName ${${PROJECT_NAME}_test})

message(STATUS "Found Tests: ")
foreach(CurrentTest ${FoundTests})
    message(STATUS "   ${CurrentTest}")
endforeach(CurrentTest ${FoundTests})

message(STATUS "Generating Test Instantiation and include Code: ")
foreach(CurrentTest ${FoundTests})
    set(CppIncludes "${CppIncludes}
    #include \"${CurrentTest}.h\" ")

    set(CppInstantiations "${CppInstantiations}
    ${CurrentTest} ${CurrentTest}Implementation;")

    set(CppNameRegistration "${CppNameRegistration}
                this->operator[](AllLower(${CurrentTest}Implementation.Name())) = &${CurrentTest}Implementation;")
endforeach(CurrentTest ${FoundTests})

set(TestingScratchDir ${${PROJECT_NAME}_BINARY_DIR})
configure_file( "${${PROJECT_NAME}_ROOT_DIR}/autodetect.h.in"  "${${PROJECT_NAME}_BINARY_DIR}autodetect.h")

set(${PROJECT_NAME}_sources
    "${${PROJECT_NAME}_BINARY_DIR}autodetect.h"
    "${${PROJECT_NAME}_SRC_DIR}/consolelogic.h"
    "${${PROJECT_NAME}_SRC_DIR}/consolelogic.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/consolestringmanipulation.h"
    "${${PROJECT_NAME}_SRC_DIR}/consolestringmanipulation.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/mezztest.h"
    "${${PROJECT_NAME}_SRC_DIR}/mezztest.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/testdata.h"
    "${${PROJECT_NAME}_SRC_DIR}/testdata.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/testdatatools.h"
    "${${PROJECT_NAME}_SRC_DIR}/testdatatools.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/testenumerations.h"
    "${${PROJECT_NAME}_SRC_DIR}/testenumerations.cpp"

    "${${PROJECT_NAME}_SRC_DIR}/pugixml.h"
    "${${PROJECT_NAME}_SRC_DIR}/pugixml.cpp"
    "${${PROJECT_NAME}_SRC_DIR}/pugiconfig.h"



    "${${PROJECT_NAME}_SRC_DIR}/dagframescheduler.h" #exist to maintain compatibilty with DAG frameScheduler Unit tests

    ${${PROJECT_NAME}_test}
)

###############################################################################
# Functions to use to copy files at build time

#Description: This function copies items from the Data/lib directory into it's expected location in the output directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_DATA_DIR}${TargetFileInBinaryTree}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_DATA_DIR}${SourceFileInDataTree}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${TargetFileInBinaryTree}"
        VERBATIM
    )
endfunction(CopyFilesDuringBuild)

#Description: This function copies items from the output directory in Mezzanine into the location of the Catch executable.
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyLibraryFromMezzanineDuringBuild TargetName)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: $<TARGET_FILE:${TargetName}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${TargetName}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
endfunction(CopyLibraryFromMezzanineDuringBuild)

#Description: This function copies items from the Data/lib directory in Mezzanine into it's renamed location in the output directory for Unittests during build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesFromMezzanineDuringBuild TargetPath)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${TargetPath}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${TargetPath}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${TargetPath}"
        VERBATIM
    )
endfunction(CopyFilesFromMezzanineDuringBuild)

#Description: This function copies items from the Data/lib directory in Mezzanine into it's renamed location in the output directory for Unittests during build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesFromMezzanineDuringBuildAndRename TargetPath NewPath)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${TargetPath}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${TargetPath}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${NewPath}"
        VERBATIM
    )
endfunction(CopyFilesFromMezzanineDuringBuildAndRename)

##############################################################################
# Accessing correct headers

include_directories( ${${PROJECT_NAME}_SRC_DIR} ${${PROJECT_NAME}_TEST_DIR} ${${PROJECT_NAME}_BINARY_DIR}
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src/LinearMath"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
    "${Mezzanine_LIBINCLUDE_DIR}common/caudiosrc/cAudio2.1.0/include"
    ${OgreConfigHeaderIncludeDir}
    "${Mezz_SDLBuildDir}/SDL/include/"
    ${OgreHeaderIncludeDir}
)

if(${WIN32})
    message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
    include_directories()
    message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message ( STATUS "${PROJECT_NAME} - Begin Linux Specific Build Process")
    include_directories(
        ${Mezzanine_OgreBuildDir}/ogre/OgreMain/include/
        ${Mezzanine_OgreBuildDir}/ogre/include/
    )

    message ( STATUS "${PROJECT_NAME} - End Linux Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "APPLE")
    message ( STATUS "${PROJECT_NAME} - Begin Apple Specific Build Process")

    include_directories(
        ${Mezzanine_OgreBuildDir}/ogre/OgreMain/include/
        ${Mezzanine_OgreBuildDir}/ogre/include/
    )
endif(${CMAKE_SYSTEM_NAME} MATCHES "APPLE")

##############################################################################
#Executable definitions
include_directories(
    ${${PROJECT_NAME}_SRC_DIR}
    ${${PROJECT_NAME}_MEZZANINE_BIN_SRC}
    "${Mezzanine_LIBINCLUDE_DIR}/${BuildPlatform}/ogre" 			#only for windows which doesn't compile OGRE
    "${Mezz_SDLBuildDir}/SDL/include/"
)
link_directories( ${${PROJECT_NAME}_MEZZANINE_BIN_DIR} )

set(BigObjFiles "-Wa,-mbig-obj")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BigObjFiles}")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} Mezzanine OgreMain)

# Link to all of the Mezzanine Dependencies
target_link_libraries( ${PROJECT_NAME}
    ${MezzanineLinkName}
    ${NetworkLibrary}
    ${OgreLibraryAllNames}
    ${LuaLinkName}
    ${Bullet_LibraryNames}
    ${SDLLibraryNames}
    ${SoundLibraryNames}
)
#target_link_libraries( ${PROJECT_NAME} ${MezzanineLinkName} ${OgreLinkName})

target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )

##############################################################################
# File copies during all builds
message ( STATUS "${PROJECT_NAME} - Begin File Copy Process")

CopyFilesDuringBuild( common/r2skin.jpg data/r2skin.jpg )
CopyFilesDuringBuild( common/robot.mesh data/robot.mesh )
CopyFilesDuringBuild( common/robot.skeleton data/robot.skeleton )
CopyFilesDuringBuild( common/dejavu.png data/dejavu.png )
CopyFilesDuringBuild( common/dejavu.mta data/dejavu.mta )
CopyFilesDuringBuild( common/sounds/welcomefun-1.ogg data/ )

CopyFilesDuringBuild( common/ubuntu-font-family/ubuntu-font-licence-1.0.txt data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-B.ttf data/ubuntu-font-family/Ubuntu-B.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-BI.ttf data/ubuntu-font-family/Ubuntu-BI.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-I.ttf data/ubuntu-font-family/Ubuntu-I.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-R.ttf data/ubuntu-font-family/Ubuntu-R.ttf )

CopyFilesDuringBuild( common/music/cAudioTheme1.ogg data/music/cAudioTheme1.ogg )
CopyFilesDuringBuild( common/Examples.particle data/Examples.particle )
CopyFilesDuringBuild( common/flare.png data/flare.png )
CopyFilesDuringBuild( common/Examples.material data/Examples.material )
CopyFilesDuringBuild( common/clouds.jpg data/clouds.jpg )

CopyFilesDuringBuild( common/images/FlowerGarden-PublicDomain.jpg data/images/FlowerGarden-PublicDomain.jpg )
CopyFilesDuringBuild( common/images/ProceduralTexture-CellChessboardGenTest.png data/images/ProceduralTexture-CellChessboardGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CellGridGenTest.png data/images/ProceduralTexture-CellGridGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CloudGenTest.png data/images/ProceduralTexture-CloudGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-GradientGenTest.png data/images/ProceduralTexture-GradientGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-ImageGenTest.png data/images/ProceduralTexture-ImageGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-LabyrinthGenTest.png data/images/ProceduralTexture-LabyrinthGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-LerpGenTest.png data/images/ProceduralTexture-LerpGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-MarbleGenTest.png data/images/ProceduralTexture-MarbleGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-NoiseGenTest.png data/images/ProceduralTexture-NoiseGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-SolidGenTest.png data/images/ProceduralTexture-SolidGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-TextileGenTest.png data/images/ProceduralTexture-TextileGenTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-WoodGenTest.png data/images/ProceduralTexture-WoodGenTest.png )

CopyFilesDuringBuild( common/images/ProceduralTexture-AlphaMaskModTest.png data/images/ProceduralTexture-AlphaMaskModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-AlphaModTest.png data/images/ProceduralTexture-AlphaModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-BlitModTest.png data/images/ProceduralTexture-BlitModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-BlurModTest.png data/images/ProceduralTexture-BlurModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-ChannelModTest.png data/images/ProceduralTexture-ChannelModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CircleModTest.png data/images/ProceduralTexture-CircleModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-ColoursSaturationModTest.png data/images/ProceduralTexture-ColoursSaturationModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CombineAddClampModTest.png data/images/ProceduralTexture-CombineAddClampModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-ConvolutionModTest.png data/images/ProceduralTexture-ConvolutionModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CrackModTest.png data/images/ProceduralTexture-CrackModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidHypocycloidModTest.png data/images/ProceduralTexture-CycloidHypocycloidModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidHypotrochoidModTest.png data/images/ProceduralTexture-CycloidHypotrochoidModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidEpicycloidModTest.png data/images/ProceduralTexture-CycloidEpicycloidModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidEpitrochoidModTest.png data/images/ProceduralTexture-CycloidEpitrochoidModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidRoseCurveModTest.png data/images/ProceduralTexture-CycloidRoseCurveModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-CycloidLissajousCurveModTest.png data/images/ProceduralTexture-CycloidLissajousCurveModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-DilateModTest.png data/images/ProceduralTexture-DilateModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-EdgeDetectionSobelModTest.png data/images/ProceduralTexture-EdgeDetectionSobelModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-EllipseModTest.png data/images/ProceduralTexture-EllipseModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-FlareModTest.png data/images/ProceduralTexture-FlareModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-FlipPointModTest.png data/images/ProceduralTexture-FlipPointModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-InvertModTest.png data/images/ProceduralTexture-InvertModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-JitterModTest.png data/images/ProceduralTexture-JitterModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-LookupModTest.png data/images/ProceduralTexture-LookupModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-NormalsModTest.png data/images/ProceduralTexture-NormalsModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-OilpaintModTest.png data/images/ProceduralTexture-OilpaintModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-RandomPixelsModTest.png data/images/ProceduralTexture-RandomPixelsModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-RectangleModTest.png data/images/ProceduralTexture-RectangleModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-RotationZoomRotationOnlyModTest.png data/images/ProceduralTexture-RotationZoomRotationOnlyModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-SegmentModTest.png data/images/ProceduralTexture-SegmentModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-SharpenModTest.png data/images/ProceduralTexture-SharpenModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-ThresholdModTest.png data/images/ProceduralTexture-ThresholdModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-VortexModTest.png data/images/ProceduralTexture-VortexModTest.png )
CopyFilesDuringBuild( common/images/ProceduralTexture-WeighedLerpModTest.png data/images/ProceduralTexture-WeighedLerpModTest.png )

CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_LongerAxis.png data/images/TexturePacker-Guillotine-GS_LongerAxis.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_LongerLeftoverAxis.png data/images/TexturePacker-Guillotine-GS_LongerLeftoverAxis.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_MaximizeArea.png data/images/TexturePacker-Guillotine-GS_MaximizeArea.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_MinimizeArea.png data/images/TexturePacker-Guillotine-GS_MinimizeArea.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_ShorterAxis.png data/images/TexturePacker-Guillotine-GS_ShorterAxis.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-GS_ShorterLeftoverAxis.png data/images/TexturePacker-Guillotine-GS_ShorterLeftoverAxis.png )
CopyFilesDuringBuild( common/images/TexturePacker-Guillotine-Single-GS_LongerAxis.png data/images/TexturePacker-Guillotine-Single-GS_LongerAxis.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-MRP_BestAreaFit.png data/images/TexturePacker-MaxRects-MRP_BestAreaFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-MRP_BestLongSideFit.png data/images/TexturePacker-MaxRects-MRP_BestLongSideFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-MRP_BestShortSideFit.png data/images/TexturePacker-MaxRects-MRP_BestShortSideFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-MRP_ContactPointRule.png data/images/TexturePacker-MaxRects-MRP_ContactPointRule.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-MRP_TopLeftRule.png data/images/TexturePacker-MaxRects-MRP_TopLeftRule.png )
CopyFilesDuringBuild( common/images/TexturePacker-MaxRects-Single-MRP_ContactPointRule.png data/images/TexturePacker-MaxRects-Single-MRP_ContactPointRule.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-Single-SP_WorstWidthFit.png data/images/TexturePacker-Shelf-Single-SP_WorstWidthFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_BestAreaFit.png data/images/TexturePacker-Shelf-SP_BestAreaFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_BestHeightFit.png data/images/TexturePacker-Shelf-SP_BestHeightFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_BestWidthFit.png data/images/TexturePacker-Shelf-SP_BestWidthFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_FirstFit.png data/images/TexturePacker-Shelf-SP_FirstFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_NextFit.png data/images/TexturePacker-Shelf-SP_NextFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_WorstAreaFit.png data/images/TexturePacker-Shelf-SP_WorstAreaFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Shelf-SP_WorstWidthFit.png data/images/TexturePacker-Shelf-SP_WorstWidthFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Skyline-Single-SLP_BestAreaFit.png data/images/TexturePacker-Skyline-Single-SLP_BestAreaFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Skyline-SLP_BestAreaFit.png data/images/TexturePacker-Skyline-SLP_BestAreaFit.png )
CopyFilesDuringBuild( common/images/TexturePacker-Skyline-SLP_TopLeft.png data/images/TexturePacker-Skyline-SLP_TopLeft.png )

CopyFilesDuringBuild( common/ArchiveTest/FirstTestFile.txt data/ArchiveTest/FirstTestFile.txt )
CopyFilesDuringBuild( common/ArchiveTest/SecondTestFile.txt data/ArchiveTest/SecondTestFile.txt )
CopyFilesDuringBuild( common/ArchiveTest/TestFolder/SubDirTestFile.txt data/ArchiveTest/TestFolder/SubDirTestFile.txt )
CopyFilesDuringBuild( common/ArchiveTestData.zip data/ArchiveTestData.zip )

CopyFilesFromMezzanineDuringBuild( data/common/BulletZLibLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/lgpl-2.1.txt )
CopyFilesFromMezzanineDuringBuild( data/common/lgpl-3.0.txt )
CopyFilesFromMezzanineDuringBuild( data/common/Ogre-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/SDL-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/zlib-libpngLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/gpl-3.0.txt )
CopyFilesFromMezzanineDuringBuild( data/common/Licenses.txt )
CopyFilesFromMezzanineDuringBuild( data/common/PugiXML-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/XiphCopyrightNotice.txt )

#The Engine itself
CopyLibraryFromMezzanineDuringBuild( "Mezzanine" )

##############################################################################
#Copy System Dependant files during build time

# Windows File Copy Specific Items
if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
	include_directories(
		${Mezz_OgreBuildDir}/ogre/OgreMain/include/
		${Mezz_OgreBuildDir}/ogre/include/
	)
	CopyFilesFromMezzanineDuringBuildAndRename( data/linux/GLX_icon.png data/GLX_icon.png )
	message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Mac File Copy pecific Items
if(${APPLE})
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")

	message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

message ( STATUS "${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")

