set (BUILD_SHARED_LIBS ON)

include_directories (src/
  ../CommonJNI/src
  ${JNI_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  )
  
CUDA_ADD_LIBRARY(JCudaDriver-${CMAKE_HOST}-${CMAKE_ARCH}
  src/JCudaDriver.cpp
)

TARGET_LINK_LIBRARIES(JCudaDriver-${CMAKE_HOST}-${CMAKE_ARCH}
  CommonJNI
)
