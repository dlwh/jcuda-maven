set (BUILD_SHARED_LIBS ON)

include_directories (src/
  ../CommonJNI/src
  ${JNI_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  )
  
CUDA_ADD_LIBRARY(JCusparse-${CMAKE_HOST}-${CMAKE_ARCH}
  src/JCusparse.cpp
)

CUDA_ADD_CUSPARSE_TO_TARGET(JCusparse-${CMAKE_HOST}-${CMAKE_ARCH})

TARGET_LINK_LIBRARIES(JCusparse-${CMAKE_HOST}-${CMAKE_ARCH}
  CommonJNI
)
