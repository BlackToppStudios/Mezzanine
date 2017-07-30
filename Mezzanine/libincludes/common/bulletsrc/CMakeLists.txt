cmake_minimum_required (VERSION 2.8)
project (Bullet)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
#set (${PROJECT_NAME}_BINARY_DIR "${Mezzanine_ROOT_DIR}data/${BuildPlatform}/bullet/")
#message ( STATUS "Putting Bullet binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# For this to work right, the bullet CMake.list, needs to be adjusted just a little bit
# I added "add_definitions( ${CommonBuildFlags} )" near the top just after the project command.

# Also a .patch file describing the changes in more detail was generated for the CMake changes for version 2.80

set(BUILD_AMD_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_INTEL_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_NVIDIA_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_MINICL_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(USE_GLUT OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "Do not install libs!" FORCE)
set(INSTALL_EXTRA_LIBS OFF CACHE BOOL "Do not install libs!" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build unit tests!" FORCE)
set(BUILD_MULTITHREADING ON CACHE BOOL "Do build multithreading constructs!" FORCE)

# Additions since the upgrade to bullet 3 repo
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Do not build any graphical tools!" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "Do not build bullet 3!" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "Do not build CLSocket!" FORCE)
set(BUILD_ENET OFF CACHE BOOL "Do not build ENet!" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_PYBULLET OFF CACHE BOOL "Do not build python!" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build dlls!" FORCE)
set(CLSOCKET_DEP_ONLY OFF CACHE BOOL "Do not build CLSocket!" FORCE)
set(CLSOCKET_SHARED OFF CACHE BOOL "Do not build CLSocket!" FORCE)

set(BULLET2_USE_THREAD_LOCKS ${Mezz_Physics_MultiThreaded} CACHE BOOL "Conditionally enable multithreaded bullet" FORCE)

add_subdirectory ( bulletsvnsrc )

mark_as_advanced( FORCE
    USE_DOUBLE_PRECISION
    USE_MSVC_RUNTIME_LIBRARY_DLL
    USE_MSVC_INCREMENTAL_LINKING
    USE_CUSTOM_VECTOR_MATH
    USE_DX11
    USE_MSVC_SSE
    USE_MSVC_FAST_FLOATINGPOINT
    INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES
    INTERNAL_CREATE_MSVC_RELATIVE_PATH_PROJECTFILES
    INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES
    INTEL_OPENCL_INCLUDES
    INTEL_OPENCL_LIBRARIES
    INTEL_OPENCL_ICD_CFG
    NVIDIA_OPENCL_INCLUDES
    NVIDIA_OPENCL_LIBRARIES
    NVIDIA_OPENCL_ICD_CFG
    INTERNAL_UPDATE_SERIALIZATION_STRUCTURES
    BUILD_EXTRAS
    BUILD_MULTITHREADING
    AMD_OPENCL_BASE_DIR
    USE_MULTITHREADED_BENCHMARK

    BUILD_AMD_OPENCL_DEMOS
    BUILD_INTEL_OPENCL_DEMOS
    BUILD_NVIDIA_OPENCL_DEMOS
    BUILD_MINICL_OPENCL_DEMOS
    BUILD_CPU_DEMOS
    USE_GLUT
    BUILD_DEMOS
    INSTALL_LIBS
    INSTALL_EXTRA_LIBS
    BUILD_UNIT_TESTS

    WINDRES WINE
    
    # Additions since the upgrade to bullet 3 repo
    EXACT_PYTHON_VERSION
    PYTHON_VERSION_PYBULLET
    _FIND_LIB_PYTHON_PY
    USE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD
    BULLET2_USE_THREAD_LOCKS
    BULLET2_USE_OPEN_MP_MULTITHREADING
    BULLET2_USE_TBB_MULTITHREADING
    BULLET2_USE_PPL_MULTITHREADING
    # Explicitly Disabled
    USE_GRAPHICAL_BENCHMARK
    BUILD_BULLET2_DEMOS
    BUILD_BULLET3
    BUILD_CLSOCKET
    BUILD_ENET
    BUILD_OPENGL3_DEMOS
    BUILD_PYBULLET
    BUILD_SHARED_LIBS
    CLSOCKET_DEP_ONLY
    CLSOCKET_SHARED
)

message ("${PROJECT_NAME} - End")

