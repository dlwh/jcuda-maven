set (BUILD_SHARED_LIBS ON)

include_directories (src/
  ../CommonJNI/src
  ${JNI_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  )
  
CUDA_ADD_LIBRARY(JCudaRuntime-${CMAKE_HOST}-${CMAKE_ARCH}
  src/JCudaRuntime.cpp
)

TARGET_LINK_LIBRARIES(JCudaRuntime-${CMAKE_HOST}-${CMAKE_ARCH}
  CommonJNI
)
