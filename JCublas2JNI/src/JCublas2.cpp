/*
 * JCublas - Java bindings for CUBLAS, the NVIDIA CUDA BLAS library,
 * to be used with JCuda
 *
 * Copyright (c) 2010-2012 Marco Hutter - http://www.jcuda.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "JCublas2.hpp"
#include "JCublas2_common.hpp"
#include <iostream>
#include <string>
#include <map>
#include <cuda_runtime_api.h>

jfieldID cuComplex_x; // float
jfieldID cuComplex_y; // float

jfieldID cuDoubleComplex_x; // double
jfieldID cuDoubleComplex_y; // double

/**
 * Creates a global reference to the class with the given name and
 * stores it in the given jclass argument, and stores the no-args
 * constructor ID for this class in the given jmethodID.
 * Returns whether this initialization succeeded.
 */
bool init(JNIEnv *env, const char *className, jclass &globalCls, jmethodID &constructor)
{
    jclass cls = NULL;
    if (!init(env, cls, className)) return false;
    if (!init(env, cls, constructor, "<init>", "()V")) return false;

    globalCls = (jclass)env->NewGlobalRef(cls);
    if (globalCls == NULL)
    {
        Logger::log(LOG_ERROR, "Failed to create reference to class %s\n", className);
        return false;
    }
    return true;
}

/**
 * Called when the library is loaded. Will initialize all
 * required field and method IDs
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_4))
    {
        return JNI_ERR;
    }

    Logger::log(LOG_TRACE, "Initializing JCublas2\n");

    jclass cls = NULL;

    // Initialize the JNIUtils and PointerUtils
    if (initJNIUtils(env) == JNI_ERR) return JNI_ERR;
    if (initPointerUtils(env) == JNI_ERR) return JNI_ERR;


    // Obtain the fieldIDs for cuComplex#x and cuComplex#y
    if (!init(env, cls, "jcuda/cuComplex")) return JNI_ERR;
    if (!init(env, cls, cuComplex_x, "x", "F")) return JNI_ERR;
    if (!init(env, cls, cuComplex_y, "y", "F")) return JNI_ERR;


    // Obtain the fieldIDs for cuDoubleComplex#x and cuDoubleComplex#y
    if (!init(env, cls, "jcuda/cuDoubleComplex")) return JNI_ERR;
    if (!init(env, cls, cuDoubleComplex_x, "x", "D")) return JNI_ERR;
    if (!init(env, cls, cuDoubleComplex_y, "y", "D")) return JNI_ERR;

    return JNI_VERSION_1_4;

}









//============================================================================
// Memory management functions


/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasSetVectorNative
 * Signature: (IILjcuda/Pointer;IILjcuda/Pointer;II)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetVectorNative
  (JNIEnv *env, jclass cla, jint n, jint elemSize, jobject x, jint incx, jobject y, jint incy)
{
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSetVector");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSetVector");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;

    PointerData *xPointerData = initPointerData(env, x);
    if (xPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    deviceMemory = getPointer(env, y);

    Logger::log(LOG_TRACE, "Setting %d elements of size %d from java with inc %d to '%s' with inc %d\n",
        n, elemSize, incx, "y", incy);

    cublasStatus_t result = cublasSetVector(n, elemSize, (void*)xPointerData->getPointer(env), incx, deviceMemory, incy);

    if (!releasePointerData(env, xPointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}




/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasGetVectorNative
 * Signature: (IILjcuda/Pointer;IILjcuda/Pointer;II)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetVectorNative
  (JNIEnv *env, jclass cla, jint n, jint elemSize, jobject x, jint incx, jobject y, jint incy)
{
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasGetVector");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasGetVector");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;

    deviceMemory = getPointer(env, x);
    PointerData *yPointerData = initPointerData(env, y);
    if (yPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Getting %d elements of size %d from '%s' with inc %d to java with inc %d\n",
        n, elemSize, "x", incx, incy);

    cublasStatus_t result = cublasGetVector(n, elemSize, deviceMemory, incx, (void*)yPointerData->getPointer(env), incy);

    if (!releasePointerData(env, yPointerData)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}





/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasSetMatrixNative
 * Signature: (IIILjcuda/Pointer;IILjcuda/Pointer;II)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetMatrixNative
  (JNIEnv *env, jclass cla, jint rows, jint cols, jint elemSize, jobject A, jint lda, jobject B, jint ldb)
{
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSetMatrix");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSetMatrix");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;

    PointerData *APointerData = initPointerData(env, A);
    if (APointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    deviceMemory = getPointer(env, B);

    Logger::log(LOG_TRACE, "Setting %dx%d elements of size %d from java with lda %d to '%s' with ldb %d\n",
        rows, cols, elemSize, lda, "B", ldb);

    cublasStatus_t result = cublasSetMatrix(rows, cols, elemSize, (void*)APointerData->getPointer(env), lda, deviceMemory, ldb);

    if (!releasePointerData(env, APointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}


/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasGetMatrixNative
 * Signature: (IIILjcuda/Pointer;IILjcuda/Pointer;II)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetMatrixNative
  (JNIEnv *env, jclass cla, jint rows, jint cols, jint elemSize, jobject A, jint lda, jobject B, jint ldb)
{
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasGetMatrix");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasGetMatrix");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;

    deviceMemory = getPointer(env, A);
    PointerData *BPointerData = initPointerData(env, B);
    if (BPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Getting %dx%d elements of size %d from '%s' with lda %d to java with ldb %d\n",
        rows, cols, elemSize, "A", lda, ldb);

    cublasStatus_t result = cublasGetMatrix(rows, cols, elemSize, deviceMemory, lda, (void*)BPointerData->getPointer(env), ldb);

    if (!releasePointerData(env, BPointerData)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}









//============================================================================
// Asynchronous Memory management functions


/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasSetVectorAsyncNative
 * Signature: (IILjcuda/Pointer;IILjcuda/Pointer;IILjcuda/runtime/cudaStream_t;)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetVectorAsyncNative
  (JNIEnv *env, jclass cla, jint n, jint elemSize, jobject x, jint incx, jobject y, jint incy, jobject stream)
{
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSetVectorAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSetVectorAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;
    cudaStream_t nativeStream = NULL;

    PointerData *xPointerData = initPointerData(env, x);
    if (xPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    deviceMemory = getPointer(env, y);

    nativeStream = (cudaStream_t)getNativePointerValue(env, stream);

    Logger::log(LOG_TRACE, "Setting %d elements of size %d from java with inc %d to '%s' with inc %d\n",
        n, elemSize, incx, "y", incy);

    cublasStatus_t result = cublasSetVectorAsync(n, elemSize, (void*)xPointerData->getPointer(env), incx, deviceMemory, incy, nativeStream);

    if (!releasePointerData(env, xPointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}




/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasGetVectorAsyncNative
 * Signature: (IILjcuda/Pointer;IILjcuda/Pointer;IILjcuda/runtime/cudaStream_t;)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetVectorAsyncNative
  (JNIEnv *env, jclass cla, jint n, jint elemSize, jobject x, jint incx, jobject y, jint incy, jobject stream)
{
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasGetVectorAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasGetVectorAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;
    cudaStream_t nativeStream = NULL;

    deviceMemory = getPointer(env, x);
    PointerData *yPointerData = initPointerData(env, y);
    if (yPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    nativeStream = (cudaStream_t)getNativePointerValue(env, stream);

    Logger::log(LOG_TRACE, "Getting %d elements of size %d from '%s' with inc %d to java with inc %d\n",
        n, elemSize, "x", incx, incy);

    cublasStatus_t result = cublasGetVectorAsync(n, elemSize, deviceMemory, incx, (void*)yPointerData->getPointer(env), incy, nativeStream);

    if (!releasePointerData(env, yPointerData)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}





/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasSetMatrixAsyncNative
 * Signature: (IIILjcuda/Pointer;IILjcuda/Pointer;IILjcuda/runtime/cudaStream_t;)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetMatrixAsyncNative
  (JNIEnv *env, jclass cla, jint rows, jint cols, jint elemSize, jobject A, jint lda, jobject B, jint ldb, jobject stream)
{
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSetMatrixAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSetMatrixAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;
    cudaStream_t nativeStream = NULL;

    PointerData *APointerData = initPointerData(env, A);
    if (APointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    deviceMemory = getPointer(env, B);

    nativeStream = (cudaStream_t)getNativePointerValue(env, stream);

    Logger::log(LOG_TRACE, "Setting %dx%d elements of size %d from java with lda %d to '%s' with ldb %d\n",
        rows, cols, elemSize, lda, "B", ldb);

    cublasStatus_t result = cublasSetMatrixAsync(rows, cols, elemSize, (void*)APointerData->getPointer(env), lda, deviceMemory, ldb, nativeStream);

    if (!releasePointerData(env, APointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}


/*
 * Passes the call to Cublas
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    cublasGetMatrixAsyncNative
 * Signature: (IIILjcuda/Pointer;IILjcuda/Pointer;IILjcuda/runtime/cudaStream_t;)I
 */
JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetMatrixAsyncNative
  (JNIEnv *env, jclass cla, jint rows, jint cols, jint elemSize, jobject A, jint lda, jobject B, jint ldb, jobject stream)
{
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasGetMatrixAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasGetMatrixAsync");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    void *deviceMemory = NULL;
    void *hostMemory = NULL;
    cudaStream_t nativeStream = NULL;

    deviceMemory = getPointer(env, A);
    PointerData *BPointerData = initPointerData(env, B);
    if (BPointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    nativeStream = (cudaStream_t)getNativePointerValue(env, stream);

    Logger::log(LOG_TRACE, "Getting %dx%d elements of size %d from '%s' with lda %d to java with ldb %d\n",
        rows, cols, elemSize, "A", lda, ldb);

    cublasStatus_t result = cublasGetMatrixAsync(rows, cols, elemSize, deviceMemory, lda, (void*)BPointerData->getPointer(env), ldb, nativeStream);

    if (!releasePointerData(env, BPointerData)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    return result;
}






/*
 * Set the log level
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    setLogLevelNative
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jcuda_jcublas_JCublas2_setLogLevelNative
  (JNIEnv *env, jclass cla, jint logLevel)
{
    Logger::setLogLevel((LogLevel)logLevel);
}



/*
 * Prints the specified vector of single precision floating point elements
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    printVector
 * Signature: (ILjcuda/Pointer;)V
 */
JNIEXPORT void JNICALL Java_jcuda_jcublas_JCublas2_printVector
  (JNIEnv *env, jclass cla, jint n, jobject x)
{
    float *deviceMemory = (float*)getPointer(env, x);
    float *hostMemory = (float*)malloc(n * sizeof(float));
    cublasGetVector(n, 4, deviceMemory, 1, hostMemory, 1);
    LogLevel tempLogLevel = Logger::currentLogLevel;
    Logger::setLogLevel(LOG_INFO);
    for (int i=0; i<n; i++)
    {
        Logger::log(LOG_INFO, "%2.1f  ", hostMemory[i]);
    }
    Logger::log(LOG_INFO, "\n");
    Logger::setLogLevel(tempLogLevel);
    free(hostMemory);
}


/*
 * Prints the specified matrix of single precision floating point elements
 *
 * Class:     jcuda_jcublas_JCublas2
 * Method:    printMatrix
 * Signature: (ILjcuda/Pointer;I)V
 */
JNIEXPORT void JNICALL Java_jcuda_jcublas_JCublas2_printMatrix
  (JNIEnv *env, jclass cla, jint cols, jobject A, jint lda)
{
    float *deviceMemory = (float*)getPointer(env, A);
    float *hostMemory = (float*)malloc(cols * lda * sizeof(float));
    cublasGetMatrix(lda, cols, 4, deviceMemory, lda, hostMemory, lda);
    LogLevel tempLogLevel = Logger::currentLogLevel;
    Logger::setLogLevel(LOG_INFO);
    for (int r=0; r<lda; r++)
    {
        for (int c=0; c<cols; c++)
        {
            Logger::log(LOG_INFO, "%2.1f  ", hostMemory[c * lda + r]);
        }
        Logger::log(LOG_INFO, "\n");
    }
    Logger::log(LOG_INFO, "\n");
    Logger::setLogLevel(tempLogLevel);
    free(hostMemory);
}


/**
 * Set the pointer to be the array elements that correspond to the given java
 * array. Returns true iff the array elements could be obtained, and the java
 * array had the expectedSize.
 *
 * Note that the array elements must later be released by calling
 * env->ReleaseFloatArrayElements(array, arrayElements, JNI_ABORT);
 */
bool getArrayElements(JNIEnv *env, jfloatArray array, float* &arrayElements, int expectedSize)
{
    int size = env->GetArrayLength(array);
    if (size != expectedSize)
    {
        Logger::log(LOG_ERROR, "Expected an array size of %d, but it has a size of %d\n", expectedSize, size);
        return false;
    }
    arrayElements = env->GetFloatArrayElements(array, NULL);
    if (arrayElements == NULL)
    {
        Logger::log(LOG_ERROR, "Out of memory while obtaining array elements\n");
        return false;
    }
    return true;
}

/**
 * Set the pointer to be the array elements that correspond to the given java
 * array. Returns true iff the array elements could be obtained, and the java
 * array had the expectedSize.
 *
 * Note that the array elements must later be released by calling
 * env->ReleaseDoubleArrayElements(array, arrayElements, JNI_ABORT);
 */
bool getDoubleArrayElements(JNIEnv *env, jdoubleArray array, double* &arrayElements, int expectedSize)
{
    int size = env->GetArrayLength(array);
    if (size != expectedSize)
    {
        Logger::log(LOG_ERROR, "Expected an array size of %d, but it has a size of %d\n", expectedSize, size);
        return false;
    }
    arrayElements = env->GetDoubleArrayElements(array, NULL);
    if (arrayElements == NULL)
    {
        Logger::log(LOG_ERROR, "Out of memory while obtaining array elements\n");
        return false;
    }
    return true;
}





//============================================================================
// Auto-generated part:

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCreateNative(JNIEnv *env, jclass cls, jobject handle)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCreate");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCreate(handle=%p)\n",
        handle);

    // Native variable declarations
    cublasHandle_t handle_native;

    // Obtain native variable values
    // handle is initialized here

    // Native function call
    cublasStatus_t jniResult_native = cublasCreate(&handle_native);

    // Write back native variable values
    setNativePointerValue(env, handle, (jlong)handle_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDestroyNative(JNIEnv *env, jclass cls, jobject handle)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDestroy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDestroy(handle=%p)\n",
        handle);

    // Native variable declarations
    cublasHandle_t handle_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);

    // Native function call
    cublasStatus_t jniResult_native = cublasDestroy(handle_native);

    // Write back native variable values
    // handle is destroyed here

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetVersionNative(JNIEnv *env, jclass cls, jobject handle, jintArray version)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasGetVersion");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (version == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'version' is null for cublasGetVersion");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasGetVersion(handle=%p, version=%p)\n",
        handle, version);

    // Native variable declarations
    cublasHandle_t handle_native;
    int version_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    // version is set here

    // Native function call
    cublasStatus_t jniResult_native = cublasGetVersion(handle_native, &version_native);

    // Write back native variable values
    // handle is a native pointer object
    if (!set(env, version, 0, (jint)version_native)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetStreamNative(JNIEnv *env, jclass cls, jobject handle, jobject streamId)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSetStream");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // streamId may be NULL

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSetStream(handle=%p, streamId=%p)\n",
        handle, streamId);

    // Native variable declarations
    cublasHandle_t handle_native;
    cudaStream_t streamId_native = NULL;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    if (streamId != NULL)
    {
        streamId_native = (cudaStream_t)getNativePointerValue(env, streamId);
    }

    // Native function call
    cublasStatus_t jniResult_native = cublasSetStream(handle_native, streamId_native);

    // Write back native variable values
    // handle is a native pointer object
    // streamId is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetStreamNative(JNIEnv *env, jclass cls, jobject handle, jobject streamId)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasGetStream");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (streamId == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'streamId' is null for cublasGetStream");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasGetStream(handle=%p, streamId=%p)\n",
        handle, streamId);

    // Native variable declarations
    cublasHandle_t handle_native;
    cudaStream_t streamId_native = NULL;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    // streamId is initialized here

    // Native function call
    cublasStatus_t jniResult_native = cublasGetStream(handle_native, &streamId_native);

    // Write back native variable values
    // handle is a native pointer object
    setNativePointerValue(env, streamId, (jlong)streamId_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetPointerModeNative(JNIEnv *env, jclass cls, jobject handle, jintArray mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasGetPointerMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (mode == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'mode' is null for cublasGetPointerMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasGetPointerMode(handle=%p, mode=%p)\n",
        handle, mode);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasPointerMode_t mode_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    // mode is set here

    // Native function call
    cublasStatus_t jniResult_native = cublasGetPointerMode(handle_native, &mode_native);

    // Write back native variable values
    // handle is a native pointer object
    if (!set(env, mode, 0, (jint)mode_native)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetPointerModeNative(JNIEnv *env, jclass cls, jobject handle, jint mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSetPointerMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSetPointerMode(handle=%p, mode=%d)\n",
        handle, mode);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasPointerMode_t mode_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasPointerMode_t)mode;

    // Native function call
    cublasStatus_t jniResult_native = cublasSetPointerMode(handle_native, mode_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasGetAtomicsModeNative(JNIEnv *env, jclass cls, jobject handle, jintArray mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasGetAtomicsMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (mode == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'mode' is null for cublasGetAtomicsMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasGetAtomicsMode(handle=%p, mode=%p)\n",
        handle, mode);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasAtomicsMode_t* mode_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasAtomicsMode_t*)getPointer(env, mode);

    // Native function call
    cublasStatus_t jniResult_native = cublasGetAtomicsMode(handle_native, mode_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSetAtomicsModeNative(JNIEnv *env, jclass cls, jobject handle, jint mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSetAtomicsMode");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSetAtomicsMode(handle=%p, mode=%d)\n",
        handle, mode);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasAtomicsMode_t mode_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasAtomicsMode_t)mode;

    // Native function call
    cublasStatus_t jniResult_native = cublasSetAtomicsMode(handle_native, mode_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}


JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSnrm2Native(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasSnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSnrm2(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (float*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSnrm2(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDnrm2Native(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasDnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDnrm2(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (double*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDnrm2(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasScnrm2Native(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasScnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasScnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasScnrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasScnrm2(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    float* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (float*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasScnrm2(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDznrm2Native(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDznrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDznrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasDznrm2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDznrm2(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    double* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (double*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDznrm2(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSdotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasSdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSdot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (float*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSdot(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDdotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasDdot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDdot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (double*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDdot(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCdotuNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasCdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCdotu(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (cuComplex*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasCdotu(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCdotcNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasCdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCdotc(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (cuComplex*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasCdotc(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZdotuNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasZdotu");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZdotu(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (cuDoubleComplex*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasZdotu(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZdotcNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasZdotc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZdotc(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, result=%p)\n",
        handle, n, x, incx, y, incy, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (cuDoubleComplex*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasZdotc(handle_native, n_native, x_native, incx_native, y_native, incy_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasSscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCsscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZdscalNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZdscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZdscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZdscal");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZdscal(handle=%p, n=%d, alpha=%p, x=%p, incx=%d)\n",
        handle, n, alpha, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZdscal(handle_native, n_native, alpha_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSaxpyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSaxpy(handle=%p, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, alpha, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSaxpy(handle_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDaxpyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDaxpy(handle=%p, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, alpha, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDaxpy(handle_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCaxpyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCaxpy(handle=%p, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, alpha, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCaxpy(handle_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZaxpyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZaxpy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZaxpy(handle=%p, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, alpha, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZaxpy(handle_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasScopyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasScopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasScopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasScopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasScopy(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasScopy(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDcopyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDcopy(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDcopy(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCcopyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCcopy(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCcopy(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZcopyNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZcopy");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZcopy(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZcopy(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSswapNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSswap(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSswap(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDswapNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDswap(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDswap(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCswapNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCswap(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCswap(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZswapNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZswap");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZswap(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d)\n",
        handle, n, x, incx, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZswap(handle_native, n_native, x_native, incx_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIsamaxNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIsamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIsamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIsamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIsamax(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIsamax(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIdamaxNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIdamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIdamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIdamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIdamax(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIdamax(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIcamaxNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIcamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIcamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIcamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIcamax(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIcamax(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIzamaxNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIzamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIzamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIzamax");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIzamax(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIzamax(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIsaminNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIsamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIsamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIsamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIsamin(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIsamin(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIdaminNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIdamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIdamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIdamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIdamin(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIdamin(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIcaminNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIcamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIcamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIcamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIcamin(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIcamin(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasIzaminNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasIzamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasIzamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasIzamin");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasIzamin(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    int* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (int*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasIzamin(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSasumNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasSasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSasum(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (float*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSasum(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDasumNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasDasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDasum(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (double*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDasum(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasScasumNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasScasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasScasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasScasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasScasum(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    float* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (float*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasScasum(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDzasumNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject result)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDzasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDzasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'result' is null for cublasDzasum");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDzasum(handle=%p, n=%d, x=%p, incx=%d, result=%p)\n",
        handle, n, x, incx, result);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    double* result_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *result_pointerData = initPointerData(env, result);
    if (result_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    result_native = (double*)result_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDzasum(handle_native, n_native, x_native, incx_native, result_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, result))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, result_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasSrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasSrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* c_native;
    float* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (float*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (float*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasDrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasDrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* c_native;
    double* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (double*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (double*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasCrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasCrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    float* c_native;
    cuComplex* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (float*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (cuComplex*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasCrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCsrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCsrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasCsrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasCsrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    float* c_native;
    float* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (float*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (float*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasCsrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasZrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasZrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    double* c_native;
    cuDoubleComplex* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (double*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (cuDoubleComplex*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasZrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZdrotNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZdrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZdrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZdrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasZdrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasZdrot");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZdrot(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, c=%p, s=%p)\n",
        handle, n, x, incx, y, incy, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    double* c_native;
    double* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (double*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (double*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasZdrot(handle_native, n_native, x_native, incx_native, y_native, incy_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSrotgNative(JNIEnv *env, jclass cls, jobject handle, jobject a, jobject b, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (a == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'a' is null for cublasSrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (b == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'b' is null for cublasSrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasSrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasSrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSrotg(handle=%p, a=%p, b=%p, c=%p, s=%p)\n",
        handle, a, b, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    float* a_native;
    float* b_native;
    float* c_native;
    float* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *a_pointerData = initPointerData(env, a);
    if (a_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    a_native = (float*)a_pointerData->getPointer(env);
    PointerData *b_pointerData = initPointerData(env, b);
    if (b_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    b_native = (float*)b_pointerData->getPointer(env);
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (float*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (float*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSrotg(handle_native, a_native, b_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, a))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, a_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, b))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, b_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDrotgNative(JNIEnv *env, jclass cls, jobject handle, jobject a, jobject b, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (a == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'a' is null for cublasDrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (b == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'b' is null for cublasDrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasDrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasDrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDrotg(handle=%p, a=%p, b=%p, c=%p, s=%p)\n",
        handle, a, b, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    double* a_native;
    double* b_native;
    double* c_native;
    double* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *a_pointerData = initPointerData(env, a);
    if (a_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    a_native = (double*)a_pointerData->getPointer(env);
    PointerData *b_pointerData = initPointerData(env, b);
    if (b_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    b_native = (double*)b_pointerData->getPointer(env);
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (double*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (double*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDrotg(handle_native, a_native, b_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, a))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, a_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, b))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, b_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCrotgNative(JNIEnv *env, jclass cls, jobject handle, jobject a, jobject b, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (a == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'a' is null for cublasCrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (b == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'b' is null for cublasCrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasCrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasCrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCrotg(handle=%p, a=%p, b=%p, c=%p, s=%p)\n",
        handle, a, b, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    cuComplex* a_native;
    cuComplex* b_native;
    float* c_native;
    cuComplex* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *a_pointerData = initPointerData(env, a);
    if (a_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    a_native = (cuComplex*)a_pointerData->getPointer(env);
    PointerData *b_pointerData = initPointerData(env, b);
    if (b_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    b_native = (cuComplex*)b_pointerData->getPointer(env);
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (float*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (cuComplex*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasCrotg(handle_native, a_native, b_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, a))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, a_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, b))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, b_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZrotgNative(JNIEnv *env, jclass cls, jobject handle, jobject a, jobject b, jobject c, jobject s)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (a == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'a' is null for cublasZrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (b == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'b' is null for cublasZrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cublasZrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cublasZrotg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZrotg(handle=%p, a=%p, b=%p, c=%p, s=%p)\n",
        handle, a, b, c, s);

    // Native variable declarations
    cublasHandle_t handle_native;
    cuDoubleComplex* a_native;
    cuDoubleComplex* b_native;
    double* c_native;
    cuDoubleComplex* s_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *a_pointerData = initPointerData(env, a);
    if (a_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    a_native = (cuDoubleComplex*)a_pointerData->getPointer(env);
    PointerData *b_pointerData = initPointerData(env, b);
    if (b_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    b_native = (cuDoubleComplex*)b_pointerData->getPointer(env);
    PointerData *c_pointerData = initPointerData(env, c);
    if (c_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    c_native = (double*)c_pointerData->getPointer(env);
    PointerData *s_pointerData = initPointerData(env, s);
    if (s_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    s_native = (cuDoubleComplex*)s_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasZrotg(handle_native, a_native, b_native, c_native, s_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, a))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, a_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, b))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, b_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, c))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, c_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, s))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, s_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSrotmNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject param)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (param == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'param' is null for cublasSrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSrotm(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, param=%p)\n",
        handle, n, x, incx, y, incy, param);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* param_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *param_pointerData = initPointerData(env, param);
    if (param_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    param_native = (float*)param_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSrotm(handle_native, n_native, x_native, incx_native, y_native, incy_native, param_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, param))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, param_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDrotmNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject x, jint incx, jobject y, jint incy, jobject param)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (param == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'param' is null for cublasDrotm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDrotm(handle=%p, n=%d, x=%p, incx=%d, y=%p, incy=%d, param=%p)\n",
        handle, n, x, incx, y, incy, param);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* param_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    PointerData *param_pointerData = initPointerData(env, param);
    if (param_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    param_native = (double*)param_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDrotm(handle_native, n_native, x_native, incx_native, y_native, incy_native, param_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, param))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, param_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSrotmgNative(JNIEnv *env, jclass cls, jobject handle, jobject d1, jobject d2, jobject x1, jobject y1, jobject param)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (d1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd1' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (d2 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd2' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x1' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y1' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (param == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'param' is null for cublasSrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSrotmg(handle=%p, d1=%p, d2=%p, x1=%p, y1=%p, param=%p)\n",
        handle, d1, d2, x1, y1, param);

    // Native variable declarations
    cublasHandle_t handle_native;
    float* d1_native;
    float* d2_native;
    float* x1_native;
    float* y1_native;
    float* param_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *d1_pointerData = initPointerData(env, d1);
    if (d1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    d1_native = (float*)d1_pointerData->getPointer(env);
    PointerData *d2_pointerData = initPointerData(env, d2);
    if (d2_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    d2_native = (float*)d2_pointerData->getPointer(env);
    PointerData *x1_pointerData = initPointerData(env, x1);
    if (x1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    x1_native = (float*)x1_pointerData->getPointer(env);
    PointerData *y1_pointerData = initPointerData(env, y1);
    if (y1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    y1_native = (float*)y1_pointerData->getPointer(env);
    PointerData *param_pointerData = initPointerData(env, param);
    if (param_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    param_native = (float*)param_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasSrotmg(handle_native, d1_native, d2_native, x1_native, y1_native, param_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, d1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, d1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, d2))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, d2_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, x1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, x1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, y1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, y1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, param))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, param_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDrotmgNative(JNIEnv *env, jclass cls, jobject handle, jobject d1, jobject d2, jobject x1, jobject y1, jobject param)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (d1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd1' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (d2 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd2' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x1' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y1 == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y1' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (param == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'param' is null for cublasDrotmg");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDrotmg(handle=%p, d1=%p, d2=%p, x1=%p, y1=%p, param=%p)\n",
        handle, d1, d2, x1, y1, param);

    // Native variable declarations
    cublasHandle_t handle_native;
    double* d1_native;
    double* d2_native;
    double* x1_native;
    double* y1_native;
    double* param_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    PointerData *d1_pointerData = initPointerData(env, d1);
    if (d1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    d1_native = (double*)d1_pointerData->getPointer(env);
    PointerData *d2_pointerData = initPointerData(env, d2);
    if (d2_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    d2_native = (double*)d2_pointerData->getPointer(env);
    PointerData *x1_pointerData = initPointerData(env, x1);
    if (x1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    x1_native = (double*)x1_pointerData->getPointer(env);
    PointerData *y1_pointerData = initPointerData(env, y1);
    if (y1_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    y1_native = (double*)y1_pointerData->getPointer(env);
    PointerData *param_pointerData = initPointerData(env, param);
    if (param_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    param_native = (double*)param_pointerData->getPointer(env);

    // Native function call
    cublasStatus_t jniResult_native = cublasDrotmg(handle_native, d1_native, d2_native, x1_native, y1_native, param_native);

    // Write back native variable values
    // handle is a native pointer object
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, d1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, d1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, d2))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, d2_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, x1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, x1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, y1))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, y1_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // If the PointerData is not a backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, param))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, param_pointerData, 0)) return JCUBLAS_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgemvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgemv(handle=%p, trans=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;
    float* beta_native;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgemv(handle_native, trans_native, m_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgemvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgemv(handle=%p, trans=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;
    double* beta_native;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgemv(handle_native, trans_native, m_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgemvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgemv(handle=%p, trans=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgemv(handle_native, trans_native, m_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgemvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZgemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgemv(handle=%p, trans=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgemv(handle_native, trans_native, m_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgbmvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jint kl, jint ku, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgbmv(handle=%p, trans=%d, m=%d, n=%d, kl=%d, ku=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, kl, ku, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    int kl_native = 0;
    int ku_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;
    float* beta_native;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    kl_native = (int)kl;
    ku_native = (int)ku;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgbmv(handle_native, trans_native, m_native, n_native, kl_native, ku_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgbmvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jint kl, jint ku, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgbmv(handle=%p, trans=%d, m=%d, n=%d, kl=%d, ku=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, kl, ku, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    int kl_native = 0;
    int ku_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;
    double* beta_native;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    kl_native = (int)kl;
    ku_native = (int)ku;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgbmv(handle_native, trans_native, m_native, n_native, kl_native, ku_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgbmvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jint kl, jint ku, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgbmv(handle=%p, trans=%d, m=%d, n=%d, kl=%d, ku=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, kl, ku, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    int kl_native = 0;
    int ku_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    kl_native = (int)kl;
    ku_native = (int)ku;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgbmv(handle_native, trans_native, m_native, n_native, kl_native, ku_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgbmvNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jint n, jint kl, jint ku, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZgbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgbmv(handle=%p, trans=%d, m=%d, n=%d, kl=%d, ku=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, trans, m, n, kl, ku, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t trans_native;
    int m_native = 0;
    int n_native = 0;
    int kl_native = 0;
    int ku_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    trans_native = (cublasOperation_t)trans;
    m_native = (int)m;
    n_native = (int)n;
    kl_native = (int)kl;
    ku_native = (int)ku;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgbmv(handle_native, trans_native, m_native, n_native, kl_native, ku_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // trans is primitive
    // m is primitive
    // n is primitive
    // kl is primitive
    // ku is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStrmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStrmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStrmv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtrmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtrmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtrmv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtrmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtrmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtrmv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtrmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtrmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtrmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtrmv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStbmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStbmv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtbmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtbmv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtbmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtbmv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtbmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtbmv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasStpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStpmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    float* AP_native;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (float*)getPointer(env, AP);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStpmv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasDtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtpmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    double* AP_native;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (double*)getPointer(env, AP);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtpmv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasCtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtpmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuComplex* AP_native;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (cuComplex*)getPointer(env, AP);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtpmv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasZtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtpmv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuDoubleComplex* AP_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (cuDoubleComplex*)getPointer(env, AP);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtpmv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStrsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStrsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStrsv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtrsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtrsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtrsv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtrsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtrsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtrsv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtrsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtrsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtrsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtrsv(handle_native, uplo_native, trans_native, diag_native, n_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStpsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasStpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStpsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    float* AP_native;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (float*)getPointer(env, AP);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStpsv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtpsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasDtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtpsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    double* AP_native;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (double*)getPointer(env, AP);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtpsv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtpsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasCtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtpsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuComplex* AP_native;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (cuComplex*)getPointer(env, AP);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtpsv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtpsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jobject AP, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasZtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtpsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtpsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, AP=%p, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, AP, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    cuDoubleComplex* AP_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    AP_native = (cuDoubleComplex*)getPointer(env, AP);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtpsv(handle_native, uplo_native, trans_native, diag_native, n_native, AP_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStbsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasStbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStbsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasStbsv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtbsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtbsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtbsv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtbsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtbsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtbsv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtbsvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint diag, jint n, jint k, jobject A, jint lda, jobject x, jint incx)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZtbsv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtbsv(handle=%p, uplo=%d, trans=%d, diag=%d, n=%d, k=%d, A=%p, lda=%d, x=%p, incx=%d)\n",
        handle, uplo, trans, diag, n, k, A, lda, x, incx);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    n_native = (int)n;
    k_native = (int)k;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtbsv(handle_native, uplo_native, trans_native, diag_native, n_native, k_native, A_native, lda_native, x_native, incx_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // n is primitive
    // k is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsymvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsymv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;
    float* beta_native;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsymv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsymvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsymv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;
    double* beta_native;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsymv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsymvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsymv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsymv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsymvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZsymv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsymv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsymv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChemvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasChemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChemv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasChemv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhemvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZhemv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhemv(handle=%p, uplo=%d, n=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZhemv(handle_native, uplo_native, n_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jint k, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsbmv(handle=%p, uplo=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, k, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;
    float* beta_native;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsbmv(handle_native, uplo_native, n_native, k_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jint k, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDsbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsbmv(handle=%p, uplo=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, k, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;
    double* beta_native;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsbmv(handle_native, uplo_native, n_native, k_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jint k, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasChbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChbmv(handle=%p, uplo=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, k, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasChbmv(handle_native, uplo_native, n_native, k_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhbmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jint k, jobject alpha, jobject A, jint lda, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZhbmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhbmv(handle=%p, uplo=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, k, alpha, A, lda, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZhbmv(handle_native, uplo_native, n_native, k_native, alpha_native, A_native, lda_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSspmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject AP, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSspmv(handle=%p, uplo=%d, n=%d, alpha=%p, AP=%p, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, AP, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* AP_native;
    float* x_native;
    int incx_native = 0;
    float* beta_native;
    float* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    AP_native = (float*)getPointer(env, AP);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasSspmv(handle_native, uplo_native, n_native, alpha_native, AP_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDspmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject AP, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDspmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDspmv(handle=%p, uplo=%d, n=%d, alpha=%p, AP=%p, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, AP, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* AP_native;
    double* x_native;
    int incx_native = 0;
    double* beta_native;
    double* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    AP_native = (double*)getPointer(env, AP);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasDspmv(handle_native, uplo_native, n_native, alpha_native, AP_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject AP, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasChpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChpmv(handle=%p, uplo=%d, n=%d, alpha=%p, AP=%p, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, AP, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* AP_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* beta_native;
    cuComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    AP_native = (cuComplex*)getPointer(env, AP);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasChpmv(handle_native, uplo_native, n_native, alpha_native, AP_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhpmvNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject AP, jobject x, jint incx, jobject beta, jobject y, jint incy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZhpmv");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhpmv(handle=%p, uplo=%d, n=%d, alpha=%p, AP=%p, x=%p, incx=%d, beta=%p, y=%p, incy=%d)\n",
        handle, uplo, n, alpha, AP, x, incx, beta, y, incy);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* AP_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;
    int incy_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    AP_native = (cuDoubleComplex*)getPointer(env, AP);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;

    // Native function call
    cublasStatus_t jniResult_native = cublasZhpmv(handle_native, uplo_native, n_native, alpha_native, AP_native, x_native, incx_native, beta_native, y_native, incy_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // AP is a native pointer
    // x is a native pointer
    // incx is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // y is a native pointer
    // incy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgerNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSger(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasSger(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgerNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDger");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDger(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasDger(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgeruNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgeru(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgeru(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgercNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgerc(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgerc(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgeruNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgeru");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgeru(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgeru(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgercNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgerc");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgerc(handle=%p, m=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, m, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgerc(handle_native, m_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsyrNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsyr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsyr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsyrNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsyr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsyr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsyrNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsyr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsyr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsyrNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsyr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsyr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsyr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCherNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCher(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCher(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZherNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZher");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZher(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZher(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsprNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasSspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSspr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    AP_native = (float*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasSspr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsprNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasDspr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDspr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    AP_native = (double*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasDspr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChprNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasChpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasChpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChpr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    AP_native = (cuComplex*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasChpr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhprNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZhpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasZhpr");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhpr(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    AP_native = (cuDoubleComplex*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasZhpr(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsyr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsyr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsyr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsyr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsyr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsyr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsyr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsyr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsyr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsyr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsyr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsyr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsyr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCher2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasCher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCher2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasCher2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZher2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZher2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZher2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, A=%p, lda=%d)\n",
        handle, uplo, n, alpha, x, incx, y, incy, A, lda);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cublasStatus_t jniResult_native = cublasZher2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, A_native, lda_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSspr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasSspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasSspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSspr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, y, incy, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    float* alpha_native;
    float* x_native;
    int incx_native = 0;
    float* y_native;
    int incy_native = 0;
    float* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (float*)getPointer(env, y);
    incy_native = (int)incy;
    AP_native = (float*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasSspr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDspr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasDspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasDspr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDspr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, y, incy, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    double* alpha_native;
    double* x_native;
    int incx_native = 0;
    double* y_native;
    int incy_native = 0;
    double* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (double*)getPointer(env, y);
    incy_native = (int)incy;
    AP_native = (double*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasDspr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChpr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasChpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasChpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasChpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChpr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, y, incy, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* y_native;
    int incy_native = 0;
    cuComplex* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuComplex*)getPointer(env, y);
    incy_native = (int)incy;
    AP_native = (cuComplex*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasChpr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhpr2Native(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint n, jobject alpha, jobject x, jint incx, jobject y, jint incy, jobject AP)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZhpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cublasZhpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incy is primitive
    if (AP == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'AP' is null for cublasZhpr2");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhpr2(handle=%p, uplo=%d, n=%d, alpha=%p, x=%p, incx=%d, y=%p, incy=%d, AP=%p)\n",
        handle, uplo, n, alpha, x, incx, y, incy, AP);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* y_native;
    int incy_native = 0;
    cuDoubleComplex* AP_native;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    incy_native = (int)incy;
    AP_native = (cuDoubleComplex*)getPointer(env, AP);

    // Native function call
    cublasStatus_t jniResult_native = cublasZhpr2(handle_native, uplo_native, n_native, alpha_native, x_native, incx_native, y_native, incy_native, AP_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // x is a native pointer
    // incx is primitive
    // y is a native pointer
    // incy is primitive
    // AP is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgemm(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgemm(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgemm(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgemm(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgemm(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgemm(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZgemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgemm(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgemm(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsyrkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsyrk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsyrk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsyrkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsyrk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsyrk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsyrkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsyrk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsyrk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsyrkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZsyrk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsyrk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsyrk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCherkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCherk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    float* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCherk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZherkNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZherk");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZherk(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    double* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZherk(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsyr2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsyr2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsyr2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsyr2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsyr2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsyr2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsyr2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsyr2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsyr2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsyr2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZsyr2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsyr2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsyr2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCher2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCher2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    float* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCher2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZher2kNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZher2k");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZher2k(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    double* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZher2k(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsyrkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsyrkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsyrkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsyrkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsyrkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsyrkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsyrkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsyrkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsyrkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsyrkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZsyrkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsyrkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsyrkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCherkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCherkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    float* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCherkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZherkxNative(JNIEnv *env, jclass cls, jobject handle, jint uplo, jint trans, jint n, jint k, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZherkx");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZherkx(handle=%p, uplo=%d, trans=%d, n=%d, k=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, uplo, trans, n, k, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    double* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZherkx(handle_native, uplo_native, trans_native, n_native, k_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // uplo is primitive
    // trans is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSsymmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSsymm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSsymm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDsymmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDsymm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDsymm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCsymmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCsymm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCsymm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZsymmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZsymm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZsymm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZsymm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasChemmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasChemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasChemm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasChemm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZhemmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZhemm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZhemm(handle=%p, side=%d, uplo=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, side, uplo, m, n, alpha, A, lda, B, ldb, beta, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZhemm(handle_native, side_native, uplo_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStrsmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasStrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasStrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStrsm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cublasStatus_t jniResult_native = cublasStrsm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtrsmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtrsm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtrsm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtrsmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtrsm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtrsm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtrsmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZtrsm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtrsm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtrsm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStrmmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasStrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasStrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasStrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStrmm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* B_native;
    int ldb_native = 0;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasStrmm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtrmmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtrmm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* B_native;
    int ldb_native = 0;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtrmm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtrmmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtrmm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtrmm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtrmmNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZtrmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtrmm(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtrmm(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgemmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject Aarray, jint lda, jobject Barray, jint ldb, jobject beta, jobject Carray, jint ldc, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Aarray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Aarray' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Barray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Barray' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Carray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Carray' is null for cublasSgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgemmBatched(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, Aarray=%p, lda=%d, Barray=%p, ldb=%d, beta=%p, Carray=%p, ldc=%d, batchCount=%d)\n",
        handle, transa, transb, m, n, k, alpha, Aarray, lda, Barray, ldb, beta, Carray, ldc, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    float* alpha_native;
    const float** Aarray_native = NULL;
    int lda_native = 0;
    const float** Barray_native = NULL;
    int ldb_native = 0;
    float* beta_native;
    float** Carray_native = NULL;
    int ldc_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    Aarray_native = (const float**)getPointer(env, Aarray);
    lda_native = (int)lda;
    Barray_native = (const float**)getPointer(env, Barray);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    Carray_native = (float**)getPointer(env, Carray);
    ldc_native = (int)ldc;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgemmBatched(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, Aarray_native, lda_native, Barray_native, ldb_native, beta_native, Carray_native, ldc_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Aarray is a native pointer
    // lda is primitive
    // Barray is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Carray is a native pointer
    // ldc is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgemmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject Aarray, jint lda, jobject Barray, jint ldb, jobject beta, jobject Carray, jint ldc, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Aarray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Aarray' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Barray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Barray' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Carray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Carray' is null for cublasDgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgemmBatched(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, Aarray=%p, lda=%d, Barray=%p, ldb=%d, beta=%p, Carray=%p, ldc=%d, batchCount=%d)\n",
        handle, transa, transb, m, n, k, alpha, Aarray, lda, Barray, ldb, beta, Carray, ldc, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    double* alpha_native;
    const double** Aarray_native = NULL;
    int lda_native = 0;
    const double** Barray_native = NULL;
    int ldb_native = 0;
    double* beta_native;
    double** Carray_native = NULL;
    int ldc_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    Aarray_native = (const double**)getPointer(env, Aarray);
    lda_native = (int)lda;
    Barray_native = (const double**)getPointer(env, Barray);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    Carray_native = (double**)getPointer(env, Carray);
    ldc_native = (int)ldc;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgemmBatched(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, Aarray_native, lda_native, Barray_native, ldb_native, beta_native, Carray_native, ldc_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Aarray is a native pointer
    // lda is primitive
    // Barray is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Carray is a native pointer
    // ldc is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgemmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject Aarray, jint lda, jobject Barray, jint ldb, jobject beta, jobject Carray, jint ldc, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Aarray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Aarray' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Barray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Barray' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Carray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Carray' is null for cublasCgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgemmBatched(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, Aarray=%p, lda=%d, Barray=%p, ldb=%d, beta=%p, Carray=%p, ldc=%d, batchCount=%d)\n",
        handle, transa, transb, m, n, k, alpha, Aarray, lda, Barray, ldb, beta, Carray, ldc, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuComplex* alpha_native;
    const cuComplex** Aarray_native = NULL;
    int lda_native = 0;
    const cuComplex** Barray_native = NULL;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex** Carray_native = NULL;
    int ldc_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    Aarray_native = (const cuComplex**)getPointer(env, Aarray);
    lda_native = (int)lda;
    Barray_native = (const cuComplex**)getPointer(env, Barray);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    Carray_native = (cuComplex**)getPointer(env, Carray);
    ldc_native = (int)ldc;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgemmBatched(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, Aarray_native, lda_native, Barray_native, ldb_native, beta_native, Carray_native, ldc_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Aarray is a native pointer
    // lda is primitive
    // Barray is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Carray is a native pointer
    // ldc is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgemmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jobject alpha, jobject Aarray, jint lda, jobject Barray, jint ldb, jobject beta, jobject Carray, jint ldc, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Aarray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Aarray' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Barray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Barray' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (Carray == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Carray' is null for cublasZgemmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgemmBatched(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, alpha=%p, Aarray=%p, lda=%d, Barray=%p, ldb=%d, beta=%p, Carray=%p, ldc=%d, batchCount=%d)\n",
        handle, transa, transb, m, n, k, alpha, Aarray, lda, Barray, ldb, beta, Carray, ldc, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex* alpha_native;
    const cuDoubleComplex** Aarray_native = NULL;
    int lda_native = 0;
    const cuDoubleComplex** Barray_native = NULL;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex** Carray_native = NULL;
    int ldc_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    Aarray_native = (const cuDoubleComplex**)getPointer(env, Aarray);
    lda_native = (int)lda;
    Barray_native = (const cuDoubleComplex**)getPointer(env, Barray);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    Carray_native = (cuDoubleComplex**)getPointer(env, Carray);
    ldc_native = (int)ldc;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgemmBatched(handle_native, transa_native, transb_native, m_native, n_native, k_native, alpha_native, Aarray_native, lda_native, Barray_native, ldb_native, beta_native, Carray_native, ldc_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Aarray is a native pointer
    // lda is primitive
    // Barray is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // Carray is a native pointer
    // ldc is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgeamNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jobject alpha, jobject A, jint lda, jobject beta, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgeam(handle=%p, transa=%d, transb=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, beta=%p, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float* A_native;
    int lda_native = 0;
    float* beta_native;
    float* B_native;
    int ldb_native = 0;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgeam(handle_native, transa_native, transb_native, m_native, n_native, alpha_native, A_native, lda_native, beta_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgeamNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jobject alpha, jobject A, jint lda, jobject beta, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgeam(handle=%p, transa=%d, transb=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, beta=%p, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double* A_native;
    int lda_native = 0;
    double* beta_native;
    double* B_native;
    int ldb_native = 0;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgeam(handle_native, transa_native, transb_native, m_native, n_native, alpha_native, A_native, lda_native, beta_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgeamNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jobject alpha, jobject A, jint lda, jobject beta, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgeam(handle=%p, transa=%d, transb=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, beta=%p, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* beta_native;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgeam(handle_native, transa_native, transb_native, m_native, n_native, alpha_native, A_native, lda_native, beta_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgeamNative(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jobject alpha, jobject A, jint lda, jobject beta, jobject B, jint ldb, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZgeam");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgeam(handle=%p, transa=%d, transb=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, beta=%p, B=%p, ldb=%d, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, alpha, A, lda, beta, B, ldb, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasOperation_t transa_native;
    cublasOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    transa_native = (cublasOperation_t)transa;
    transb_native = (cublasOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgeam(handle_native, transa_native, transb_native, m_native, n_native, alpha_native, A_native, lda_native, beta_native, B_native, ldb_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // B is a native pointer
    // ldb is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgetrfBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasSgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasSgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgetrfBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (float**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgetrfBatched(handle_native, n_native, A_native, lda_native, P_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgetrfBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasDgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasDgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgetrfBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (double**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgetrfBatched(handle_native, n_native, A_native, lda_native, P_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgetrfBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasCgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasCgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgetrfBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuComplex**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgetrfBatched(handle_native, n_native, A_native, lda_native, P_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgetrfBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasZgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasZgetrfBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgetrfBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuDoubleComplex**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgetrfBatched(handle_native, n_native, A_native, lda_native, P_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSgetriBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject C, jint ldc, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasSgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasSgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSgetriBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, C=%p, ldc=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, C, ldc, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    float** C_native = NULL;
    int ldc_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (float**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    C_native = (float**)getPointer(env, C);
    ldc_native = (int)ldc;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasSgetriBatched(handle_native, n_native, A_native, lda_native, P_native, C_native, ldc_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // C is a native pointer
    // ldc is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDgetriBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject C, jint ldc, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasDgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasDgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDgetriBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, C=%p, ldc=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, C, ldc, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    double** C_native = NULL;
    int ldc_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (double**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    C_native = (double**)getPointer(env, C);
    ldc_native = (int)ldc;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasDgetriBatched(handle_native, n_native, A_native, lda_native, P_native, C_native, ldc_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // C is a native pointer
    // ldc is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCgetriBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject C, jint ldc, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasCgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasCgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCgetriBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, C=%p, ldc=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, C, ldc, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    cuComplex** C_native = NULL;
    int ldc_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuComplex**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    C_native = (cuComplex**)getPointer(env, C);
    ldc_native = (int)ldc;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasCgetriBatched(handle_native, n_native, A_native, lda_native, P_native, C_native, ldc_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // C is a native pointer
    // ldc is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZgetriBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject P, jobject C, jint ldc, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (P == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'P' is null for cublasZgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasZgetriBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZgetriBatched(handle=%p, n=%d, A=%p, lda=%d, P=%p, C=%p, ldc=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, P, C, ldc, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex** A_native = NULL;
    int lda_native = 0;
    int* P_native;
    cuDoubleComplex** C_native = NULL;
    int ldc_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuDoubleComplex**)getPointer(env, A);
    lda_native = (int)lda;
    P_native = (int*)getPointer(env, P);
    C_native = (cuDoubleComplex**)getPointer(env, C);
    ldc_native = (int)ldc;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasZgetriBatched(handle_native, n_native, A_native, lda_native, P_native, C_native, ldc_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // P is a native pointer
    // C is a native pointer
    // ldc is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasStrsmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasStrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasStrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasStrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasStrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasStrsmBatched(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, batchCount=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    float** A_native = NULL;
    int lda_native = 0;
    float** B_native = NULL;
    int ldb_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    A_native = (float**)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (float**)getPointer(env, B);
    ldb_native = (int)ldb;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasStrsmBatched(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDtrsmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasDtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasDtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDtrsmBatched(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, batchCount=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    double** A_native = NULL;
    int lda_native = 0;
    double** B_native = NULL;
    int ldb_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    A_native = (double**)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (double**)getPointer(env, B);
    ldb_native = (int)ldb;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasDtrsmBatched(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCtrsmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasCtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasCtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCtrsmBatched(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, batchCount=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cuComplex** A_native = NULL;
    int lda_native = 0;
    cuComplex** B_native = NULL;
    int ldb_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuComplex**)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuComplex**)getPointer(env, B);
    ldb_native = (int)ldb;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasCtrsmBatched(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZtrsmBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint side, jint uplo, jint trans, jint diag, jint m, jint n, jobject alpha, jobject A, jint lda, jobject B, jint ldb, jint batchCount)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cublasZtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cublasZtrsmBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    // batchCount is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZtrsmBatched(handle=%p, side=%d, uplo=%d, trans=%d, diag=%d, m=%d, n=%d, alpha=%p, A=%p, lda=%d, B=%p, ldb=%d, batchCount=%d)\n",
        handle, side, uplo, trans, diag, m, n, alpha, A, lda, B, ldb, batchCount);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t side_native;
    cublasFillMode_t uplo_native;
    cublasOperation_t trans_native;
    cublasDiagType_t diag_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex** A_native = NULL;
    int lda_native = 0;
    cuDoubleComplex** B_native = NULL;
    int ldb_native = 0;
    int batchCount_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    side_native = (cublasSideMode_t)side;
    uplo_native = (cublasFillMode_t)uplo;
    trans_native = (cublasOperation_t)trans;
    diag_native = (cublasDiagType_t)diag;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    A_native = (cuDoubleComplex**)getPointer(env, A);
    lda_native = (int)lda;
    B_native = (cuDoubleComplex**)getPointer(env, B);
    ldb_native = (int)ldb;
    batchCount_native = (int)batchCount;

    // Native function call
    cublasStatus_t jniResult_native = cublasZtrsmBatched(handle_native, side_native, uplo_native, trans_native, diag_native, m_native, n_native, alpha_native, A_native, lda_native, B_native, ldb_native, batchCount_native);

    // Write back native variable values
    // handle is a native pointer object
    // side is primitive
    // uplo is primitive
    // trans is primitive
    // diag is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUBLAS_STATUS_INTERNAL_ERROR;
    // A is a native pointer
    // lda is primitive
    // B is a native pointer
    // ldb is primitive
    // batchCount is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSmatinvBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject Ainv, jint lda_inv, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Ainv == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Ainv' is null for cublasSmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda_inv is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasSmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSmatinvBatched(handle=%p, n=%d, A=%p, lda=%d, Ainv=%p, lda_inv=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, Ainv, lda_inv, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    float** A_native = NULL;
    int lda_native = 0;
    float** Ainv_native = NULL;
    int lda_inv_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (float**)getPointer(env, A);
    lda_native = (int)lda;
    Ainv_native = (float**)getPointer(env, Ainv);
    lda_inv_native = (int)lda_inv;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasSmatinvBatched(handle_native, n_native, A_native, lda_native, Ainv_native, lda_inv_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // Ainv is a native pointer
    // lda_inv is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDmatinvBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject Ainv, jint lda_inv, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Ainv == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Ainv' is null for cublasDmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda_inv is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasDmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDmatinvBatched(handle=%p, n=%d, A=%p, lda=%d, Ainv=%p, lda_inv=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, Ainv, lda_inv, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    double** A_native = NULL;
    int lda_native = 0;
    double** Ainv_native = NULL;
    int lda_inv_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (double**)getPointer(env, A);
    lda_native = (int)lda;
    Ainv_native = (double**)getPointer(env, Ainv);
    lda_inv_native = (int)lda_inv;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasDmatinvBatched(handle_native, n_native, A_native, lda_native, Ainv_native, lda_inv_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // Ainv is a native pointer
    // lda_inv is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCmatinvBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject Ainv, jint lda_inv, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Ainv == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Ainv' is null for cublasCmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda_inv is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasCmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCmatinvBatched(handle=%p, n=%d, A=%p, lda=%d, Ainv=%p, lda_inv=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, Ainv, lda_inv, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuComplex** A_native = NULL;
    int lda_native = 0;
    cuComplex** Ainv_native = NULL;
    int lda_inv_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuComplex**)getPointer(env, A);
    lda_native = (int)lda;
    Ainv_native = (cuComplex**)getPointer(env, Ainv);
    lda_inv_native = (int)lda_inv;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasCmatinvBatched(handle_native, n_native, A_native, lda_native, Ainv_native, lda_inv_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // Ainv is a native pointer
    // lda_inv is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZmatinvBatchedNative(JNIEnv *env, jclass cls, jobject handle, jint n, jobject A, jint lda, jobject Ainv, jint lda_inv, jobject INFO, jint batchSize)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (Ainv == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'Ainv' is null for cublasZmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda_inv is primitive
    if (INFO == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'INFO' is null for cublasZmatinvBatched");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // batchSize is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZmatinvBatched(handle=%p, n=%d, A=%p, lda=%d, Ainv=%p, lda_inv=%d, INFO=%p, batchSize=%d)\n",
        handle, n, A, lda, Ainv, lda_inv, INFO, batchSize);

    // Native variable declarations
    cublasHandle_t handle_native;
    int n_native = 0;
    cuDoubleComplex** A_native = NULL;
    int lda_native = 0;
    cuDoubleComplex** Ainv_native = NULL;
    int lda_inv_native = 0;
    int* INFO_native;
    int batchSize_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    n_native = (int)n;
    A_native = (cuDoubleComplex**)getPointer(env, A);
    lda_native = (int)lda;
    Ainv_native = (cuDoubleComplex**)getPointer(env, Ainv);
    lda_inv_native = (int)lda_inv;
    INFO_native = (int*)getPointer(env, INFO);
    batchSize_native = (int)batchSize;

    // Native function call
    cublasStatus_t jniResult_native = cublasZmatinvBatched(handle_native, n_native, A_native, lda_native, Ainv_native, lda_inv_native, INFO_native, batchSize_native);

    // Write back native variable values
    // handle is a native pointer object
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // Ainv is a native pointer
    // lda_inv is primitive
    // INFO is a native pointer
    // batchSize is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasSdgmmNative(JNIEnv *env, jclass cls, jobject handle, jint mode, jint m, jint n, jobject A, jint lda, jobject x, jint incx, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasSdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive
    // m is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasSdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasSdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasSdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasSdgmm(handle=%p, mode=%d, m=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d, C=%p, ldc=%d)\n",
        handle, mode, m, n, A, lda, x, incx, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t mode_native;
    int m_native = 0;
    int n_native = 0;
    float* A_native;
    int lda_native = 0;
    float* x_native;
    int incx_native = 0;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasSideMode_t)mode;
    m_native = (int)m;
    n_native = (int)n;
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (float*)getPointer(env, x);
    incx_native = (int)incx;
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasSdgmm(handle_native, mode_native, m_native, n_native, A_native, lda_native, x_native, incx_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive
    // m is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasDdgmmNative(JNIEnv *env, jclass cls, jobject handle, jint mode, jint m, jint n, jobject A, jint lda, jobject x, jint incx, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasDdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive
    // m is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasDdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasDdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasDdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasDdgmm(handle=%p, mode=%d, m=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d, C=%p, ldc=%d)\n",
        handle, mode, m, n, A, lda, x, incx, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t mode_native;
    int m_native = 0;
    int n_native = 0;
    double* A_native;
    int lda_native = 0;
    double* x_native;
    int incx_native = 0;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasSideMode_t)mode;
    m_native = (int)m;
    n_native = (int)n;
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (double*)getPointer(env, x);
    incx_native = (int)incx;
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasDdgmm(handle_native, mode_native, m_native, n_native, A_native, lda_native, x_native, incx_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive
    // m is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasCdgmmNative(JNIEnv *env, jclass cls, jobject handle, jint mode, jint m, jint n, jobject A, jint lda, jobject x, jint incx, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasCdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive
    // m is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasCdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasCdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasCdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasCdgmm(handle=%p, mode=%d, m=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d, C=%p, ldc=%d)\n",
        handle, mode, m, n, A, lda, x, incx, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t mode_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* A_native;
    int lda_native = 0;
    cuComplex* x_native;
    int incx_native = 0;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasSideMode_t)mode;
    m_native = (int)m;
    n_native = (int)n;
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuComplex*)getPointer(env, x);
    incx_native = (int)incx;
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasCdgmm(handle_native, mode_native, m_native, n_native, A_native, lda_native, x_native, incx_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive
    // m is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcublas_JCublas2_cublasZdgmmNative(JNIEnv *env, jclass cls, jobject handle, jint mode, jint m, jint n, jobject A, jint lda, jobject x, jint incx, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cublasZdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive
    // m is primitive
    // n is primitive
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cublasZdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cublasZdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // incx is primitive
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cublasZdgmm");
        return JCUBLAS_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cublasZdgmm(handle=%p, mode=%d, m=%d, n=%d, A=%p, lda=%d, x=%p, incx=%d, C=%p, ldc=%d)\n",
        handle, mode, m, n, A, lda, x, incx, C, ldc);

    // Native variable declarations
    cublasHandle_t handle_native;
    cublasSideMode_t mode_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    cuDoubleComplex* x_native;
    int incx_native = 0;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cublasHandle_t)getNativePointerValue(env, handle);
    mode_native = (cublasSideMode_t)mode;
    m_native = (int)m;
    n_native = (int)n;
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    incx_native = (int)incx;
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cublasStatus_t jniResult_native = cublasZdgmm(handle_native, mode_native, m_native, n_native, A_native, lda_native, x_native, incx_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a native pointer object
    // mode is primitive
    // m is primitive
    // n is primitive
    // A is a native pointer
    // lda is primitive
    // x is a native pointer
    // incx is primitive
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}


