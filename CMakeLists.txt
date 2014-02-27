cmake_minimum_required(VERSION 2.8)

project(JCuda-All-0.5.5)

set(CMAKE_MODULE_PATH 
  "${CMAKE_SOURCE_DIR}/CMake"
  ${CMAKE_MODULE_PATH})

find_package(CUDA 4.0 REQUIRED)
find_package(JNI REQUIRED)

if(CMAKE_HOST_WIN32)
  set(CMAKE_HOST "windows")
elseif(CMAKE_HOST_APPLE)
  set(CMAKE_HOST "apple")
  set(CMAKE_SKIP_RPATH FALSE)
elseif(CMAKE_HOST_UNIX)
  set(CMAKE_HOST "linux")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CMAKE_ARCH "x86_64")
else()
  set(CMAKE_ARCH "x86")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(CommonJNI)
add_subdirectory(JCudaDriverJNI)
add_subdirectory(JCudaRuntimeJNI)
add_subdirectory(JCublasJNI)
add_subdirectory(JCublas2JNI)
add_subdirectory(JCufftJNI)
add_subdirectory(JCurandJNI)
add_subdirectory(JCusparseJNI)
add_subdirectory(JCusparse2JNI)
