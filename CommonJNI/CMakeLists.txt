set (BUILD_SHARED_LIBS OFF)

include_directories (src/
  ${JNI_INCLUDE_DIRS})
  
ADD_LIBRARY(CommonJNI
  src/JNIUtils.cpp
  src/Logger.cpp
  src/PointerUtils.cpp
)
SET_TARGET_PROPERTIES(CommonJNI PROPERTIES COMPILE_FLAGS -fPIC)
