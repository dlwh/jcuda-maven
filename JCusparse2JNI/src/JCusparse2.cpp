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

#include "JCusparse2.hpp"
#include "JCusparse2_common.hpp"
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
JNIEXPORT void JNICALL Java_jcuda_jcusparse_JCusparse2_setLogLevelNative
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










//=== Auto-generated part: ===================================================


/** CUSPARSE initialization and managment routines */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCreateNative(JNIEnv *env, jclass cls, jobject handle)
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

    // Obtain native variable values
    // handle is write-only

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCreate(&handle_native);

    // Write back native variable values
    setNativePointerValue(env, handle, (jlong)handle_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDestroyNative(JNIEnv *env, jclass cls, jobject handle)
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
    cusparseStatus_t jniResult_native = cusparseDestroy(handle_native);

    // Write back native variable values
    // handle is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetVersionNative(JNIEnv *env, jclass cls, jobject handle, jintArray version)
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
    int version_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    // version is set here

    // Native function call
    cusparseStatus_t jniResult_native = cusparseGetVersion(handle_native, &version_native);

    // Write back native variable values
    // handle is a read-only native pointer
    if (!set(env, version, 0, (jint)version_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetStreamNative(JNIEnv *env, jclass cls, jobject handle, jobject streamId)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSetStream");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // streamId may be NULL

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetStream(handle=%p, streamId=%p)\n",
        handle, streamId);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cudaStream_t streamId_native = NULL;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    if (streamId != NULL)
    {
        streamId_native = (cudaStream_t)getNativePointerValue(env, streamId);
    }

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetStream(handle_native, streamId_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // streamId is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** CUSPARSE type creation, destruction, set and get routines */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetPointerModeNative(JNIEnv *env, jclass cls, jobject handle, jintArray mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseGetPointerMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (mode == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'mode' is null for cusparseGetPointerMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetPointerMode(handle=%p, mode=%p)\n",
        handle, mode);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparsePointerMode_t mode_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    // mode is set here

    // Native function call
    cusparseStatus_t jniResult_native = cusparseGetPointerMode(handle_native, &mode_native);

    // Write back native variable values
    // handle is a read-only native pointer
    if (!set(env, mode, 0, (jint)mode_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetPointerModeNative(JNIEnv *env, jclass cls, jobject handle, jint mode)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSetPointerMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // mode is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetPointerMode(handle=%p, mode=%d)\n",
        handle, mode);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparsePointerMode_t mode_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    mode_native = (cusparsePointerMode_t)mode;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetPointerMode(handle_native, mode_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // mode is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** sparse matrix descriptor */
/**
 * <pre>
 * When the matrix descriptor is created, its fields are initialized to:
   CUSPARSE_MATRIX_TYPE_GENERAL
   CUSPARSE_INDEX_BASE_ZERO
   All other fields are uninitialized
 * </pre>
 */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCreateMatDescrNative(JNIEnv *env, jclass cls, jobject descrA)
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

    // Obtain native variable values
    // descrA is write-only

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCreateMatDescr(&descrA_native);

    // Write back native variable values
    setNativePointerValue(env, descrA, (jlong)descrA_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDestroyMatDescrNative(JNIEnv *env, jclass cls, jobject descrA)
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
    cusparseStatus_t jniResult_native = cusparseDestroyMatDescr(descrA_native);

    // Write back native variable values
    // descrA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetMatTypeNative(JNIEnv *env, jclass cls, jobject descrA, jint type)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // type is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatType(descrA=%p, type=%d)\n",
        descrA, type);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseMatrixType_t type_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    type_native = (cusparseMatrixType_t)type;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetMatType(descrA_native, type_native);

    // Write back native variable values
    // descrA is a read-only native pointer
    // type is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetMatTypeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatType");
        return 0;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatType(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseMatrixType_t jniResult_native = cusparseGetMatType(descrA_native);

    // Write back native variable values
    // descrA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetMatFillModeNative(JNIEnv *env, jclass cls, jobject descrA, jint fillMode)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatFillMode");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // fillMode is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatFillMode(descrA=%p, fillMode=%d)\n",
        descrA, fillMode);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseFillMode_t fillMode_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    fillMode_native = (cusparseFillMode_t)fillMode;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetMatFillMode(descrA_native, fillMode_native);

    // Write back native variable values
    // descrA is a read-only native pointer
    // fillMode is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetMatFillModeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatFillMode");
        return 0;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatFillMode(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseFillMode_t jniResult_native = cusparseGetMatFillMode(descrA_native);

    // Write back native variable values
    // descrA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetMatDiagTypeNative(JNIEnv *env, jclass cls, jobject descrA, jint diagType)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatDiagType");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // diagType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatDiagType(descrA=%p, diagType=%d)\n",
        descrA, diagType);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseDiagType_t diagType_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    diagType_native = (cusparseDiagType_t)diagType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetMatDiagType(descrA_native, diagType_native);

    // Write back native variable values
    // descrA is a read-only native pointer
    // diagType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetMatDiagTypeNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatDiagType");
        return 0;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatDiagType(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseDiagType_t jniResult_native = cusparseGetMatDiagType(descrA_native);

    // Write back native variable values
    // descrA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSetMatIndexBaseNative(JNIEnv *env, jclass cls, jobject descrA, jint base)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSetMatIndexBase");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // base is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSetMatIndexBase(descrA=%p, base=%d)\n",
        descrA, base);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;
    cusparseIndexBase_t base_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    base_native = (cusparseIndexBase_t)base;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSetMatIndexBase(descrA_native, base_native);

    // Write back native variable values
    // descrA is a read-only native pointer
    // base is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetMatIndexBaseNative(JNIEnv *env, jclass cls, jobject descrA)
{
    // Null-checks for non-primitive arguments
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseGetMatIndexBase");
        return 0;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetMatIndexBase(descrA=%p)\n",
        descrA);

    // Native variable declarations
    cusparseMatDescr_t descrA_native;

    // Obtain native variable values
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);

    // Native function call
    cusparseIndexBase_t jniResult_native = cusparseGetMatIndexBase(descrA_native);

    // Write back native variable values
    // descrA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** sparse traingular solve */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCreateSolveAnalysisInfoNative(JNIEnv *env, jclass cls, jobject info)
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

    // Obtain native variable values
    // info is write-only

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCreateSolveAnalysisInfo(&info_native);

    // Write back native variable values
    setNativePointerValue(env, info, (jlong)info_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDestroySolveAnalysisInfoNative(JNIEnv *env, jclass cls, jobject info)
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
    cusparseStatus_t jniResult_native = cusparseDestroySolveAnalysisInfo(info_native);

    // Write back native variable values
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}


JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseGetLevelInfoNative(JNIEnv *env, jclass cls, jobject handle, jobject info, jintArray nlevels, jobject levelPtr, jobject levelInd)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseGetLevelInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseGetLevelInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nlevels == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nlevels' is null for cusparseGetLevelInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (levelPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'levelPtr' is null for cusparseGetLevelInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (levelInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'levelInd' is null for cusparseGetLevelInfo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseGetLevelInfo(handle=%p, info=%p, nlevels=%p, levelPtr=%p, levelInd=%p)\n",
        handle, info, nlevels, levelPtr, levelInd);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseSolveAnalysisInfo_t info_native;
    int nlevels_native = 0;
    int** levelPtr_native;
    int** levelInd_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    // nlevels is set here
    levelPtr_native = (int**)getPointer(env, levelPtr);
    levelInd_native = (int**)getPointer(env, levelInd);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseGetLevelInfo(handle_native, info_native, &nlevels_native, levelPtr_native, levelInd_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // info is a read-only native pointer
    if (!set(env, nlevels, 0, (jint)nlevels_native)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
	setNativePointerValue(env, levelPtr, (jlong)*levelPtr_native);
	setNativePointerValue(env, levelInd, (jlong)*levelInd_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}




/** hybrid (HYB) format */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCreateHybMatNative(JNIEnv *env, jclass cls, jobject hybA)
{
    // Null-checks for non-primitive arguments
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseCreateHybMat");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCreateHybMat(hybA=%p)\n",
        hybA);

    // Native variable declarations
    cusparseHybMat_t hybA_native;

    // Obtain native variable values
    // hybA is write-only

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCreateHybMat(&hybA_native);

    // Write back native variable values
    setNativePointerValue(env, hybA, (jlong)hybA_native);

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDestroyHybMatNative(JNIEnv *env, jclass cls, jobject hybA)
{
    // Null-checks for non-primitive arguments
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDestroyHybMat");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDestroyHybMat(hybA=%p)\n",
        hybA);

    // Native variable declarations
    cusparseHybMat_t hybA_native;

    // Obtain native variable values
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDestroyHybMat(hybA_native);

    // Write back native variable values
    // hybA is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Sparse Level 1 routines --- */
/** Description: Addition of a scalar multiple of a sparse vector x  
   and a dense vector y. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseSaxpyi");
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* alpha_native;
    float* xVal_native;
    int* xInd_native;
    float* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDaxpyi");
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* alpha_native;
    double* xVal_native;
    int* xInd_native;
    double* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* alpha_native;
    cuComplex* xVal_native;
    int* xInd_native;
    cuComplex* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZaxpyiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject alpha, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZaxpyi");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZaxpyi(handle=%p, nnz=%d, alpha=%p, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, alpha, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* alpha_native;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cuDoubleComplex* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZaxpyi(handle_native, nnz_native, alpha_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: dot product of a sparse vector x and a dense vector y. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseSdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native;
    int* xInd_native;
    float* y_native;
    float* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (float*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseDdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native;
    int* xInd_native;
    double* y_native;
    double* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (double*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseCdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native;
    int* xInd_native;
    cuComplex* y_native;
    cuComplex* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (cuComplex*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZdotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseZdoti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdoti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cuDoubleComplex* y_native;
    cuDoubleComplex* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (cuDoubleComplex*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZdoti(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: dot product of complex conjugate of a sparse vector x
   and a dense vector y. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCdotciNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseCdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdotci(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native;
    int* xInd_native;
    cuComplex* y_native;
    cuComplex* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (cuComplex*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCdotci(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZdotciNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject resultDevHostPtr, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (resultDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'resultDevHostPtr' is null for cusparseZdotci");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdotci(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, resultDevHostPtr=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, resultDevHostPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cuDoubleComplex* y_native;
    cuDoubleComplex* resultDevHostPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    PointerData *resultDevHostPtr_pointerData = initPointerData(env, resultDevHostPtr);
    if (resultDevHostPtr_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    resultDevHostPtr_native = (cuDoubleComplex*)resultDevHostPtr_pointerData->getPointer(env);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZdotci(handle_native, nnz_native, xVal_native, xInd_native, y_native, resultDevHostPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // If the PointerData is not backed by native memory, then this call has to block
    if (!isPointerBackedByNativeMemory(env, resultDevHostPtr))
    {
        cudaDeviceSynchronize();
    }
    if (!releasePointerData(env, resultDevHostPtr_pointerData, 0)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Gather of non-zero elements from dense vector y into 
   sparse vector x. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* y_native;
    float* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (float*)getPointer(env, y);
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* y_native;
    double* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (double*)getPointer(env, y);
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* y_native;
    cuComplex* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuComplex*)getPointer(env, y);
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZgthrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgthr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgthr(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* y_native;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZgthr(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Gather of non-zero elements from desne vector y into 
   sparse vector x (also replacing these elements in y by zeros). */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* y_native;
    float* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (float*)getPointer(env, y);
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* y_native;
    double* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (double*)getPointer(env, y);
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* y_native;
    cuComplex* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuComplex*)getPointer(env, y);
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZgthrzNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject y, jobject xVal, jobject xInd, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgthrz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgthrz(handle=%p, nnz=%d, y=%p, xVal=%p, xInd=%p, idxBase=%d)\n",
        handle, nnz, y, xVal, xInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* y_native;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZgthrz(handle_native, nnz_native, y_native, xVal_native, xInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // y is a native pointer
    // xVal is a native pointer
    // xInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Scatter of elements of the sparse vector x into 
   dense vector y. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native;
    int* xInd_native;
    float* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native;
    int* xInd_native;
    double* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuComplex* xVal_native;
    int* xInd_native;
    cuComplex* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZsctrNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZsctr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZsctr(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    cuDoubleComplex* xVal_native;
    int* xInd_native;
    cuDoubleComplex* y_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (cuDoubleComplex*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (cuDoubleComplex*)getPointer(env, y);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZsctr(handle_native, nnz_native, xVal_native, xInd_native, y_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Givens rotation, where c and s are cosine and sine, 
   x and y are sparse and dense vectors, respectively. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSrotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject c, jobject s, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cusparseSroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSroti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, c=%p, s=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, c, s, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    float* xVal_native;
    int* xInd_native;
    float* y_native;
    float* c_native;
    float* s_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (float*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (float*)getPointer(env, y);
    c_native = (float*)getPointer(env, c);
    s_native = (float*)getPointer(env, s);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSroti(handle_native, nnz_native, xVal_native, xInd_native, y_native, c_native, s_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // c is a native pointer
    // s is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDrotiNative(JNIEnv *env, jclass cls, jobject handle, jint nnz, jobject xVal, jobject xInd, jobject y, jobject c, jobject s, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnz is primitive
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
    if (c == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'c' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (s == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 's' is null for cusparseDroti");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDroti(handle=%p, nnz=%d, xVal=%p, xInd=%p, y=%p, c=%p, s=%p, idxBase=%d)\n",
        handle, nnz, xVal, xInd, y, c, s, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int nnz_native = 0;
    double* xVal_native;
    int* xInd_native;
    double* y_native;
    double* c_native;
    double* s_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    nnz_native = (int)nnz;
    xVal_native = (double*)getPointer(env, xVal);
    xInd_native = (int*)getPointer(env, xInd);
    y_native = (double*)getPointer(env, y);
    c_native = (double*)getPointer(env, c);
    s_native = (double*)getPointer(env, s);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDroti(handle_native, nnz_native, xVal_native, xInd_native, y_native, c_native, s_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // nnz is primitive
    // xVal is a native pointer
    // xInd is a native pointer
    // y is a native pointer
    // c is a native pointer
    // s is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Sparse Level 2 routines --- */
/** Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y, 
   where A is a sparse matrix in CSR storage format, x and y are dense vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrmv");
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
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseScsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrmv(handle=%p, transA=%d, m=%d, n=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    float* x_native;
    float* beta_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (float*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrmv(handle_native, transA_native, m_native, n_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrmv");
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
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrmv(handle=%p, transA=%d, m=%d, n=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    double* x_native;
    double* beta_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (double*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrmv(handle_native, transA_native, m_native, n_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseCcsrmv(handle=%p, transA=%d, m=%d, n=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuComplex* x_native;
    cuComplex* beta_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (cuComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrmv(handle_native, transA_native, m_native, n_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseZcsrmv(handle=%p, transA=%d, m=%d, n=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, m, n, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuDoubleComplex* x_native;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrmv(handle_native, transA_native, m_native, n_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y, 
   where A is a sparse matrix in HYB storage format, x and y are dense vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShybmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject alpha, jobject descrA, jobject hybA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseShybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShybmv(handle=%p, transA=%d, alpha=%p, descrA=%p, hybA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, alpha, descrA, hybA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    float* x_native;
    float* beta_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    x_native = (float*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShybmv(handle_native, transA_native, alpha_native, descrA_native, hybA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhybmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject alpha, jobject descrA, jobject hybA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhybmv(handle=%p, transA=%d, alpha=%p, descrA=%p, hybA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, alpha, descrA, hybA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    double* x_native;
    double* beta_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    x_native = (double*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhybmv(handle_native, transA_native, alpha_native, descrA_native, hybA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChybmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject alpha, jobject descrA, jobject hybA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseChybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChybmv(handle=%p, transA=%d, alpha=%p, descrA=%p, hybA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, alpha, descrA, hybA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuComplex* x_native;
    cuComplex* beta_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    x_native = (cuComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChybmv(handle_native, transA_native, alpha_native, descrA_native, hybA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhybmvNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject alpha, jobject descrA, jobject hybA, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZhybmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhybmv(handle=%p, transA=%d, alpha=%p, descrA=%p, hybA=%p, x=%p, beta=%p, y=%p)\n",
        handle, transA, alpha, descrA, hybA, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuDoubleComplex* x_native;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhybmv(handle_native, transA_native, alpha_native, descrA_native, hybA_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y, 
   where A is a sparse matrix in BSR storage format, x and y are dense vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSbsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSbsrmv(handle=%p, dirA=%d, transA=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, mb, nb, nnzb, alpha, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    float* x_native;
    float* beta_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (float*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (float*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSbsrmv(handle_native, dirA_native, transA_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDbsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDbsrmv(handle=%p, dirA=%d, transA=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, mb, nb, nnzb, alpha, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    double* x_native;
    double* beta_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (double*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (double*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDbsrmv(handle_native, dirA_native, transA_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCbsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCbsrmv(handle=%p, dirA=%d, transA=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, mb, nb, nnzb, alpha, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cuComplex* x_native;
    cuComplex* beta_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuComplex*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (cuComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCbsrmv(handle_native, dirA_native, transA_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZbsrmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZbsrmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZbsrmv(handle=%p, dirA=%d, transA=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, mb, nb, nnzb, alpha, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cuDoubleComplex* x_native;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuDoubleComplex*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZbsrmv(handle_native, dirA_native, transA_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y, 
   where A is a sparse matrix in extended BSR storage format, x and y are dense 
   vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSbsrxmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint sizeOfMask, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrMaskPtrA, jobject bsrRowPtrA, jobject bsrEndPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrMaskPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrMaskPtrA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrEndPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrEndPtrA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseSbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSbsrxmv(handle=%p, dirA=%d, transA=%d, sizeOfMask=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrMaskPtrA=%p, bsrRowPtrA=%p, bsrEndPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, sizeOfMask, mb, nb, nnzb, alpha, descrA, bsrValA, bsrMaskPtrA, bsrRowPtrA, bsrEndPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int sizeOfMask_native = 0;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* bsrValA_native;
    int* bsrMaskPtrA_native;
    int* bsrRowPtrA_native;
    int* bsrEndPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    float* x_native;
    float* beta_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    sizeOfMask_native = (int)sizeOfMask;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (float*)getPointer(env, bsrValA);
    bsrMaskPtrA_native = (int*)getPointer(env, bsrMaskPtrA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrEndPtrA_native = (int*)getPointer(env, bsrEndPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (float*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSbsrxmv(handle_native, dirA_native, transA_native, sizeOfMask_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrMaskPtrA_native, bsrRowPtrA_native, bsrEndPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrMaskPtrA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrEndPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDbsrxmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint sizeOfMask, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrMaskPtrA, jobject bsrRowPtrA, jobject bsrEndPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrMaskPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrMaskPtrA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrEndPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrEndPtrA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDbsrxmv(handle=%p, dirA=%d, transA=%d, sizeOfMask=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrMaskPtrA=%p, bsrRowPtrA=%p, bsrEndPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, sizeOfMask, mb, nb, nnzb, alpha, descrA, bsrValA, bsrMaskPtrA, bsrRowPtrA, bsrEndPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int sizeOfMask_native = 0;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* bsrValA_native;
    int* bsrMaskPtrA_native;
    int* bsrRowPtrA_native;
    int* bsrEndPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    double* x_native;
    double* beta_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    sizeOfMask_native = (int)sizeOfMask;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (double*)getPointer(env, bsrValA);
    bsrMaskPtrA_native = (int*)getPointer(env, bsrMaskPtrA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrEndPtrA_native = (int*)getPointer(env, bsrEndPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (double*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDbsrxmv(handle_native, dirA_native, transA_native, sizeOfMask_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrMaskPtrA_native, bsrRowPtrA_native, bsrEndPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrMaskPtrA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrEndPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCbsrxmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint sizeOfMask, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrMaskPtrA, jobject bsrRowPtrA, jobject bsrEndPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrMaskPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrMaskPtrA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrEndPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrEndPtrA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCbsrxmv(handle=%p, dirA=%d, transA=%d, sizeOfMask=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrMaskPtrA=%p, bsrRowPtrA=%p, bsrEndPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, sizeOfMask, mb, nb, nnzb, alpha, descrA, bsrValA, bsrMaskPtrA, bsrRowPtrA, bsrEndPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int sizeOfMask_native = 0;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* bsrValA_native;
    int* bsrMaskPtrA_native;
    int* bsrRowPtrA_native;
    int* bsrEndPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cuComplex* x_native;
    cuComplex* beta_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    sizeOfMask_native = (int)sizeOfMask;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuComplex*)getPointer(env, bsrValA);
    bsrMaskPtrA_native = (int*)getPointer(env, bsrMaskPtrA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrEndPtrA_native = (int*)getPointer(env, bsrEndPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (cuComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCbsrxmv(handle_native, dirA_native, transA_native, sizeOfMask_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrMaskPtrA_native, bsrRowPtrA_native, bsrEndPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrMaskPtrA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrEndPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZbsrxmvNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint transA, jint sizeOfMask, jint mb, jint nb, jint nnzb, jobject alpha, jobject descrA, jobject bsrValA, jobject bsrMaskPtrA, jobject bsrRowPtrA, jobject bsrEndPtrA, jobject bsrColIndA, jint blockDim, jobject x, jobject beta, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrMaskPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrMaskPtrA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrEndPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrEndPtrA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZbsrxmv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZbsrxmv(handle=%p, dirA=%d, transA=%d, sizeOfMask=%d, mb=%d, nb=%d, nnzb=%d, alpha=%p, descrA=%p, bsrValA=%p, bsrMaskPtrA=%p, bsrRowPtrA=%p, bsrEndPtrA=%p, bsrColIndA=%p, blockDim=%d, x=%p, beta=%p, y=%p)\n",
        handle, dirA, transA, sizeOfMask, mb, nb, nnzb, alpha, descrA, bsrValA, bsrMaskPtrA, bsrRowPtrA, bsrEndPtrA, bsrColIndA, blockDim, x, beta, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    cusparseOperation_t transA_native;
    int sizeOfMask_native = 0;
    int mb_native = 0;
    int nb_native = 0;
    int nnzb_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* bsrValA_native;
    int* bsrMaskPtrA_native;
    int* bsrRowPtrA_native;
    int* bsrEndPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cuDoubleComplex* x_native;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    transA_native = (cusparseOperation_t)transA;
    sizeOfMask_native = (int)sizeOfMask;
    mb_native = (int)mb;
    nb_native = (int)nb;
    nnzb_native = (int)nnzb;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuDoubleComplex*)getPointer(env, bsrValA);
    bsrMaskPtrA_native = (int*)getPointer(env, bsrMaskPtrA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrEndPtrA_native = (int*)getPointer(env, bsrEndPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    x_native = (cuDoubleComplex*)getPointer(env, x);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZbsrxmv(handle_native, dirA_native, transA_native, sizeOfMask_native, mb_native, nb_native, nnzb_native, alpha_native, descrA_native, bsrValA_native, bsrMaskPtrA_native, bsrRowPtrA_native, bsrEndPtrA_native, bsrColIndA_native, blockDim_native, x_native, beta_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // transA is primitive
    // sizeOfMask is primitive
    // mb is primitive
    // nb is primitive
    // nnzb is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrMaskPtrA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrEndPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // x is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Solution of triangular linear system op(A) * y = alpha * x, 
   where A is a sparse matrix in CSR storage format, x and y are dense vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseScsrsv_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrsv_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsv_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrsv_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseCcsrsv_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrsv_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
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
    Logger::log(LOG_TRACE, "Executing cusparseZcsrsv_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrsv_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrsv_solve");
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
    Logger::log(LOG_TRACE, "Executing cusparseScsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    float* x_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (float*)getPointer(env, x);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrsv_solve");
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
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsv_solve(handle=%p, transA=%d, m=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, y=%p)\n",
        handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    double* x_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (double*)getPointer(env, x);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
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
    cusparseOperation_t transA_native;
    int m_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuComplex* x_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuComplex*)getPointer(env, x);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
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
    cusparseOperation_t transA_native;
    int m_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuDoubleComplex* x_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrsv_solve(handle_native, transA_native, m_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Solution of triangular linear system op(A) * y = alpha * x, 
   where A is a sparse matrix in HYB storage format, x and y are dense vectors. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShybsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject descrA, jobject hybA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseShybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseShybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShybsv_analysis(handle=%p, transA=%d, descrA=%p, hybA=%p, info=%p)\n",
        handle, transA, descrA, hybA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShybsv_analysis(handle_native, transA_native, descrA_native, hybA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhybsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject descrA, jobject hybA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhybsv_analysis(handle=%p, transA=%d, descrA=%p, hybA=%p, info=%p)\n",
        handle, transA, descrA, hybA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhybsv_analysis(handle_native, transA_native, descrA_native, hybA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChybsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject descrA, jobject hybA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseChybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseChybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChybsv_analysis(handle=%p, transA=%d, descrA=%p, hybA=%p, info=%p)\n",
        handle, transA, descrA, hybA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChybsv_analysis(handle_native, transA_native, descrA_native, hybA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhybsv_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jobject descrA, jobject hybA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZhybsv_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhybsv_analysis(handle=%p, transA=%d, descrA=%p, hybA=%p, info=%p)\n",
        handle, transA, descrA, hybA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhybsv_analysis(handle_native, transA_native, descrA_native, hybA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShybsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jobject alpha, jobject descra, jobject hybA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descra == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descra' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseShybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShybsv_solve(handle=%p, trans=%d, alpha=%p, descra=%p, hybA=%p, info=%p, x=%p, y=%p)\n",
        handle, trans, alpha, descra, hybA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    float* alpha_native;
    cusparseMatDescr_t descra_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;
    float* x_native;
    float* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descra_native = (cusparseMatDescr_t)getNativePointerValue(env, descra);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (float*)getPointer(env, x);
    y_native = (float*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShybsv_solve(handle_native, trans_native, alpha_native, descra_native, hybA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descra is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChybsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jobject alpha, jobject descra, jobject hybA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descra == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descra' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseChybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChybsv_solve(handle=%p, trans=%d, alpha=%p, descra=%p, hybA=%p, info=%p, x=%p, y=%p)\n",
        handle, trans, alpha, descra, hybA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    cuComplex* alpha_native;
    cusparseMatDescr_t descra_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuComplex* x_native;
    cuComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descra_native = (cusparseMatDescr_t)getNativePointerValue(env, descra);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuComplex*)getPointer(env, x);
    y_native = (cuComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChybsv_solve(handle_native, trans_native, alpha_native, descra_native, hybA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descra is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhybsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jobject alpha, jobject descra, jobject hybA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descra == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descra' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhybsv_solve(handle=%p, trans=%d, alpha=%p, descra=%p, hybA=%p, info=%p, x=%p, y=%p)\n",
        handle, trans, alpha, descra, hybA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    double* alpha_native;
    cusparseMatDescr_t descra_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;
    double* x_native;
    double* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descra_native = (cusparseMatDescr_t)getNativePointerValue(env, descra);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (double*)getPointer(env, x);
    y_native = (double*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhybsv_solve(handle_native, trans_native, alpha_native, descra_native, hybA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descra is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhybsv_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint trans, jobject alpha, jobject descra, jobject hybA, jobject info, jobject x, jobject y)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descra == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descra' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZhybsv_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhybsv_solve(handle=%p, trans=%d, alpha=%p, descra=%p, hybA=%p, info=%p, x=%p, y=%p)\n",
        handle, trans, alpha, descra, hybA, info, x, y);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descra_native;
    cusparseHybMat_t hybA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuDoubleComplex* x_native;
    cuDoubleComplex* y_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descra_native = (cusparseMatDescr_t)getNativePointerValue(env, descra);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    y_native = (cuDoubleComplex*)getPointer(env, y);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhybsv_solve(handle_native, trans_native, alpha_native, descra_native, hybA_native, info_native, x_native, y_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descra is a read-only native pointer
    // hybA is a read-only native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // y is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Sparse Level 3 routines --- */
/** Description: Matrix-matrix multiplication C = alpha * op(A) * B  + beta * C, 
   where A is a sparse matrix, B and C are dense and usually tall matrices. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrmm");
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
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseScsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrmm(handle_native, transA_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrmm");
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
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseDcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrmm(handle_native, transA_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
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
    // ldb is primitive
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
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrmm(handle_native, transA_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrmmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrmm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
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
    // ldb is primitive
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
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrmm(handle=%p, transA=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transA, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrmm(handle_native, transA_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrmm2Native(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseScsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrmm2(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transa_native;
    cusparseOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    float* B_native;
    int ldb_native = 0;
    float* beta_native;
    float* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transa_native = (cusparseOperation_t)transa;
    transb_native = (cusparseOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    C_native = (float*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrmm2(handle_native, transa_native, transb_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrmm2Native(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseDcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrmm2(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transa_native;
    cusparseOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    double* B_native;
    int ldb_native = 0;
    double* beta_native;
    double* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transa_native = (cusparseOperation_t)transa;
    transb_native = (cusparseOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    C_native = (double*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrmm2(handle_native, transa_native, transb_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrmm2Native(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseCcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrmm2(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transa_native;
    cusparseOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuComplex* B_native;
    int ldb_native = 0;
    cuComplex* beta_native;
    cuComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transa_native = (cusparseOperation_t)transa;
    transb_native = (cusparseOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    C_native = (cuComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrmm2(handle_native, transa_native, transb_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrmm2Native(JNIEnv *env, jclass cls, jobject handle, jint transa, jint transb, jint m, jint n, jint k, jint nnz, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject B, jint ldb, jobject beta, jobject C, jint ldc)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (C == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'C' is null for cusparseZcsrmm2");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldc is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrmm2(handle=%p, transa=%d, transb=%d, m=%d, n=%d, k=%d, nnz=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, B=%p, ldb=%d, beta=%p, C=%p, ldc=%d)\n",
        handle, transa, transb, m, n, k, nnz, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transa_native;
    cusparseOperation_t transb_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    int nnz_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuDoubleComplex* B_native;
    int ldb_native = 0;
    cuDoubleComplex* beta_native;
    cuDoubleComplex* C_native;
    int ldc_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transa_native = (cusparseOperation_t)transa;
    transb_native = (cusparseOperation_t)transb;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    nnz_native = (int)nnz;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    C_native = (cuDoubleComplex*)getPointer(env, C);
    ldc_native = (int)ldc;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrmm2(handle_native, transa_native, transb_native, m_native, n_native, k_native, nnz_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, B_native, ldb_native, beta_native, C_native, ldc_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transa is primitive
    // transb is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // nnz is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // B is a native pointer
    // ldb is primitive
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // C is a native pointer
    // ldc is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}


/** Description: Solution of triangular linear system op(A) * Y = alpha * X, 
   with multiple right-hand-sides, where A is a sparse matrix in CSR storage 
   format, X and Y are dense and usually tall matrices. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrsm_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrsm_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrsm_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrsm_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsm_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrsm_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrsm_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrsm_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrsm_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrsm_1analysisNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint nnz, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // nnz is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsrsm_analysis");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrsm_analysis(handle=%p, transA=%d, m=%d, nnz=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, transA, m, nnz, descrA, csrValA, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int nnz_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    nnz_native = (int)nnz;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrsm_analysis(handle_native, transA_native, m_native, nnz_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // nnz is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrsm_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jint ldx, jobject y, jint ldy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseScsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrsm_solve(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, ldx=%d, y=%p, ldy=%d)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, ldx, y, ldy);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    float* x_native;
    int ldx_native = 0;
    float* y_native;
    int ldy_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (float*)getPointer(env, x);
    ldx_native = (int)ldx;
    y_native = (float*)getPointer(env, y);
    ldy_native = (int)ldy;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrsm_solve(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, ldx_native, y_native, ldy_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // ldx is primitive
    // y is a native pointer
    // ldy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrsm_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jint ldx, jobject y, jint ldy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseDcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrsm_solve(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, ldx=%d, y=%p, ldy=%d)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, ldx, y, ldy);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    double* x_native;
    int ldx_native = 0;
    double* y_native;
    int ldy_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (double*)getPointer(env, x);
    ldx_native = (int)ldx;
    y_native = (double*)getPointer(env, y);
    ldy_native = (int)ldy;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrsm_solve(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, ldx_native, y_native, ldy_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // ldx is primitive
    // y is a native pointer
    // ldy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrsm_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jint ldx, jobject y, jint ldy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseCcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrsm_solve(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, ldx=%d, y=%p, ldy=%d)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, ldx, y, ldy);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuComplex* x_native;
    int ldx_native = 0;
    cuComplex* y_native;
    int ldy_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuComplex*)getPointer(env, x);
    ldx_native = (int)ldx;
    y_native = (cuComplex*)getPointer(env, y);
    ldy_native = (int)ldy;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrsm_solve(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, ldx_native, y_native, ldy_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // ldx is primitive
    // y is a native pointer
    // ldy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrsm_1solveNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint m, jint n, jobject alpha, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject info, jobject x, jint ldx, jobject y, jint ldy)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldx is primitive
    if (y == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'y' is null for cusparseZcsrsm_solve");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldy is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrsm_solve(handle=%p, transA=%d, m=%d, n=%d, alpha=%p, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p, x=%p, ldx=%d, y=%p, ldy=%d)\n",
        handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, ldx, y, ldy);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;
    cuDoubleComplex* x_native;
    int ldx_native = 0;
    cuDoubleComplex* y_native;
    int ldy_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    ldx_native = (int)ldx;
    y_native = (cuDoubleComplex*)getPointer(env, y);
    ldy_native = (int)ldy;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrsm_solve(handle_native, transA_native, m_native, n_native, alpha_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, info_native, x_native, ldx_native, y_native, ldy_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer
    // x is a native pointer
    // ldx is primitive
    // y is a native pointer
    // ldy is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Preconditioners --- */
/** Description: Compute the incomplete-LU factorization with 0 fill-in (ILU0)
   based on the information in the opaque structure info that was obtained 
   from the analysis phase (csrsv_analysis). */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrilu0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrilu0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (float*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrilu0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrilu0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrilu0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (double*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrilu0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrilu0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrilu0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (cuComplex*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrilu0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrilu0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsrilu0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrilu0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (cuDoubleComplex*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrilu0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: Compute the incomplete-Cholesky factorization with 0 fill-in (IC0)
   based on the information in the opaque structure info that was obtained 
   from the analysis phase (csrsv_analysis). */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsric0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseScsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsric0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (float*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsric0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsric0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseDcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsric0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (double*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsric0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsric0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseCcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsric0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (cuComplex*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsric0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsric0Native(JNIEnv *env, jclass cls, jobject handle, jint trans, jint m, jobject descrA, jobject csrValA_ValM, jobject csrRowPtrA, jobject csrColIndA, jobject info)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // trans is primitive
    // m is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA_ValM == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA_ValM' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (info == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'info' is null for cusparseZcsric0");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsric0(handle=%p, trans=%d, m=%d, descrA=%p, csrValA_ValM=%p, csrRowPtrA=%p, csrColIndA=%p, info=%p)\n",
        handle, trans, m, descrA, csrValA_ValM, csrRowPtrA, csrColIndA, info);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t trans_native;
    int m_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_ValM_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseSolveAnalysisInfo_t info_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    trans_native = (cusparseOperation_t)trans;
    m_native = (int)m;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_ValM_native = (cuDoubleComplex*)getPointer(env, csrValA_ValM);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    info_native = (cusparseSolveAnalysisInfo_t)getNativePointerValue(env, info);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsric0(handle_native, trans_native, m_native, descrA_native, csrValA_ValM_native, csrRowPtrA_native, csrColIndA_native, info_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // trans is primitive
    // m is primitive
    // descrA is a read-only native pointer
    // csrValA_ValM is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // info is a read-only native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/**
 * <pre>
 * Description: Solution of tridiagonal linear system A * B = B,
   with multiple right-hand-sides. The coefficient matrix A is 
   composed of lower (dl), main (d) and upper (du) diagonals, and 
 * the right-hand-sides B are overwritten with the solution.
 * </pre>
 */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSgtsvNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseSgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseSgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseSgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseSgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgtsv(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    float* dl_native;
    float* d_native;
    float* du_native;
    float* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (float*)getPointer(env, dl);
    d_native = (float*)getPointer(env, d);
    du_native = (float*)getPointer(env, du);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSgtsv(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDgtsvNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseDgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseDgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseDgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseDgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgtsv(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    double* dl_native;
    double* d_native;
    double* du_native;
    double* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (double*)getPointer(env, dl);
    d_native = (double*)getPointer(env, d);
    du_native = (double*)getPointer(env, du);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDgtsv(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCgtsvNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseCgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseCgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseCgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseCgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgtsv(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* dl_native;
    cuComplex* d_native;
    cuComplex* du_native;
    cuComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (cuComplex*)getPointer(env, dl);
    d_native = (cuComplex*)getPointer(env, d);
    du_native = (cuComplex*)getPointer(env, du);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCgtsv(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZgtsvNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseZgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseZgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseZgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseZgtsv");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgtsv(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* dl_native;
    cuDoubleComplex* d_native;
    cuDoubleComplex* du_native;
    cuDoubleComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (cuDoubleComplex*)getPointer(env, dl);
    d_native = (cuDoubleComplex*)getPointer(env, d);
    du_native = (cuDoubleComplex*)getPointer(env, du);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZgtsv(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/**
 * <pre>
 * Description: Solution of tridiagonal linear system A * B = B,
   with multiple right-hand-sides. The coefficient matrix A is 
   composed of lower (dl), main (d) and upper (du) diagonals, and 
   the right-hand-sides B are overwritten with the solution. 
 * These routines do not use pivoting, using a combination of PCR and CR algorithm
 * </pre>
 */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSgtsv_1nopivotNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseSgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseSgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseSgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseSgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgtsv_nopivot(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    float* dl_native;
    float* d_native;
    float* du_native;
    float* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (float*)getPointer(env, dl);
    d_native = (float*)getPointer(env, d);
    du_native = (float*)getPointer(env, du);
    B_native = (float*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSgtsv_nopivot(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDgtsv_1nopivotNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseDgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseDgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseDgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseDgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgtsv_nopivot(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    double* dl_native;
    double* d_native;
    double* du_native;
    double* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (double*)getPointer(env, dl);
    d_native = (double*)getPointer(env, d);
    du_native = (double*)getPointer(env, du);
    B_native = (double*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDgtsv_nopivot(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCgtsv_1nopivotNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseCgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseCgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseCgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseCgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgtsv_nopivot(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* dl_native;
    cuComplex* d_native;
    cuComplex* du_native;
    cuComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (cuComplex*)getPointer(env, dl);
    d_native = (cuComplex*)getPointer(env, d);
    du_native = (cuComplex*)getPointer(env, du);
    B_native = (cuComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCgtsv_nopivot(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZgtsv_1nopivotNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject dl, jobject d, jobject du, jobject B, jint ldb)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseZgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseZgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseZgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (B == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'B' is null for cusparseZgtsv_nopivot");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // ldb is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgtsv_nopivot(handle=%p, m=%d, n=%d, dl=%p, d=%p, du=%p, B=%p, ldb=%d)\n",
        handle, m, n, dl, d, du, B, ldb);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* dl_native;
    cuDoubleComplex* d_native;
    cuDoubleComplex* du_native;
    cuDoubleComplex* B_native;
    int ldb_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    dl_native = (cuDoubleComplex*)getPointer(env, dl);
    d_native = (cuDoubleComplex*)getPointer(env, d);
    du_native = (cuDoubleComplex*)getPointer(env, du);
    B_native = (cuDoubleComplex*)getPointer(env, B);
    ldb_native = (int)ldb;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZgtsv_nopivot(handle_native, m_native, n_native, dl_native, d_native, du_native, B_native, ldb_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // B is a native pointer
    // ldb is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}


/**
 * <pre>
 * Description: Solution of a set of tridiagonal linear systems
   A * x = x, each with a single right-hand-side. The coefficient 
   matrices A are composed of lower (dl), main (d) and upper (du) 
   diagonals and stored separated by a batchStride, while the 
 * right-hand-sides x are also separated by a batchStride.
 * </pre>
 */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSgtsvStridedBatchNative(JNIEnv *env, jclass cls, jobject handle, jint m, jobject dl, jobject d, jobject du, jobject x, jint batchCount, jint batchStride)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseSgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseSgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseSgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseSgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // batchCount is primitive
    // batchStride is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSgtsvStridedBatch(handle=%p, m=%d, dl=%p, d=%p, du=%p, x=%p, batchCount=%d, batchStride=%d)\n",
        handle, m, dl, d, du, x, batchCount, batchStride);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    float* dl_native;
    float* d_native;
    float* du_native;
    float* x_native;
    int batchCount_native = 0;
    int batchStride_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    dl_native = (float*)getPointer(env, dl);
    d_native = (float*)getPointer(env, d);
    du_native = (float*)getPointer(env, du);
    x_native = (float*)getPointer(env, x);
    batchCount_native = (int)batchCount;
    batchStride_native = (int)batchStride;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSgtsvStridedBatch(handle_native, m_native, dl_native, d_native, du_native, x_native, batchCount_native, batchStride_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // x is a native pointer
    // batchCount is primitive
    // batchStride is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDgtsvStridedBatchNative(JNIEnv *env, jclass cls, jobject handle, jint m, jobject dl, jobject d, jobject du, jobject x, jint batchCount, jint batchStride)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseDgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseDgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseDgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseDgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // batchCount is primitive
    // batchStride is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDgtsvStridedBatch(handle=%p, m=%d, dl=%p, d=%p, du=%p, x=%p, batchCount=%d, batchStride=%d)\n",
        handle, m, dl, d, du, x, batchCount, batchStride);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    double* dl_native;
    double* d_native;
    double* du_native;
    double* x_native;
    int batchCount_native = 0;
    int batchStride_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    dl_native = (double*)getPointer(env, dl);
    d_native = (double*)getPointer(env, d);
    du_native = (double*)getPointer(env, du);
    x_native = (double*)getPointer(env, x);
    batchCount_native = (int)batchCount;
    batchStride_native = (int)batchStride;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDgtsvStridedBatch(handle_native, m_native, dl_native, d_native, du_native, x_native, batchCount_native, batchStride_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // x is a native pointer
    // batchCount is primitive
    // batchStride is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCgtsvStridedBatchNative(JNIEnv *env, jclass cls, jobject handle, jint m, jobject dl, jobject d, jobject du, jobject x, jint batchCount, jint batchStride)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseCgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseCgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseCgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseCgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // batchCount is primitive
    // batchStride is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCgtsvStridedBatch(handle=%p, m=%d, dl=%p, d=%p, du=%p, x=%p, batchCount=%d, batchStride=%d)\n",
        handle, m, dl, d, du, x, batchCount, batchStride);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    cuComplex* dl_native;
    cuComplex* d_native;
    cuComplex* du_native;
    cuComplex* x_native;
    int batchCount_native = 0;
    int batchStride_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    dl_native = (cuComplex*)getPointer(env, dl);
    d_native = (cuComplex*)getPointer(env, d);
    du_native = (cuComplex*)getPointer(env, du);
    x_native = (cuComplex*)getPointer(env, x);
    batchCount_native = (int)batchCount;
    batchStride_native = (int)batchStride;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCgtsvStridedBatch(handle_native, m_native, dl_native, d_native, du_native, x_native, batchCount_native, batchStride_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // x is a native pointer
    // batchCount is primitive
    // batchStride is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZgtsvStridedBatchNative(JNIEnv *env, jclass cls, jobject handle, jint m, jobject dl, jobject d, jobject du, jobject x, jint batchCount, jint batchStride)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    if (dl == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'dl' is null for cusparseZgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (d == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'd' is null for cusparseZgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (du == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'du' is null for cusparseZgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (x == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'x' is null for cusparseZgtsvStridedBatch");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // batchCount is primitive
    // batchStride is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZgtsvStridedBatch(handle=%p, m=%d, dl=%p, d=%p, du=%p, x=%p, batchCount=%d, batchStride=%d)\n",
        handle, m, dl, d, du, x, batchCount, batchStride);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    cuDoubleComplex* dl_native;
    cuDoubleComplex* d_native;
    cuDoubleComplex* du_native;
    cuDoubleComplex* x_native;
    int batchCount_native = 0;
    int batchStride_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    dl_native = (cuDoubleComplex*)getPointer(env, dl);
    d_native = (cuDoubleComplex*)getPointer(env, d);
    du_native = (cuDoubleComplex*)getPointer(env, du);
    x_native = (cuDoubleComplex*)getPointer(env, x);
    batchCount_native = (int)batchCount;
    batchStride_native = (int)batchStride;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZgtsvStridedBatch(handle_native, m_native, dl_native, d_native, du_native, x_native, batchCount_native, batchStride_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // dl is a native pointer
    // d is a native pointer
    // du is a native pointer
    // x is a native pointer
    // batchCount is primitive
    // batchStride is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Extra --- */
/** Description: This routine computes a sparse matrix that results from 
   multiplication of two sparse matrices. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseXcsrgemmNnzNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint transB, jint m, jint n, jint k, jobject descrA, jint nnzA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrRowPtrC, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseXcsrgemmNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcsrgemmNnz(handle=%p, transA=%d, transB=%d, m=%d, n=%d, k=%d, descrA=%p, nnzA=%d, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrRowPtrC=%p, nnzTotalDevHostPtr=%p)\n",
        handle, transA, transB, m, n, k, descrA, nnzA, csrRowPtrA, csrColIndA, descrB, nnzB, csrRowPtrB, csrColIndB, descrC, csrRowPtrC, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseOperation_t transB_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    int* csrRowPtrC_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    transB_native = (cusparseOperation_t)transB;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseXcsrgemmNnz(handle_native, transA_native, transB_native, m_native, n_native, k_native, descrA_native, nnzA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrRowPtrC_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrRowPtrC is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint transB, jint m, jint n, jint k, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseScsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrgemm(handle=%p, transA=%d, transB=%d, m=%d, n=%d, k=%d, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, transA, transB, m, n, k, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseOperation_t transB_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    float* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    float* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    transB_native = (cusparseOperation_t)transB;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (float*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (float*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrgemm(handle_native, transA_native, transB_native, m_native, n_native, k_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint transB, jint m, jint n, jint k, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseDcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrgemm(handle=%p, transA=%d, transB=%d, m=%d, n=%d, k=%d, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, transA, transB, m, n, k, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseOperation_t transB_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    double* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    double* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    transB_native = (cusparseOperation_t)transB;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (double*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (double*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrgemm(handle_native, transA_native, transB_native, m_native, n_native, k_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint transB, jint m, jint n, jint k, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseCcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrgemm(handle=%p, transA=%d, transB=%d, m=%d, n=%d, k=%d, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, transA, transB, m, n, k, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseOperation_t transB_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    cuComplex* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    cuComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    transB_native = (cusparseOperation_t)transB;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (cuComplex*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrgemm(handle_native, transA_native, transB_native, m_native, n_native, k_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrgemmNative(JNIEnv *env, jclass cls, jobject handle, jint transA, jint transB, jint m, jint n, jint k, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseZcsrgemm");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrgemm(handle=%p, transA=%d, transB=%d, m=%d, n=%d, k=%d, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, transA, transB, m, n, k, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseOperation_t transA_native;
    cusparseOperation_t transB_native;
    int m_native = 0;
    int n_native = 0;
    int k_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    cuDoubleComplex* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    cuDoubleComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    transA_native = (cusparseOperation_t)transA;
    transB_native = (cusparseOperation_t)transB;
    m_native = (int)m;
    n_native = (int)n;
    k_native = (int)k;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (cuDoubleComplex*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuDoubleComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrgemm(handle_native, transA_native, transB_native, m_native, n_native, k_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // transA is primitive
    // transB is primitive
    // m is primitive
    // n is primitive
    // k is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine computes a sparse matrix that results from 
   addition of two sparse matrices. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseXcsrgeamNnzNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jint nnzA, jobject csrRowPtrA, jobject csrColIndA, jobject descrB, jint nnzB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrRowPtrC, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseXcsrgeamNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcsrgeamNnz(handle=%p, m=%d, n=%d, descrA=%p, nnzA=%d, csrRowPtrA=%p, csrColIndA=%p, descrB=%p, nnzB=%d, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrRowPtrC=%p, nnzTotalDevHostPtr=%p)\n",
        handle, m, n, descrA, nnzA, csrRowPtrA, csrColIndA, descrB, nnzB, csrRowPtrB, csrColIndB, descrC, csrRowPtrC, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    int* csrRowPtrC_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseXcsrgeamNnz(handle_native, m_native, n_native, descrA_native, nnzA_native, csrRowPtrA_native, csrColIndA_native, descrB_native, nnzB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrRowPtrC_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrRowPtrC is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsrgeamNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject beta, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseScsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsrgeam(handle=%p, m=%d, n=%d, alpha=%p, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, beta=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, m, n, alpha, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, beta, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    float* alpha_native;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    float* beta_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    float* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    float* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (float*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (float*)beta_pointerData->getPointer(env);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (float*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (float*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsrgeam(handle_native, m_native, n_native, alpha_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, beta_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsrgeamNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject beta, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseDcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsrgeam(handle=%p, m=%d, n=%d, alpha=%p, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, beta=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, m, n, alpha, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, beta, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    double* alpha_native;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    double* beta_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    double* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    double* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (double*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (double*)beta_pointerData->getPointer(env);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (double*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (double*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsrgeam(handle_native, m_native, n_native, alpha_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, beta_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsrgeamNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject beta, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseCcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsrgeam(handle=%p, m=%d, n=%d, alpha=%p, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, beta=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, m, n, alpha, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, beta, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuComplex* beta_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    cuComplex* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    cuComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuComplex*)beta_pointerData->getPointer(env);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (cuComplex*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsrgeam(handle_native, m_native, n_native, alpha_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, beta_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsrgeamNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject alpha, jobject descrA, jint nnzA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject beta, jobject descrB, jint nnzB, jobject csrValB, jobject csrRowPtrB, jobject csrColIndB, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (alpha == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'alpha' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzA is primitive
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (beta == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'beta' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrB' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // nnzB is primitive
    if (csrValB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValB' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrB' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndB == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndB' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseZcsrgeam");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsrgeam(handle=%p, m=%d, n=%d, alpha=%p, descrA=%p, nnzA=%d, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, beta=%p, descrB=%p, nnzB=%d, csrValB=%p, csrRowPtrB=%p, csrColIndB=%p, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, m, n, alpha, descrA, nnzA, csrValA, csrRowPtrA, csrColIndA, beta, descrB, nnzB, csrValB, csrRowPtrB, csrColIndB, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cuDoubleComplex* alpha_native;
    cusparseMatDescr_t descrA_native;
    int nnzA_native = 0;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuDoubleComplex* beta_native;
    cusparseMatDescr_t descrB_native;
    int nnzB_native = 0;
    cuDoubleComplex* csrValB_native;
    int* csrRowPtrB_native;
    int* csrColIndB_native;
    cusparseMatDescr_t descrC_native;
    cuDoubleComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    PointerData *alpha_pointerData = initPointerData(env, alpha);
    if (alpha_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    alpha_native = (cuDoubleComplex*)alpha_pointerData->getPointer(env);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    nnzA_native = (int)nnzA;
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    PointerData *beta_pointerData = initPointerData(env, beta);
    if (beta_pointerData == NULL)
    {
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    beta_native = (cuDoubleComplex*)beta_pointerData->getPointer(env);
    descrB_native = (cusparseMatDescr_t)getNativePointerValue(env, descrB);
    nnzB_native = (int)nnzB;
    csrValB_native = (cuDoubleComplex*)getPointer(env, csrValB);
    csrRowPtrB_native = (int*)getPointer(env, csrRowPtrB);
    csrColIndB_native = (int*)getPointer(env, csrColIndB);
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuDoubleComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsrgeam(handle_native, m_native, n_native, alpha_native, descrA_native, nnzA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, beta_native, descrB_native, nnzB_native, csrValB_native, csrRowPtrB_native, csrColIndB_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    if (!releasePointerData(env, alpha_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrA is a read-only native pointer
    // nnzA is primitive
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    if (!releasePointerData(env, beta_pointerData, JNI_ABORT)) return JCUSPARSE_STATUS_INTERNAL_ERROR;
    // descrB is a read-only native pointer
    // nnzB is primitive
    // csrValB is a native pointer
    // csrRowPtrB is a native pointer
    // csrColIndB is a native pointer
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** --- Sparse Format Conversion --- */
/** Description: This routine finds the total number of non-zero elements and 
   the number of non-zero elements per row or column in the dense matrix A. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
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
    // lda is primitive
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseSnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzTotalDevHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* A_native;
    int lda_native = 0;
    int* nnzPerRowCol_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRowCol is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
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
    // lda is primitive
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseDnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzTotalDevHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* A_native;
    int lda_native = 0;
    int* nnzPerRowCol_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRowCol is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
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
    // lda is primitive
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseCnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzTotalDevHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* A_native;
    int lda_native = 0;
    int* nnzPerRowCol_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRowCol is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZnnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRowCol, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
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
    // lda is primitive
    if (nnzPerRowCol == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRowCol' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseZnnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZnnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRowCol=%p, nnzTotalDevHostPtr=%p)\n",
        handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    int* nnzPerRowCol_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRowCol_native = (int*)getPointer(env, nnzPerRowCol);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZnnz(handle_native, dirA_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRowCol_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRowCol is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a dense matrix to a sparse matrix 
   in the CSR storage format, using the information computed by the 
   nnz routine. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    float* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

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
    cusparseStatus_t jniResult_native = cusparseSdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    double* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

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
    cusparseStatus_t jniResult_native = cusparseDdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    cuComplex* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

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
    cusparseStatus_t jniResult_native = cusparseCdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZdense2csrNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdense2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    cuDoubleComplex* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

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
    cusparseStatus_t jniResult_native = cusparseZdense2csr(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in CSR storage format
   to a dense matrix. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    float* A_native;
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
    cusparseStatus_t jniResult_native = cusparseScsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    double* A_native;
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
    cusparseStatus_t jniResult_native = cusparseDcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuComplex* A_native;
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
    cusparseStatus_t jniResult_native = cusparseCcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsr2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2dense(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cuDoubleComplex* A_native;
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
    cusparseStatus_t jniResult_native = cusparseZcsr2dense(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a dense matrix to a sparse matrix 
   in the CSC storage format, using the information computed by the 
   nnz routine. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    float* A_native;
    int lda_native = 0;
    int* nnzPerCol_native;
    float* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;

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
    cusparseStatus_t jniResult_native = cusparseSdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerCol is a native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    double* A_native;
    int lda_native = 0;
    int* nnzPerCol_native;
    double* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;

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
    cusparseStatus_t jniResult_native = cusparseDdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerCol is a native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    cuComplex* A_native;
    int lda_native = 0;
    int* nnzPerCol_native;
    cuComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;

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
    cusparseStatus_t jniResult_native = cusparseCdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerCol is a native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZdense2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerCol, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdense2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive
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
    cuDoubleComplex* A_native;
    int lda_native = 0;
    int* nnzPerCol_native;
    cuDoubleComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;

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
    cusparseStatus_t jniResult_native = cusparseZdense2csc(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerCol_native, cscValA_native, cscRowIndA_native, cscColPtrA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerCol is a native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in CSC storage format
   to a dense matrix. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    float* A_native;
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
    cusparseStatus_t jniResult_native = cusparseScsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    double* A_native;
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
    cusparseStatus_t jniResult_native = cusparseDcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cuComplex* A_native;
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
    cusparseStatus_t jniResult_native = cusparseCcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsc2denseNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsc2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
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
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsc2dense(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, A=%p, lda=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cuDoubleComplex* A_native;
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
    cusparseStatus_t jniResult_native = cusparseZcsc2dense(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine compresses the indecis of rows or columns.
   It can be interpreted as a conversion from COO to CSR sparse storage
   format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseXcoo2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject cooRowInd, jint nnz, jint m, jobject csrRowPtr, jint idxBase)
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
    // nnz is primitive
    // m is primitive
    if (csrRowPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtr' is null for cusparseXcoo2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcoo2csr(handle=%p, cooRowInd=%p, nnz=%d, m=%d, csrRowPtr=%p, idxBase=%d)\n",
        handle, cooRowInd, nnz, m, csrRowPtr, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int* cooRowInd_native;
    int nnz_native = 0;
    int m_native = 0;
    int* csrRowPtr_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    cooRowInd_native = (int*)getPointer(env, cooRowInd);
    nnz_native = (int)nnz;
    m_native = (int)m;
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseXcoo2csr(handle_native, cooRowInd_native, nnz_native, m_native, csrRowPtr_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // cooRowInd is a native pointer
    // nnz is primitive
    // m is primitive
    // csrRowPtr is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine uncompresses the indecis of rows or columns.
   It can be interpreted as a conversion from CSR to COO sparse storage
   format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseXcsr2cooNative(JNIEnv *env, jclass cls, jobject handle, jobject csrRowPtr, jint nnz, jint m, jobject cooRowInd, jint idxBase)
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
    // nnz is primitive
    // m is primitive
    if (cooRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cooRowInd' is null for cusparseXcsr2coo");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcsr2coo(handle=%p, csrRowPtr=%p, nnz=%d, m=%d, cooRowInd=%p, idxBase=%d)\n",
        handle, csrRowPtr, nnz, m, cooRowInd, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int* csrRowPtr_native;
    int nnz_native = 0;
    int m_native = 0;
    int* cooRowInd_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    nnz_native = (int)nnz;
    m_native = (int)m;
    cooRowInd_native = (int*)getPointer(env, cooRowInd);
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseXcsr2coo(handle_native, csrRowPtr_native, nnz_native, m_native, cooRowInd_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // csrRowPtr is a native pointer
    // nnz is primitive
    // m is primitive
    // cooRowInd is a native pointer
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a matrix from CSR to CSC sparse 
   storage format. The resulting matrix can be re-interpreted as a 
   transpose of the original matrix in CSR storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jint nnz, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    // copyValues is primitive
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2csc(handle=%p, m=%d, n=%d, nnz=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, nnz, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    float* csrVal_native;
    int* csrRowPtr_native;
    int* csrColInd_native;
    float* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;
    cusparseAction_t copyValues_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    csrVal_native = (float*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (float*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (cusparseAction_t)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsr2csc(handle_native, m_native, n_native, nnz_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // nnz is primitive
    // csrVal is a native pointer
    // csrRowPtr is a native pointer
    // csrColInd is a native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer
    // copyValues is primitive
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jint nnz, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    // copyValues is primitive
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2csc(handle=%p, m=%d, n=%d, nnz=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, nnz, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    double* csrVal_native;
    int* csrRowPtr_native;
    int* csrColInd_native;
    double* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;
    cusparseAction_t copyValues_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    csrVal_native = (double*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (double*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (cusparseAction_t)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsr2csc(handle_native, m_native, n_native, nnz_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // nnz is primitive
    // csrVal is a native pointer
    // csrRowPtr is a native pointer
    // csrColInd is a native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer
    // copyValues is primitive
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jint nnz, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    // copyValues is primitive
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2csc(handle=%p, m=%d, n=%d, nnz=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, nnz, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    cuComplex* csrVal_native;
    int* csrRowPtr_native;
    int* csrColInd_native;
    cuComplex* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;
    cusparseAction_t copyValues_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    csrVal_native = (cuComplex*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (cuComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (cusparseAction_t)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsr2csc(handle_native, m_native, n_native, nnz_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // nnz is primitive
    // csrVal is a native pointer
    // csrRowPtr is a native pointer
    // csrColInd is a native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer
    // copyValues is primitive
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsr2cscNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jint nnz, jobject csrVal, jobject csrRowPtr, jobject csrColInd, jobject cscVal, jobject cscRowInd, jobject cscColPtr, jint copyValues, jint idxBase)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    // nnz is primitive
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
    // copyValues is primitive
    // idxBase is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2csc(handle=%p, m=%d, n=%d, nnz=%d, csrVal=%p, csrRowPtr=%p, csrColInd=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p, copyValues=%d, idxBase=%d)\n",
        handle, m, n, nnz, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    int nnz_native = 0;
    cuDoubleComplex* csrVal_native;
    int* csrRowPtr_native;
    int* csrColInd_native;
    cuDoubleComplex* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;
    cusparseAction_t copyValues_native;
    cusparseIndexBase_t idxBase_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    nnz_native = (int)nnz;
    csrVal_native = (cuDoubleComplex*)getPointer(env, csrVal);
    csrRowPtr_native = (int*)getPointer(env, csrRowPtr);
    csrColInd_native = (int*)getPointer(env, csrColInd);
    cscVal_native = (cuDoubleComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);
    copyValues_native = (cusparseAction_t)copyValues;
    idxBase_native = (cusparseIndexBase_t)idxBase;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsr2csc(handle_native, m_native, n_native, nnz_native, csrVal_native, csrRowPtr_native, csrColInd_native, cscVal_native, cscRowInd_native, cscColPtr_native, copyValues_native, idxBase_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // nnz is primitive
    // csrVal is a native pointer
    // csrRowPtr is a native pointer
    // csrColInd is a native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer
    // copyValues is primitive
    // idxBase is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a dense matrix to a sparse matrix 
   in HYB storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSdense2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseSdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseSdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseSdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSdense2hyb(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSdense2hyb(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDdense2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseDdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDdense2hyb(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDdense2hyb(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCdense2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseCdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseCdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseCdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCdense2hyb(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCdense2hyb(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZdense2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject A, jint lda, jobject nnzPerRow, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive
    if (nnzPerRow == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzPerRow' is null for cusparseZdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZdense2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZdense2hyb(handle=%p, m=%d, n=%d, descrA=%p, A=%p, lda=%d, nnzPerRow=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, A, lda, nnzPerRow, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;
    int* nnzPerRow_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;
    nnzPerRow_native = (int*)getPointer(env, nnzPerRow);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZdense2hyb(handle_native, m_native, n_native, descrA_native, A_native, lda_native, nnzPerRow_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // A is a native pointer
    // lda is primitive
    // nnzPerRow is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in HYB storage format
   to a dense matrix. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShyb2denseNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseShyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseShyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShyb2dense(handle=%p, descrA=%p, hybA=%p, A=%p, lda=%d)\n",
        handle, descrA, hybA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    float* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    A_native = (float*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShyb2dense(handle_native, descrA_native, hybA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhyb2denseNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseDhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhyb2dense(handle=%p, descrA=%p, hybA=%p, A=%p, lda=%d)\n",
        handle, descrA, hybA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    double* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    A_native = (double*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhyb2dense(handle_native, descrA_native, hybA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChyb2denseNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseChyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseChyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChyb2dense(handle=%p, descrA=%p, hybA=%p, A=%p, lda=%d)\n",
        handle, descrA, hybA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    A_native = (cuComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChyb2dense(handle_native, descrA_native, hybA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhyb2denseNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject A, jint lda)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (A == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'A' is null for cusparseZhyb2dense");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // lda is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhyb2dense(handle=%p, descrA=%p, hybA=%p, A=%p, lda=%d)\n",
        handle, descrA, hybA, A, lda);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuDoubleComplex* A_native;
    int lda_native = 0;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    A_native = (cuDoubleComplex*)getPointer(env, A);
    lda_native = (int)lda;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhyb2dense(handle_native, descrA_native, hybA_native, A_native, lda_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // A is a native pointer
    // lda is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in CSR storage format
   to a sparse matrix in HYB storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsr2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseScsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2hyb(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsr2hyb(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsr2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2hyb(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsr2hyb(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsr2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseCcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2hyb(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsr2hyb(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsr2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZcsr2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2hyb(handle=%p, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsr2hyb(handle_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in HYB storage format
   to a sparse matrix in CSR storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShyb2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseShyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShyb2csr(handle=%p, descrA=%p, hybA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, descrA, hybA, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShyb2csr(handle_native, descrA_native, hybA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhyb2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhyb2csr(handle=%p, descrA=%p, hybA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, descrA, hybA, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhyb2csr(handle_native, descrA_native, hybA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChyb2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseChyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChyb2csr(handle=%p, descrA=%p, hybA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, descrA, hybA, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChyb2csr(handle_native, descrA_native, hybA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhyb2csrNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZhyb2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhyb2csr(handle=%p, descrA=%p, hybA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p)\n",
        handle, descrA, hybA, csrValA, csrRowPtrA, csrColIndA);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhyb2csr(handle_native, descrA_native, hybA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in CSC storage format
   to a sparse matrix in HYB storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsc2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseScsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsc2hyb(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (float*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsc2hyb(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsc2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsc2hyb(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (double*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsc2hyb(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsc2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseCcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsc2hyb(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (cuComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsc2hyb(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsc2hybNative(JNIEnv *env, jclass cls, jobject handle, jint m, jint n, jobject descrA, jobject cscValA, jobject cscRowIndA, jobject cscColPtrA, jobject hybA, jint userEllWidth, jint partitionType)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscValA' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowIndA' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtrA' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZcsc2hyb");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // userEllWidth is primitive
    // partitionType is primitive

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsc2hyb(handle=%p, m=%d, n=%d, descrA=%p, cscValA=%p, cscRowIndA=%p, cscColPtrA=%p, hybA=%p, userEllWidth=%d, partitionType=%d)\n",
        handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, hybA, userEllWidth, partitionType);

    // Native variable declarations
    cusparseHandle_t handle_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* cscValA_native;
    int* cscRowIndA_native;
    int* cscColPtrA_native;
    cusparseHybMat_t hybA_native;
    int userEllWidth_native = 0;
    cusparseHybPartition_t partitionType_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    cscValA_native = (cuDoubleComplex*)getPointer(env, cscValA);
    cscRowIndA_native = (int*)getPointer(env, cscRowIndA);
    cscColPtrA_native = (int*)getPointer(env, cscColPtrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    userEllWidth_native = (int)userEllWidth;
    partitionType_native = (cusparseHybPartition_t)partitionType;

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsc2hyb(handle_native, m_native, n_native, descrA_native, cscValA_native, cscRowIndA_native, cscColPtrA_native, hybA_native, userEllWidth_native, partitionType_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // cscValA is a native pointer
    // cscRowIndA is a native pointer
    // cscColPtrA is a native pointer
    // hybA is a read-only native pointer
    // userEllWidth is primitive
    // partitionType is primitive

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in HYB storage format
   to a sparse matrix in CSC storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseShyb2cscNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject cscVal, jobject cscRowInd, jobject cscColPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseShyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseShyb2csc(handle=%p, descrA=%p, hybA=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p)\n",
        handle, descrA, hybA, cscVal, cscRowInd, cscColPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    float* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    cscVal_native = (float*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseShyb2csc(handle_native, descrA_native, hybA_native, cscVal_native, cscRowInd_native, cscColPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDhyb2cscNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject cscVal, jobject cscRowInd, jobject cscColPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseDhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDhyb2csc(handle=%p, descrA=%p, hybA=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p)\n",
        handle, descrA, hybA, cscVal, cscRowInd, cscColPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    double* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    cscVal_native = (double*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDhyb2csc(handle_native, descrA_native, hybA_native, cscVal_native, cscRowInd_native, cscColPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseChyb2cscNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject cscVal, jobject cscRowInd, jobject cscColPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseChyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseChyb2csc(handle=%p, descrA=%p, hybA=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p)\n",
        handle, descrA, hybA, cscVal, cscRowInd, cscColPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuComplex* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    cscVal_native = (cuComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseChyb2csc(handle_native, descrA_native, hybA_native, cscVal_native, cscRowInd_native, cscColPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZhyb2cscNative(JNIEnv *env, jclass cls, jobject handle, jobject descrA, jobject hybA, jobject cscVal, jobject cscRowInd, jobject cscColPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (hybA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'hybA' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscVal == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscVal' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscRowInd == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscRowInd' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (cscColPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'cscColPtr' is null for cusparseZhyb2csc");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZhyb2csc(handle=%p, descrA=%p, hybA=%p, cscVal=%p, cscRowInd=%p, cscColPtr=%p)\n",
        handle, descrA, hybA, cscVal, cscRowInd, cscColPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseMatDescr_t descrA_native;
    cusparseHybMat_t hybA_native;
    cuDoubleComplex* cscVal_native;
    int* cscRowInd_native;
    int* cscColPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    hybA_native = (cusparseHybMat_t)getNativePointerValue(env, hybA);
    cscVal_native = (cuDoubleComplex*)getPointer(env, cscVal);
    cscRowInd_native = (int*)getPointer(env, cscRowInd);
    cscColPtr_native = (int*)getPointer(env, cscColPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZhyb2csc(handle_native, descrA_native, hybA_native, cscVal_native, cscRowInd_native, cscColPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // descrA is a read-only native pointer
    // hybA is a read-only native pointer
    // cscVal is a native pointer
    // cscRowInd is a native pointer
    // cscColPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in CSR storage format
   to a sparse matrix in BSR storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseXcsr2bsrNnzNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject csrRowPtrA, jobject csrColIndA, jint blockDim, jobject descrC, jobject bsrRowPtrC, jobject nnzTotalDevHostPtr)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrC' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (nnzTotalDevHostPtr == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'nnzTotalDevHostPtr' is null for cusparseXcsr2bsrNnz");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseXcsr2bsrNnz(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, csrRowPtrA=%p, csrColIndA=%p, blockDim=%d, descrC=%p, bsrRowPtrC=%p, nnzTotalDevHostPtr=%p)\n",
        handle, dirA, m, n, descrA, csrRowPtrA, csrColIndA, blockDim, descrC, bsrRowPtrC, nnzTotalDevHostPtr);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    int* bsrRowPtrC_native;
    int* nnzTotalDevHostPtr_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    bsrRowPtrC_native = (int*)getPointer(env, bsrRowPtrC);
    nnzTotalDevHostPtr_native = (int*)getPointer(env, nnzTotalDevHostPtr);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseXcsr2bsrNnz(handle_native, dirA_native, m_native, n_native, descrA_native, csrRowPtrA_native, csrColIndA_native, blockDim_native, descrC_native, bsrRowPtrC_native, nnzTotalDevHostPtr_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // bsrRowPtrC is a native pointer
    // nnzTotalDevHostPtr is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseScsr2bsrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jint blockDim, jobject descrC, jobject bsrValC, jobject bsrRowPtrC, jobject bsrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValC' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrC' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndC' is null for cusparseScsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseScsr2bsr(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, blockDim=%d, descrC=%p, bsrValC=%p, bsrRowPtrC=%p, bsrColIndC=%p)\n",
        handle, dirA, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, blockDim, descrC, bsrValC, bsrRowPtrC, bsrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    float* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    float* bsrValC_native;
    int* bsrRowPtrC_native;
    int* bsrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (float*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    bsrValC_native = (float*)getPointer(env, bsrValC);
    bsrRowPtrC_native = (int*)getPointer(env, bsrRowPtrC);
    bsrColIndC_native = (int*)getPointer(env, bsrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseScsr2bsr(handle_native, dirA_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, blockDim_native, descrC_native, bsrValC_native, bsrRowPtrC_native, bsrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // bsrValC is a native pointer
    // bsrRowPtrC is a native pointer
    // bsrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDcsr2bsrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jint blockDim, jobject descrC, jobject bsrValC, jobject bsrRowPtrC, jobject bsrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValC' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrC' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndC' is null for cusparseDcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDcsr2bsr(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, blockDim=%d, descrC=%p, bsrValC=%p, bsrRowPtrC=%p, bsrColIndC=%p)\n",
        handle, dirA, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, blockDim, descrC, bsrValC, bsrRowPtrC, bsrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    double* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    double* bsrValC_native;
    int* bsrRowPtrC_native;
    int* bsrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (double*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    bsrValC_native = (double*)getPointer(env, bsrValC);
    bsrRowPtrC_native = (int*)getPointer(env, bsrRowPtrC);
    bsrColIndC_native = (int*)getPointer(env, bsrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDcsr2bsr(handle_native, dirA_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, blockDim_native, descrC_native, bsrValC_native, bsrRowPtrC_native, bsrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // bsrValC is a native pointer
    // bsrRowPtrC is a native pointer
    // bsrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCcsr2bsrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jint blockDim, jobject descrC, jobject bsrValC, jobject bsrRowPtrC, jobject bsrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValC' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrC' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndC' is null for cusparseCcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCcsr2bsr(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, blockDim=%d, descrC=%p, bsrValC=%p, bsrRowPtrC=%p, bsrColIndC=%p)\n",
        handle, dirA, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, blockDim, descrC, bsrValC, bsrRowPtrC, bsrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    cuComplex* bsrValC_native;
    int* bsrRowPtrC_native;
    int* bsrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    bsrValC_native = (cuComplex*)getPointer(env, bsrValC);
    bsrRowPtrC_native = (int*)getPointer(env, bsrRowPtrC);
    bsrColIndC_native = (int*)getPointer(env, bsrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCcsr2bsr(handle_native, dirA_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, blockDim_native, descrC_native, bsrValC_native, bsrRowPtrC_native, bsrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // bsrValC is a native pointer
    // bsrRowPtrC is a native pointer
    // bsrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZcsr2bsrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint m, jint n, jobject descrA, jobject csrValA, jobject csrRowPtrA, jobject csrColIndA, jint blockDim, jobject descrC, jobject bsrValC, jobject bsrRowPtrC, jobject bsrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // m is primitive
    // n is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValA' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrA' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndA' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValC' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrC' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndC' is null for cusparseZcsr2bsr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZcsr2bsr(handle=%p, dirA=%d, m=%d, n=%d, descrA=%p, csrValA=%p, csrRowPtrA=%p, csrColIndA=%p, blockDim=%d, descrC=%p, bsrValC=%p, bsrRowPtrC=%p, bsrColIndC=%p)\n",
        handle, dirA, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, blockDim, descrC, bsrValC, bsrRowPtrC, bsrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int m_native = 0;
    int n_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* csrValA_native;
    int* csrRowPtrA_native;
    int* csrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    cuDoubleComplex* bsrValC_native;
    int* bsrRowPtrC_native;
    int* bsrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    m_native = (int)m;
    n_native = (int)n;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    csrValA_native = (cuDoubleComplex*)getPointer(env, csrValA);
    csrRowPtrA_native = (int*)getPointer(env, csrRowPtrA);
    csrColIndA_native = (int*)getPointer(env, csrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    bsrValC_native = (cuDoubleComplex*)getPointer(env, bsrValC);
    bsrRowPtrC_native = (int*)getPointer(env, bsrRowPtrC);
    bsrColIndC_native = (int*)getPointer(env, bsrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZcsr2bsr(handle_native, dirA_native, m_native, n_native, descrA_native, csrValA_native, csrRowPtrA_native, csrColIndA_native, blockDim_native, descrC_native, bsrValC_native, bsrRowPtrC_native, bsrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // m is primitive
    // n is primitive
    // descrA is a read-only native pointer
    // csrValA is a native pointer
    // csrRowPtrA is a native pointer
    // csrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // bsrValC is a native pointer
    // bsrRowPtrC is a native pointer
    // bsrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

/** Description: This routine converts a sparse matrix in BSR storage format
   to a sparse matrix in CSR storage format. */
JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseSbsr2csrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint mb, jint nb, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseSbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseSbsr2csr(handle=%p, dirA=%d, mb=%d, nb=%d, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, dirA, mb, nb, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int mb_native = 0;
    int nb_native = 0;
    cusparseMatDescr_t descrA_native;
    float* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    float* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (float*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (float*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseSbsr2csr(handle_native, dirA_native, mb_native, nb_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseDbsr2csrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint mb, jint nb, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseDbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseDbsr2csr(handle=%p, dirA=%d, mb=%d, nb=%d, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, dirA, mb, nb, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int mb_native = 0;
    int nb_native = 0;
    cusparseMatDescr_t descrA_native;
    double* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    double* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (double*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (double*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseDbsr2csr(handle_native, dirA_native, mb_native, nb_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseCbsr2csrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint mb, jint nb, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseCbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseCbsr2csr(handle=%p, dirA=%d, mb=%d, nb=%d, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, dirA, mb, nb, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int mb_native = 0;
    int nb_native = 0;
    cusparseMatDescr_t descrA_native;
    cuComplex* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    cuComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuComplex*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseCbsr2csr(handle_native, dirA_native, mb_native, nb_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}

JNIEXPORT jint JNICALL Java_jcuda_jcusparse_JCusparse2_cusparseZbsr2csrNative(JNIEnv *env, jclass cls, jobject handle, jint dirA, jint mb, jint nb, jobject descrA, jobject bsrValA, jobject bsrRowPtrA, jobject bsrColIndA, jint blockDim, jobject descrC, jobject csrValC, jobject csrRowPtrC, jobject csrColIndC)
{
    // Null-checks for non-primitive arguments
    if (handle == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'handle' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    if (descrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrA' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrValA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrValA' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrRowPtrA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrRowPtrA' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (bsrColIndA == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'bsrColIndA' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    // blockDim is primitive
    if (descrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'descrC' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrValC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrValC' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrRowPtrC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrRowPtrC' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }
    if (csrColIndC == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", "Parameter 'csrColIndC' is null for cusparseZbsr2csr");
        return JCUSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Log message
    Logger::log(LOG_TRACE, "Executing cusparseZbsr2csr(handle=%p, dirA=%d, mb=%d, nb=%d, descrA=%p, bsrValA=%p, bsrRowPtrA=%p, bsrColIndA=%p, blockDim=%d, descrC=%p, csrValC=%p, csrRowPtrC=%p, csrColIndC=%p)\n",
        handle, dirA, mb, nb, descrA, bsrValA, bsrRowPtrA, bsrColIndA, blockDim, descrC, csrValC, csrRowPtrC, csrColIndC);

    // Native variable declarations
    cusparseHandle_t handle_native;
    cusparseDirection_t dirA_native;
    int mb_native = 0;
    int nb_native = 0;
    cusparseMatDescr_t descrA_native;
    cuDoubleComplex* bsrValA_native;
    int* bsrRowPtrA_native;
    int* bsrColIndA_native;
    int blockDim_native = 0;
    cusparseMatDescr_t descrC_native;
    cuDoubleComplex* csrValC_native;
    int* csrRowPtrC_native;
    int* csrColIndC_native;

    // Obtain native variable values
    handle_native = (cusparseHandle_t)getNativePointerValue(env, handle);
    dirA_native = (cusparseDirection_t)dirA;
    mb_native = (int)mb;
    nb_native = (int)nb;
    descrA_native = (cusparseMatDescr_t)getNativePointerValue(env, descrA);
    bsrValA_native = (cuDoubleComplex*)getPointer(env, bsrValA);
    bsrRowPtrA_native = (int*)getPointer(env, bsrRowPtrA);
    bsrColIndA_native = (int*)getPointer(env, bsrColIndA);
    blockDim_native = (int)blockDim;
    descrC_native = (cusparseMatDescr_t)getNativePointerValue(env, descrC);
    csrValC_native = (cuDoubleComplex*)getPointer(env, csrValC);
    csrRowPtrC_native = (int*)getPointer(env, csrRowPtrC);
    csrColIndC_native = (int*)getPointer(env, csrColIndC);

    // Native function call
    cusparseStatus_t jniResult_native = cusparseZbsr2csr(handle_native, dirA_native, mb_native, nb_native, descrA_native, bsrValA_native, bsrRowPtrA_native, bsrColIndA_native, blockDim_native, descrC_native, csrValC_native, csrRowPtrC_native, csrColIndC_native);

    // Write back native variable values
    // handle is a read-only native pointer
    // dirA is primitive
    // mb is primitive
    // nb is primitive
    // descrA is a read-only native pointer
    // bsrValA is a native pointer
    // bsrRowPtrA is a native pointer
    // bsrColIndA is a native pointer
    // blockDim is primitive
    // descrC is a read-only native pointer
    // csrValC is a native pointer
    // csrRowPtrC is a native pointer
    // csrColIndC is a native pointer

    // Return the result
    jint jniResult;
    jniResult = (jint)jniResult_native;
    return jniResult;
}



