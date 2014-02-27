/*
 * JCusparse - Java bindings for CUSPARSE, the NVIDIA CUDA sparse
 * matrix library, to be used with JCuda
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

#include "JCusparse.hpp"
#include "JCusparse_common.hpp"
#include <iostream>
#include <string>
#include <map>

jfieldID cuComplex_x; // float
jfieldID cuComplex_y; // float

jfieldID cuDoubleComplex_x; // double
jfieldID cuDoubleComplex_y; // double


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

    Logger::log(LOG_TRACE, "Initializing JCusparse\n");

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


/*
 * Set the log level
 *
 * Class:     jcuda_jcusparse_JCusparse
 * Method:    setLogLevelNative
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jcuda_jcusparse_JCusparse_setLogLevelNative
  (JNIEnv *env, jclass cla, jint logLevel)
{
    Logger::setLogLevel((LogLevel)logLevel);
}





/**
 * Converts the given jobject
 * into a cuComplex
 * and returns whether this conversion succeeded.
 */
bool initNative(JNIEnv *env, jobject &input, cuComplex &output)
{
    output.x = env->GetFloatField(input, cuComplex_x);
    output.y = env->GetFloatField(input, cuComplex_y);
    if (env->ExceptionCheck())
    {
        return false;
    }
    return true;
}

/**
 * Converts the given jobject
 * into a cuDoubleComplex
 * and returns whether this conversion succeeded.
 */
bool initNative(JNIEnv *env, jobject &input, cuDoubleComplex &output)
{
    output.x = env->GetDoubleField(input, cuDoubleComplex_x);
    output.y = env->GetDoubleField(input, cuDoubleComplex_y);
    if (env->ExceptionCheck())
    {
        return false;
    }
    return true;
}

/**
 * Converts the given cuComplex
 * into a jobject
 * and returns whether this conversion succeeded.
 */
bool releaseNative(JNIEnv *env, cuComplex &input, jobject &output)
{
    env->SetFloatField(output, cuComplex_x, input.x);
    env->SetFloatField(output, cuComplex_y, input.y);
    if (env->ExceptionCheck())
    {
        return false;
    }
    return true;
}

/**
 * Converts the given cuDoubleComplex
 * into a jobject
 * and returns whether this conversion succeeded.
 */
bool releaseNative(JNIEnv *env, cuDoubleComplex &input, jobject &output)
{
    env->SetDoubleField(output, cuDoubleComplex_x, input.x);
    env->SetDoubleField(output, cuDoubleComplex_y, input.y);
    if (env->ExceptionCheck())
    {
        return false;
    }
    return true;
}





JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCreateNative(JNIEnv *env, jclass cls, jobject handle)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCreate");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCreate(handle=%p)\n",
        handle);

    // Native variable declarations
    cusparseHandle_t handle_native;

    // Native function call
    cusparseStatus_t result_native = cusparseCreate(&handle_native);

    // Write back native variable values
    setNativePointerValue(env, handle, (jlong)handle_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDestroyNative(JNIEnv *env, jclass cls, jobject handle)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDestroy");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDestroy(handle=%p)\n",
        handle);

    // Native variable declarations
    cusparseHandle_t handle_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);

    // Native function call
    cusparseStatus_t result_native = cusparseDestroy(handle_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseGetVersionNative(JNIEnv *env, jclass cls, jobject handle, jintArray version)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseGetVersion");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (version == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'version' is null for cusparseGetVersion");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetVersion(handle=%p, version=%p)\n",
        handle, version);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int version_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);

    // Native function call
    cusparseStatus_t result_native = cusparseGetVersion(handle_native, &version_native);

    // Write back native variable values
    if (!set(env, version, 0, version_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSetKernelStreamNative(JNIEnv *env, jclass cls, jobject handle, jobject streamId)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSetKernelStream");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (streamId == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'streamId' is null for cusparseSetKernelStream");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetKernelStream(handle=%p, streamId=%p)\n",
        handle, streamId);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cudaStream_t streamId_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    streamId_native = (cudaStream_t)getNativePointerValue(env, streamId);

    // Native function call
    cusparseStatus_t result_native = cusparseSetKernelStream(handle_native, streamId_native);

    // Return the result
    return (jint)result_native;
}

// Returns a matrix descriptor initialized to: CUSPARSE_MATRIX_TYPE_GENERAL, CUSPARSE_INDEX_BASE_ZERO
// Other fields are uninitialized
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCreateMatDescrNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCreateMatDescr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCreateMatDescr(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Native function call
    cusparseStatus_t result_native = cusparseCreateMatDescr(&descrA_native);

    // Write back native variable values
    setNativePointerValue(env, descrA, (jlong)descrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDestroyMatDescrNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDestroyMatDescr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDestroyMatDescr(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseStatus_t result_native = cusparseDestroyMatDescr(descrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSetMatTypeNative(JNIEnv *env, jclass cls, jobject descrA, jint type)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatType(descrA=%p, type=%d)\n",
        descrA, type);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseMatrixType_t type_native = CUSPARSE_MATRIX_TYPE_GENERAL;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    type_native = (cusparseMatrixType_t)type;

    // Native function call
    cusparseStatus_t result_native = cusparseSetMatType(descrA_native, type_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseGetMatTypeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatType(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseMatrixType_t result_native = cusparseGetMatType(descrA_native);

    // Return the result
    return (jint)result_native;
}

// type must be _GENERAL, _SYMMETRIC, _HERMITIAN, or _TRIANGULAR
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSetMatFillModeNative(JNIEnv *env, jclass cls, jobject descrA, jint fillMode)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatFillMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatFillMode(descrA=%p, fillMode=%d)\n",
        descrA, fillMode);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseFillMode_t fillMode_native = CUSPARSE_FILL_MODE_LOWER;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    fillMode_native = (cusparseFillMode_t)fillMode;

    // Native function call
    cusparseStatus_t result_native = cusparseSetMatFillMode(descrA_native, fillMode_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseGetMatFillModeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatFillMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatFillMode(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseFillMode_t result_native = cusparseGetMatFillMode(descrA_native);

    // Return the result
    return (jint)result_native;
}

// type must be _TRIANGULAR
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSetMatDiagTypeNative(JNIEnv *env, jclass cls, jobject descrA, jint diagType)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatDiagType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatDiagType(descrA=%p, diagType=%d)\n",
        descrA, diagType);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseDiagType_t diagType_native = CUSPARSE_DIAG_TYPE_NON_UNIT;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    diagType_native = (cusparseDiagType_t)diagType;

    // Native function call
    cusparseStatus_t result_native = cusparseSetMatDiagType(descrA_native, diagType_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseGetMatDiagTypeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatDiagType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatDiagType(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseDiagType_t result_native = cusparseGetMatDiagType(descrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSetMatIndexBaseNative(JNIEnv *env, jclass cls, jobject descrA, jint base)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatIndexBase");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatIndexBase(descrA=%p, base=%d)\n",
        descrA, base);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseIndexBase_t base_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    base_native = (cusparseIndexBase_t)base;

    // Native function call
    cusparseStatus_t result_native = cusparseSetMatIndexBase(descrA_native, base_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseGetMatIndexBaseNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatIndexBase");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatIndexBase(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseIndexBase_t result_native = cusparseGetMatIndexBase(descrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCreateSolveAnalysisInfoNative(JNIEnv *env, jclass cls, jobject info)
{
    // Null-checks for non-primitive arguments
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCreateSolveAnalysisInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCreateSolveAnalysisInfo(info=%p)\n",
        info);

    // Native variable declarations
    cusparseSolveAnalysisInfo_t info_native;

    // Native function call
    cusparseStatus_t result_native = cusparseCreateSolveAnalysisInfo(&info_native);

    // Write back native variable values
    setNativePointerValue(env, info, (jlong)info_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDestroySolveAnalysisInfoNative(JNIEnv *env, jclass cls, jobject info)
{
    // Null-checks for non-primitive arguments
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDestroySolveAnalysisInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDestroySolveAnalysisInfo(info=%p)\n",
        info);

    // Native variable declarations
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t result_native = cusparseDestroySolveAnalysisInfo(info_native);

    // Return the result
    return (jint)result_native;
}

/** Sparse Level 1 functions */
//Adds a scalar multiple of compressed sparse vector to a
//full-storage vector
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jfloat alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSaxpyi(handle=%p, nnz=%d, alpha=%f, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float alpha_native = 0.0f;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    float* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    alpha_native = (float)alpha;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jdouble alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDaxpyi(handle=%p, nnz=%d, alpha=%lf, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double alpha_native = 0.0;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    double* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    alpha_native = (double)alpha;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex alpha_native;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuComplex* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex alpha_native;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuDoubleComplex* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

//Computes the dot product of a compressed sparse real vector
//with a full-storage real vector.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jfloatArray resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    float* y_native = NULL;
    float resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    set(env, resultHostPtr, 0, (float)resultHostPtr_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jdoubleArray resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    double* y_native = NULL;
    double resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    set(env, resultHostPtr, 0, (double)resultHostPtr_native);

    // Return the result
    return (jint)result_native;
}

//Computes the dot product of a compressed sparse complex vector with
//a full-storage complex vector.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuComplex* y_native = NULL;
    cuComplex resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    if (!releaseNative(env, resultHostPtr_native, resultHostPtr)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuDoubleComplex* y_native = NULL;
    cuDoubleComplex resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    if (!releaseNative(env, resultHostPtr_native, resultHostPtr)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

//Computes the conjugated dot product of a compressed sparse complex
//vector with a full-storage complex vector.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCdotciNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdotci(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuComplex* y_native = NULL;
    cuComplex resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCdotci(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    if (!releaseNative(env, resultHostPtr_native, resultHostPtr)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZdotciNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (resultHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultHostPtr' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdotci(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuDoubleComplex* y_native = NULL;
    cuDoubleComplex resultHostPtr_native;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZdotci(handle_native, nnz_native, xVal_native, xInd_native, y_native, &resultHostPtr_native, idxBase_native);

    // Write back native variable values
    if (!releaseNative(env, resultHostPtr_native, resultHostPtr)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

//Gathers a full-storage sparse vector's elements into compressed form.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* y_native = NULL;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (float*)getPointer(env, y);
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* y_native = NULL;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (double*)getPointer(env, y);
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* y_native = NULL;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuComplex*)getPointer(env, y);
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* y_native = NULL;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

//Gathers a non-zero elements from full-storage vector y into
//compressed sparse vector x, and replaces these elements in y by zeros.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* y_native = NULL;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (float*)getPointer(env, y);
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* y_native = NULL;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (double*)getPointer(env, y);
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* y_native = NULL;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuComplex*)getPointer(env, y);
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* y_native = NULL;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

//Scatters elements od compressed sparse vector x into
//full-storage vector y.
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    float* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    double* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseCsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseCsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuComplex* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseZsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseZsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native = NULL;
    int* xInd_native = NULL;
    cuDoubleComplex* y_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

//Performs Givens rotation
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSrotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jfloat c, jfloat s, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSroti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, c=%f, s=%f, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, c, s, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native = NULL;
    int* xInd_native = NULL;
    float* y_native = NULL;
    float c_native = 0.0f;
    float s_native = 0.0f;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    c_native = (float)c;
    s_native = (float)s;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseSroti(handle_native, nnz_native, xVal_native, xInd_native, y_native, c_native, s_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDrotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jdouble c, jdouble s, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xVal' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (xInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'xInd' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDroti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, c=%lf, s=%lf, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, c, s, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native = NULL;
    int* xInd_native = NULL;
    double* y_native = NULL;
    double c_native = 0.0;
    double s_native = 0.0;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    c_native = (double)c;
    s_native = (double)s;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDroti(handle_native, nnz_native, xVal_native, xInd_native, y_native, c_native, s_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

/** Sparse Level 2 functions */
//Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y,
//where A is a sparse matrix, x and y are dense vectors
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jfloat alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jfloat beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrmv(handle=%p, transA=%d, m=%d, n=%d, alpha=%f, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%f, y=%p)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    float alpha_native = 0.0f;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    float* x_native = NULL;
    float beta_native = 0.0f;
    float* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    alpha_native = (float)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (float*)getPointer(env, x);
    beta_native = (float)beta;
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseScsrmv(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jdouble alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jdouble beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrmv(handle=%p, transA=%d, m=%d, n=%d, alpha=%lf, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%lf, y=%p)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    double alpha_native = 0.0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    double* x_native = NULL;
    double beta_native = 0.0;
    double* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    alpha_native = (double)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (double*)getPointer(env, x);
    beta_native = (double)beta;
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseDcsrmv(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrmv(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    cuComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuComplex* x_native = NULL;
    cuComplex beta_native;
    cuComplex* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (cuComplex*)getPointer(env, x);
    if (!initNative(env, beta, beta_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseCcsrmv(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrmv(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuDoubleComplex* x_native = NULL;
    cuDoubleComplex beta_native;
    cuDoubleComplex* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    if (!initNative(env, beta, beta_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseZcsrmv(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Return the result
    return (jint)result_native;
}

//Solution of triangular linear system op(A) * y = alpha * x,
//where A is a sparse matrix, x and y are dense vectors
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrsv_analysis(handle=%p, transA=%d, m=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t result_native = cusparseScsrsv_analysis(handle_native, transA_native, m_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsv_analysis(handle=%p, transA=%d, m=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t result_native = cusparseDcsrsv_analysis(handle_native, transA_native, m_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrsv_analysis(handle=%p, transA=%d, m=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t result_native = cusparseCcsrsv_analysis(handle_native, transA_native, m_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrsv_analysis(handle=%p, transA=%d, m=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t result_native = cusparseZcsrsv_analysis(handle_native, transA_native, m_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jfloat alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%f, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    float alpha_native = 0.0f;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;
    float* x_native = NULL;
    float* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    alpha_native = (float)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (float*)getPointer(env, x);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseScsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jdouble alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%lf, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    double alpha_native = 0.0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;
    double* x_native = NULL;
    double* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    alpha_native = (double)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (double*)getPointer(env, x);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseDcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cuComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;
    cuComplex* x_native = NULL;
    cuComplex* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuComplex*)getPointer(env, x);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseCcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    cuDoubleComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cusparseSolveAnalysisInfo_t info_native;
    cuDoubleComplex* x_native = NULL;
    cuDoubleComplex* y_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t result_native = cusparseZcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Return the result
    return (jint)result_native;
}

/** Sparse Level 3 functions */
//Matrix-matrix multiplication C = alpha * op(A) * B  + beta * C,
//where A is a sparse matrix, B and C are dense matrices
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jfloat alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jfloat beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, alpha=%f, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%f, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    float alpha_native = 0.0f;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    float* B_native = NULL;
    int ldb_native = 0;
    float beta_native = 0.0f;
    float* C_native = NULL;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    alpha_native = (float)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    beta_native = (float)beta;
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t result_native = cusparseScsrmm(handle_native, transA_native, m_native, n_native, k_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jdouble alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jdouble beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, alpha=%lf, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%lf, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    double alpha_native = 0.0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    double* B_native = NULL;
    int ldb_native = 0;
    double beta_native = 0.0;
    double* C_native = NULL;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    alpha_native = (double)alpha;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    beta_native = (double)beta;
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t result_native = cusparseDcsrmm(handle_native, transA_native, m_native, n_native, k_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuComplex* B_native = NULL;
    int ldb_native = 0;
    cuComplex beta_native;
    cuComplex* C_native = NULL;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    if (!initNative(env, beta, beta_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t result_native = cusparseCcsrmm(handle_native, transA_native, m_native, n_native, k_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native = CUSPARSE_OPERATION_NON_TRANSPOSE;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cuDoubleComplex alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuDoubleComplex* B_native = NULL;
    int ldb_native = 0;
    cuDoubleComplex beta_native;
    cuDoubleComplex* C_native = NULL;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    if (!initNative(env, alpha, alpha_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    if (!initNative(env, beta, beta_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t result_native = cusparseZcsrmm(handle_native, transA_native, m_native, n_native, k_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Return the result
    return (jint)result_native;
}

/** Sparse Format Conversion */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzHostPtr' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native = CUSPARSE_DIRECTION_ROW;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRowCol_native = NULL;
    int* nnzHostPtr_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    PointerData *nnzHostPtrPointerData = initPointerData(env, nnzHostPtr);
    if (nnzHostPtrPointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    nnzHostPtr_native = (int*)nnzHostPtrPointerData->getPointer(env);

    // Native function call
    cusparseStatus_t result_native = cusparseSnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzHostPtr_native);

    // Write back native variable values
    if (!releasePointerData(env, nnzHostPtrPointerData)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzHostPtr' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native = CUSPARSE_DIRECTION_ROW;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRowCol_native = NULL;
    int* nnzHostPtr_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    PointerData *nnzHostPtrPointerData = initPointerData(env, nnzHostPtr);
    if (nnzHostPtrPointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    nnzHostPtr_native = (int*)nnzHostPtrPointerData->getPointer(env);

    // Native function call
    cusparseStatus_t result_native = cusparseDnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzHostPtr_native);

    // Write back native variable values
    if (!releasePointerData(env, nnzHostPtrPointerData)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzHostPtr' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native = CUSPARSE_DIRECTION_ROW;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRowCol_native = NULL;
    int* nnzHostPtr_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    PointerData *nnzHostPtrPointerData = initPointerData(env, nnzHostPtr);
    if (nnzHostPtrPointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    nnzHostPtr_native = (int*)nnzHostPtrPointerData->getPointer(env);

    // Native function call
    cusparseStatus_t result_native = cusparseCnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzHostPtr_native);

    // Write back native variable values
    if (!releasePointerData(env, nnzHostPtrPointerData)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzHostPtr' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native = CUSPARSE_DIRECTION_ROW;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRowCol_native = NULL;
    int* nnzHostPtr_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    PointerData *nnzHostPtrPointerData = initPointerData(env, nnzHostPtr);
    if (nnzHostPtrPointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    nnzHostPtr_native = (int*)nnzHostPtrPointerData->getPointer(env);

    // Native function call
    cusparseStatus_t result_native = cusparseZnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzHostPtr_native);

    // Write back native variable values
    if (!releasePointerData(env, nnzHostPtrPointerData)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    return (jint)result_native;
}

//Dense to CSR
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSdense2csr(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRow_native = NULL;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t result_native = cusparseSdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDdense2csr(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRow_native = NULL;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t result_native = cusparseDdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdense2csr(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRow_native = NULL;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t result_native = cusparseCdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdense2csr(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerRow_native = NULL;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t result_native = cusparseZdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Return the result
    return (jint)result_native;
}

//CSR to Dense
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    float* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseScsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    double* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseDcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuComplex* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseCcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native = NULL;
    int* csrRowPtrA_native = NULL;
    int* csrColIndA_native = NULL;
    cuDoubleComplex* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseZcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

//Dense to CSC
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseSdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerCol' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSdense2csc(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerCol=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* A_native = NULL;
    int lda_native = 0;
    int* nnzPerCol_native = NULL;
    float* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerCol_native = (int*)getPointer(env, nnzPerCol);
    cscValA_native = (float*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);

    // Native function call
    cusparseStatus_t result_native = cusparseSdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerCol' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDdense2csc(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerCol=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* A_native = NULL;
    int lda_native = 0;
    int* nnzPerCol_native = NULL;
    double* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerCol_native = (int*)getPointer(env, nnzPerCol);
    cscValA_native = (double*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);

    // Native function call
    cusparseStatus_t result_native = cusparseDdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerCol' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdense2csc(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerCol=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerCol_native = NULL;
    cuComplex* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerCol_native = (int*)getPointer(env, nnzPerCol);
    cscValA_native = (cuComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);

    // Native function call
    cusparseStatus_t result_native = cusparseCdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzPerCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerCol' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdense2csc(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerCol=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p)\n",
        handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* A_native = NULL;
    int lda_native = 0;
    int* nnzPerCol_native = NULL;
    cuDoubleComplex* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerCol_native = (int*)getPointer(env, nnzPerCol);
    cscValA_native = (cuDoubleComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);

    // Native function call
    cusparseStatus_t result_native = cusparseZdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Return the result
    return (jint)result_native;
}

//CSC to Dense
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;
    float* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (float*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseScsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;
    double* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (double*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseDcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;
    cuComplex* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (cuComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseCcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* cscValA_native = NULL;
    int* cscRowIndA_native = NULL;
    int* cscColPtrA_native = NULL;
    cuDoubleComplex* A_native = NULL;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (cuDoubleComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t result_native = cusparseZcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Return the result
    return (jint)result_native;
}

//COO to CSR, CSR to COO
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseXcoo2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject cooRowInd, jint nnz, jint m, jobject csrRowPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseXcoo2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cooRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cooRowInd' is null for cusparseXcoo2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseXcoo2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcoo2csr(handle=%p, cooRowInd=%p, nnz=%d, m=%d, csrRowPtr=%p, idxBase=%d)\n",
        handle, cooRowInd, nnz, m, csrRowPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int* cooRowInd_native = NULL;
    int nnz_native = 0;
    int m_native = 0;
    int* csrRowPtr_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    cooRowInd_native = (int*)getPointer(env, cooRowInd);
    nnz_native = (int)nnz;
    m_native = (int)m;
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseXcoo2csr(handle_native, cooRowInd_native, nnz_native, m_native, csrRowPtr_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseXcsr2cooNative(JNIEnv *env, jclass cls, jobject handle, jobject csrRowPtr, jint nnz, jint m, jobject cooRowInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseXcsr2coo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseXcsr2coo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cooRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cooRowInd' is null for cusparseXcsr2coo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcsr2coo(handle=%p, csrRowPtr=%p, nnz=%d, m=%d, cooRowInd=%p, idxBase=%d)\n",
        handle, csrRowPtr, nnz, m, cooRowInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int* csrRowPtr_native = NULL;
    int nnz_native = 0;
    int m_native = 0;
    int* cooRowInd_native = NULL;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    nnz_native = (int)nnz;
    m_native = (int)m;
    cooRowInd_native = (int*)getPointer(env, cooRowInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseXcsr2coo(handle_native, csrRowPtr_native, nnz_native, m_native, cooRowInd_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

//CSR to CSC  (transpose)
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseScsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrVal' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColInd' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2csc(handle=%p, m=%d, n=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    float* csrVal_native = NULL;
    int* csrRowPtr_native = NULL;
    int* csrColInd_native = NULL;
    float* cscVal_native = NULL;
    int* cscRowInd_native = NULL;
    int* cscColPtr_native = NULL;
    int copyValues_native = 0;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    csrVal_native = (float*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (float*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (int)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseScsr2csc(handle_native, m_native, n_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseDcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrVal' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColInd' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2csc(handle=%p, m=%d, n=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    double* csrVal_native = NULL;
    int* csrRowPtr_native = NULL;
    int* csrColInd_native = NULL;
    double* cscVal_native = NULL;
    int* cscRowInd_native = NULL;
    int* cscColPtr_native = NULL;
    int copyValues_native = 0;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    csrVal_native = (double*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (double*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (int)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseDcsr2csc(handle_native, m_native, n_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseCcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrVal' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColInd' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2csc(handle=%p, m=%d, n=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* csrVal_native = NULL;
    int* csrRowPtr_native = NULL;
    int* csrColInd_native = NULL;
    cuComplex* cscVal_native = NULL;
    int* cscRowInd_native = NULL;
    int* cscColPtr_native = NULL;
    int copyValues_native = 0;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    csrVal_native = (cuComplex*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (cuComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (int)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseCcsr2csc(handle_native, m_native, n_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse_cusparseZcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrVal' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColInd' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2csc(handle=%p, m=%d, n=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* csrVal_native = NULL;
    int* csrRowPtr_native = NULL;
    int* csrColInd_native = NULL;
    cuDoubleComplex* cscVal_native = NULL;
    int* cscRowInd_native = NULL;
    int* cscColPtr_native = NULL;
    int copyValues_native = 0;
    cusparseIndexBase_t idxBase_native = CUSPARSE_INDEX_BASE_ZERO;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    csrVal_native = (cuDoubleComplex*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (cuDoubleComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (int)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t result_native = cusparseZcsr2csc(handle_native, m_native, n_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Return the result
    return (jint)result_native;
}


