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

package jcuda.jcusparse;

import jcuda.*;
import jcuda.runtime.cudaStream_t;

/**
 * Java bindings for CUSPARSE, the NVIDIA CUDA sparse matrix
 * BLAS library.
 */
public class JCusparse
{
    /**
     * The flag that indicates whether the native library has been
     * loaded
     */
    private static boolean initialized = false;

    /**
     * Whether a CudaException should be thrown if a method is about
     * to return a result code that is not 
     * cusparseStatus.CUSPARSE_STATUS_SUCCESS
     */
    private static boolean exceptionsEnabled = false;
    
    /* Private constructor to prevent instantiation */
    private JCusparse()
    {
    }

    // Initialize the native library.
    static
    {
        initialize();
    }
    
    /**
     * Initializes the native library. Note that this method
     * does not have to be called explicitly, since it will
     * be called automatically when this class is loaded.
     */
    public static void initialize()
    {

      if(!initialized) {
        String jnilib = com.github.fommil.jni.JniNamer.getJniName("jcusparse");
        String natives = System.getProperty("jcusparse", jnilib);
        com.github.fommil.jni.JniLoader.load(natives.split(","));
        initialized = true;
      }

    }

    /**
     * Set the specified log level for the JCusparse library.<br />
     * <br />
     * Currently supported log levels:
     * <br />
     * LOG_QUIET: Never print anything <br />
     * LOG_ERROR: Print error messages <br />
     * LOG_TRACE: Print a trace of all native function calls <br />
     *
     * @param logLevel The log level to use.
     */
    public static void setLogLevel(LogLevel logLevel)
    {
        setLogLevelNative(logLevel.ordinal());
    }

    private static native void setLogLevelNative(int logLevel);


    /**
     * Enables or disables exceptions. By default, the methods of this class
     * only set the {@link cusparseStatus} from the native methods. 
     * If exceptions are enabled, a CudaException with a detailed error 
     * message will be thrown if a method is about to set a result code 
     * that is not cusparseStatus.CUSPARSE_STATUS_SUCCESS
     * 
     * @param enabled Whether exceptions are enabled
     */
    public static void setExceptionsEnabled(boolean enabled)
    {
        exceptionsEnabled = enabled;
    }
    
    /**
     * If the given result is not cusparseStatus.CUSPARSE_STATUS_SUCCESS
     * and exceptions have been enabled, this method will throw a 
     * CudaException with an error message that corresponds to the
     * given result code. Otherwise, the given result is simply
     * returned.
     * 
     * @param result The result to check
     * @return The result that was given as the parameter
     * @throws CudaException If exceptions have been enabled and
     * the given result code is not cusparseStatus.CUSPARSE_STATUS_SUCCESS
     */
    private static int checkResult(int result)
    {
        if (exceptionsEnabled && result != 
            cusparseStatus.CUSPARSE_STATUS_SUCCESS)
        {
            throw new CudaException(cusparseStatus.stringFor(result));
        }
        return result;
    }

    /**
     * If the given result is <strong>equal</strong> to 
     * cusparseStatus.JCUSPARSE_STATUS_INTERNAL_ERROR
     * and exceptions have been enabled, this method will throw a 
     * CudaException with an error message that corresponds to the
     * given result code. Otherwise, the given result is simply
     * returned.<br />
     * <br />
     * This method is used for the functions that do not return
     * an error code, but a constant value, like a cusparseFillMode.
     * The respective functions may still return internal errors
     * from the JNI part.
     * 
     * @param result The result to check
     * @return The result that was given as the parameter
     * @throws CudaException If exceptions have been enabled and
     * the given result code is cusparseStatus.JCUSPARSE_STATUS_INTERNAL_ERROR
     */
    private static int checkForError(int result)
    {
        if (exceptionsEnabled && result == 
            cusparseStatus.JCUSPARSE_STATUS_INTERNAL_ERROR)
        {
            throw new CudaException(cusparseStatus.stringFor(result));
        }
        return result;
    }
    

    
    
    
    public static int cusparseCreate(cusparseHandle handle)
    {
        return checkResult(cusparseCreateNative(handle));
    }
    private native static int cusparseCreateNative(cusparseHandle handle);

    public static int cusparseDestroy(cusparseHandle handle)
    {
        return checkResult(cusparseDestroyNative(handle));
    }
    private native static int cusparseDestroyNative(cusparseHandle handle);

    public static int cusparseGetVersion(cusparseHandle handle, int[] version)
    {
        return checkResult(cusparseGetVersionNative(handle, version));
    }
    private native static int cusparseGetVersionNative(cusparseHandle handle, int[] version);

    public static int cusparseSetKernelStream(cusparseHandle handle, cudaStream_t streamId)
    {
        return checkResult(cusparseSetKernelStreamNative(handle, streamId));
    }
    private native static int cusparseSetKernelStreamNative(cusparseHandle handle, cudaStream_t streamId);

    // Returns a matrix descriptor initialized to: CUSPARSE_MATRIX_TYPE_GENERAL, CUSPARSE_INDEX_BASE_ZERO
    // Other fields are uninitialized                                   
    public static int cusparseCreateMatDescr(cusparseMatDescr descrA)
    {
        return checkResult(cusparseCreateMatDescrNative(descrA));
    }
    private native static int cusparseCreateMatDescrNative(cusparseMatDescr descrA);

    public static int cusparseDestroyMatDescr(cusparseMatDescr descrA)
    {
        return checkResult(cusparseDestroyMatDescrNative(descrA));
    }
    private native static int cusparseDestroyMatDescrNative(cusparseMatDescr descrA);

    public static int cusparseSetMatType(cusparseMatDescr descrA, int type)
    {
        return checkResult(cusparseSetMatTypeNative(descrA, type));
    }
    private native static int cusparseSetMatTypeNative(cusparseMatDescr descrA, int type);

    public static int cusparseGetMatType(cusparseMatDescr descrA)
    {
        return checkForError(cusparseGetMatTypeNative(descrA));
    }
    private native static int cusparseGetMatTypeNative(cusparseMatDescr descrA);

    // type must be _GENERAL, _SYMMETRIC, _HERMITIAN, or _TRIANGULAR
    public static int cusparseSetMatFillMode(cusparseMatDescr descrA, int fillMode)
    {
        return checkResult(cusparseSetMatFillModeNative(descrA, fillMode));
    }
    private native static int cusparseSetMatFillModeNative(cusparseMatDescr descrA, int fillMode);

    public static int cusparseGetMatFillMode(cusparseMatDescr descrA)
    {
        return checkForError(cusparseGetMatFillModeNative(descrA));
    }
    private native static int cusparseGetMatFillModeNative(cusparseMatDescr descrA);

    // type must be _TRIANGULAR
    public static int cusparseSetMatDiagType(cusparseMatDescr descrA, int diagType)
    {
        return checkResult(cusparseSetMatDiagTypeNative(descrA, diagType));
    }
    private native static int cusparseSetMatDiagTypeNative(cusparseMatDescr descrA, int diagType);

    public static int cusparseGetMatDiagType(cusparseMatDescr descrA)
    {
        return checkForError(cusparseGetMatDiagTypeNative(descrA));
    }
    private native static int cusparseGetMatDiagTypeNative(cusparseMatDescr descrA);

    public static int cusparseSetMatIndexBase(cusparseMatDescr descrA, int base)
    {
        return checkResult(cusparseSetMatIndexBaseNative(descrA, base));
    }
    private native static int cusparseSetMatIndexBaseNative(cusparseMatDescr descrA, int base);

    public static int cusparseGetMatIndexBase(cusparseMatDescr descrA)
    {
        return checkForError(cusparseGetMatIndexBaseNative(descrA));
    }
    private native static int cusparseGetMatIndexBaseNative(cusparseMatDescr descrA);

    public static int cusparseCreateSolveAnalysisInfo(cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseCreateSolveAnalysisInfoNative(info));
    }
    private native static int cusparseCreateSolveAnalysisInfoNative(cusparseSolveAnalysisInfo info);

    public static int cusparseDestroySolveAnalysisInfo(cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseDestroySolveAnalysisInfoNative(info));
    }
    private native static int cusparseDestroySolveAnalysisInfoNative(cusparseSolveAnalysisInfo info);

    /** Sparse Level 1 functions */
    //Adds a scalar multiple of compressed sparse vector to a 
    //full-storage vector 
    public static int cusparseSaxpyi(cusparseHandle handle, int nnz, float alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseSaxpyiNative(handle, nnz, alpha, xVal, xInd, y, idxBase));
    }
    private native static int cusparseSaxpyiNative(cusparseHandle handle, int nnz, float alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseDaxpyi(cusparseHandle handle, int nnz, double alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseDaxpyiNative(handle, nnz, alpha, xVal, xInd, y, idxBase));
    }
    private native static int cusparseDaxpyiNative(cusparseHandle handle, int nnz, double alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseCaxpyi(cusparseHandle handle, int nnz, cuComplex alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseCaxpyiNative(handle, nnz, alpha, xVal, xInd, y, idxBase));
    }
    private native static int cusparseCaxpyiNative(cusparseHandle handle, int nnz, cuComplex alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseZaxpyi(cusparseHandle handle, int nnz, cuDoubleComplex alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseZaxpyiNative(handle, nnz, alpha, xVal, xInd, y, idxBase));
    }
    private native static int cusparseZaxpyiNative(cusparseHandle handle, int nnz, cuDoubleComplex alpha, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    //Computes the dot product of a compressed sparse real vector 
    //with a full-storage real vector.
    public static int cusparseSdoti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, float[] resultHostPtr, int idxBase)
    {
        return checkResult(cusparseSdotiNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseSdotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, float[] resultHostPtr, int idxBase);

    public static int cusparseDdoti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, double[] resultHostPtr, int idxBase)
    {
        return checkResult(cusparseDdotiNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseDdotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, double[] resultHostPtr, int idxBase);

    //Computes the dot product of a compressed sparse complex vector with 
    //a full-storage complex vector.
    public static int cusparseCdoti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuComplex resultHostPtr, int idxBase)
    {
        return checkResult(cusparseCdotiNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseCdotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuComplex resultHostPtr, int idxBase);

    public static int cusparseZdoti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuDoubleComplex resultHostPtr, int idxBase)
    {
        return checkResult(cusparseZdotiNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseZdotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuDoubleComplex resultHostPtr, int idxBase);

    //Computes the conjugated dot product of a compressed sparse complex 
    //vector with a full-storage complex vector.
    public static int cusparseCdotci(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuComplex resultHostPtr, int idxBase)
    {
        return checkResult(cusparseCdotciNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseCdotciNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuComplex resultHostPtr, int idxBase);

    public static int cusparseZdotci(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuDoubleComplex resultHostPtr, int idxBase)
    {
        return checkResult(cusparseZdotciNative(handle, nnz, xVal, xInd, y, resultHostPtr, idxBase));
    }
    private native static int cusparseZdotciNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, cuDoubleComplex resultHostPtr, int idxBase);

    //Gathers a full-storage sparse vector's elements into compressed form.
    public static int cusparseSgthr(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseSgthrNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseSgthrNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseDgthr(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseDgthrNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseDgthrNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseCgthr(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseCgthrNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseCgthrNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseZgthr(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseZgthrNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseZgthrNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    //Gathers a non-zero elements from full-storage vector y into 
    //compressed sparse vector x, and replaces these elements in y by zeros.
    public static int cusparseSgthrz(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseSgthrzNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseSgthrzNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseDgthrz(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseDgthrzNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseDgthrzNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseCgthrz(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseCgthrzNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseCgthrzNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    public static int cusparseZgthrz(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase)
    {
        return checkResult(cusparseZgthrzNative(handle, nnz, y, xVal, xInd, idxBase));
    }
    private native static int cusparseZgthrzNative(cusparseHandle handle, int nnz, Pointer y, Pointer xVal, Pointer xInd, int idxBase);

    //Scatters elements od compressed sparse vector x into 
    //full-storage vector y.
    public static int cusparseSsctr(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseSsctrNative(handle, nnz, xVal, xInd, y, idxBase));
    }
    private native static int cusparseSsctrNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseDsctr(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseDsctrNative(handle, nnz, xVal, xInd, y, idxBase));
    }
    private native static int cusparseDsctrNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseCsctr(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseCsctrNative(handle, nnz, xVal, xInd, y, idxBase));
    }
    private native static int cusparseCsctrNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    public static int cusparseZsctr(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase)
    {
        return checkResult(cusparseZsctrNative(handle, nnz, xVal, xInd, y, idxBase));
    }
    private native static int cusparseZsctrNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, int idxBase);

    //Performs Givens rotation
    public static int cusparseSroti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, float c, float s, int idxBase)
    {
        return checkResult(cusparseSrotiNative(handle, nnz, xVal, xInd, y, c, s, idxBase));
    }
    private native static int cusparseSrotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, float c, float s, int idxBase);

    public static int cusparseDroti(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, double c, double s, int idxBase)
    {
        return checkResult(cusparseDrotiNative(handle, nnz, xVal, xInd, y, c, s, idxBase));
    }
    private native static int cusparseDrotiNative(cusparseHandle handle, int nnz, Pointer xVal, Pointer xInd, Pointer y, double c, double s, int idxBase);

    /** Sparse Level 2 functions */
    //Matrix-vector multiplication  y = alpha * op(A) * x  + beta * y, 
    //where A is a sparse matrix, x and y are dense vectors     
    public static int cusparseScsrmv(cusparseHandle handle, int transA, int m, int n, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, float beta, Pointer y)
    {
        return checkResult(cusparseScsrmvNative(handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y));
    }
    private native static int cusparseScsrmvNative(cusparseHandle handle, int transA, int m, int n, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, float beta, Pointer y);

    public static int cusparseDcsrmv(cusparseHandle handle, int transA, int m, int n, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, double beta, Pointer y)
    {
        return checkResult(cusparseDcsrmvNative(handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y));
    }
    private native static int cusparseDcsrmvNative(cusparseHandle handle, int transA, int m, int n, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, double beta, Pointer y);

    public static int cusparseCcsrmv(cusparseHandle handle, int transA, int m, int n, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, cuComplex beta, Pointer y)
    {
        return checkResult(cusparseCcsrmvNative(handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y));
    }
    private native static int cusparseCcsrmvNative(cusparseHandle handle, int transA, int m, int n, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, cuComplex beta, Pointer y);

    public static int cusparseZcsrmv(cusparseHandle handle, int transA, int m, int n, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, cuDoubleComplex beta, Pointer y)
    {
        return checkResult(cusparseZcsrmvNative(handle, transA, m, n, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, x, beta, y));
    }
    private native static int cusparseZcsrmvNative(cusparseHandle handle, int transA, int m, int n, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer x, cuDoubleComplex beta, Pointer y);

    //Solution of triangular linear system op(A) * y = alpha * x, 
    //where A is a sparse matrix, x and y are dense vectors     
    public static int cusparseScsrsv_analysis(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseScsrsv_analysisNative(handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info));
    }
    private native static int cusparseScsrsv_analysisNative(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info);

    public static int cusparseDcsrsv_analysis(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseDcsrsv_analysisNative(handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info));
    }
    private native static int cusparseDcsrsv_analysisNative(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info);

    public static int cusparseCcsrsv_analysis(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseCcsrsv_analysisNative(handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info));
    }
    private native static int cusparseCcsrsv_analysisNative(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info);

    public static int cusparseZcsrsv_analysis(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info)
    {
        return checkResult(cusparseZcsrsv_analysisNative(handle, transA, m, descrA, csrValA, csrRowPtrA, csrColIndA, info));
    }
    private native static int cusparseZcsrsv_analysisNative(cusparseHandle handle, int transA, int m, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info);

    public static int cusparseScsrsv_solve(cusparseHandle handle, int transA, int m, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y)
    {
        return checkResult(cusparseScsrsv_solveNative(handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y));
    }
    private native static int cusparseScsrsv_solveNative(cusparseHandle handle, int transA, int m, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y);

    public static int cusparseDcsrsv_solve(cusparseHandle handle, int transA, int m, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y)
    {
        return checkResult(cusparseDcsrsv_solveNative(handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y));
    }
    private native static int cusparseDcsrsv_solveNative(cusparseHandle handle, int transA, int m, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y);

    public static int cusparseCcsrsv_solve(cusparseHandle handle, int transA, int m, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y)
    {
        return checkResult(cusparseCcsrsv_solveNative(handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y));
    }
    private native static int cusparseCcsrsv_solveNative(cusparseHandle handle, int transA, int m, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y);

    public static int cusparseZcsrsv_solve(cusparseHandle handle, int transA, int m, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y)
    {
        return checkResult(cusparseZcsrsv_solveNative(handle, transA, m, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, info, x, y));
    }
    private native static int cusparseZcsrsv_solveNative(cusparseHandle handle, int transA, int m, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, cusparseSolveAnalysisInfo info, Pointer x, Pointer y);

    /** Sparse Level 3 functions */
    //Matrix-matrix multiplication C = alpha * op(A) * B  + beta * C, 
    //where A is a sparse matrix, B and C are dense matrices                 
    public static int cusparseScsrmm(cusparseHandle handle, int transA, int m, int n, int k, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, float beta, Pointer C, int ldc)
    {
        return checkResult(cusparseScsrmmNative(handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc));
    }
    private native static int cusparseScsrmmNative(cusparseHandle handle, int transA, int m, int n, int k, float alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, float beta, Pointer C, int ldc);

    public static int cusparseDcsrmm(cusparseHandle handle, int transA, int m, int n, int k, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, double beta, Pointer C, int ldc)
    {
        return checkResult(cusparseDcsrmmNative(handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc));
    }
    private native static int cusparseDcsrmmNative(cusparseHandle handle, int transA, int m, int n, int k, double alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, double beta, Pointer C, int ldc);

    public static int cusparseCcsrmm(cusparseHandle handle, int transA, int m, int n, int k, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, cuComplex beta, Pointer C, int ldc)
    {
        return checkResult(cusparseCcsrmmNative(handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc));
    }
    private native static int cusparseCcsrmmNative(cusparseHandle handle, int transA, int m, int n, int k, cuComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, cuComplex beta, Pointer C, int ldc);

    public static int cusparseZcsrmm(cusparseHandle handle, int transA, int m, int n, int k, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, cuDoubleComplex beta, Pointer C, int ldc)
    {
        return checkResult(cusparseZcsrmmNative(handle, transA, m, n, k, alpha, descrA, csrValA, csrRowPtrA, csrColIndA, B, ldb, beta, C, ldc));
    }
    private native static int cusparseZcsrmmNative(cusparseHandle handle, int transA, int m, int n, int k, cuDoubleComplex alpha, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer B, int ldb, cuDoubleComplex beta, Pointer C, int ldc);

    /** Sparse Format Conversion */
    public static int cusparseSnnz(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr)
    {
        return checkResult(cusparseSnnzNative(handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr));
    }
    private native static int cusparseSnnzNative(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr);

    public static int cusparseDnnz(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr)
    {
        return checkResult(cusparseDnnzNative(handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr));
    }
    private native static int cusparseDnnzNative(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr);

    public static int cusparseCnnz(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr)
    {
        return checkResult(cusparseCnnzNative(handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr));
    }
    private native static int cusparseCnnzNative(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr);

    public static int cusparseZnnz(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr)
    {
        return checkResult(cusparseZnnzNative(handle, dirA, m, n, descrA, A, lda, nnzPerRowCol, nnzHostPtr));
    }
    private native static int cusparseZnnzNative(cusparseHandle handle, int dirA, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRowCol, Pointer nnzHostPtr);

    //Dense to CSR                           
    public static int cusparseSdense2csr(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA)
    {
        return checkResult(cusparseSdense2csrNative(handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA));
    }
    private native static int cusparseSdense2csrNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA);

    public static int cusparseDdense2csr(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA)
    {
        return checkResult(cusparseDdense2csrNative(handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA));
    }
    private native static int cusparseDdense2csrNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA);

    public static int cusparseCdense2csr(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA)
    {
        return checkResult(cusparseCdense2csrNative(handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA));
    }
    private native static int cusparseCdense2csrNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA);

    public static int cusparseZdense2csr(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA)
    {
        return checkResult(cusparseZdense2csrNative(handle, m, n, descrA, A, lda, nnzPerRow, csrValA, csrRowPtrA, csrColIndA));
    }
    private native static int cusparseZdense2csrNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerRow, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA);

    //CSR to Dense                          
    public static int cusparseScsr2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda)
    {
        return checkResult(cusparseScsr2denseNative(handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda));
    }
    private native static int cusparseScsr2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda);

    public static int cusparseDcsr2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda)
    {
        return checkResult(cusparseDcsr2denseNative(handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda));
    }
    private native static int cusparseDcsr2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda);

    public static int cusparseCcsr2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda)
    {
        return checkResult(cusparseCcsr2denseNative(handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda));
    }
    private native static int cusparseCcsr2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda);

    public static int cusparseZcsr2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda)
    {
        return checkResult(cusparseZcsr2denseNative(handle, m, n, descrA, csrValA, csrRowPtrA, csrColIndA, A, lda));
    }
    private native static int cusparseZcsr2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer csrValA, Pointer csrRowPtrA, Pointer csrColIndA, Pointer A, int lda);

    //Dense to CSC                           
    public static int cusparseSdense2csc(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA)
    {
        return checkResult(cusparseSdense2cscNative(handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA));
    }
    private native static int cusparseSdense2cscNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA);

    public static int cusparseDdense2csc(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA)
    {
        return checkResult(cusparseDdense2cscNative(handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA));
    }
    private native static int cusparseDdense2cscNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA);

    public static int cusparseCdense2csc(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA)
    {
        return checkResult(cusparseCdense2cscNative(handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA));
    }
    private native static int cusparseCdense2cscNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA);

    public static int cusparseZdense2csc(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA)
    {
        return checkResult(cusparseZdense2cscNative(handle, m, n, descrA, A, lda, nnzPerCol, cscValA, cscRowIndA, cscColPtrA));
    }
    private native static int cusparseZdense2cscNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer A, int lda, Pointer nnzPerCol, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA);

    //CSC to Dense                          
    public static int cusparseScsc2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda)
    {
        return checkResult(cusparseScsc2denseNative(handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda));
    }
    private native static int cusparseScsc2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda);

    public static int cusparseDcsc2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda)
    {
        return checkResult(cusparseDcsc2denseNative(handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda));
    }
    private native static int cusparseDcsc2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda);

    public static int cusparseCcsc2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda)
    {
        return checkResult(cusparseCcsc2denseNative(handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda));
    }
    private native static int cusparseCcsc2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda);

    public static int cusparseZcsc2dense(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda)
    {
        return checkResult(cusparseZcsc2denseNative(handle, m, n, descrA, cscValA, cscRowIndA, cscColPtrA, A, lda));
    }
    private native static int cusparseZcsc2denseNative(cusparseHandle handle, int m, int n, cusparseMatDescr descrA, Pointer cscValA, Pointer cscRowIndA, Pointer cscColPtrA, Pointer A, int lda);

    //COO to CSR, CSR to COO                         
    public static int cusparseXcoo2csr(cusparseHandle handle, Pointer cooRowInd, int nnz, int m, Pointer csrRowPtr, int idxBase)
    {
        return checkResult(cusparseXcoo2csrNative(handle, cooRowInd, nnz, m, csrRowPtr, idxBase));
    }
    private native static int cusparseXcoo2csrNative(cusparseHandle handle, Pointer cooRowInd, int nnz, int m, Pointer csrRowPtr, int idxBase);

    public static int cusparseXcsr2coo(cusparseHandle handle, Pointer csrRowPtr, int nnz, int m, Pointer cooRowInd, int idxBase)
    {
        return checkResult(cusparseXcsr2cooNative(handle, csrRowPtr, nnz, m, cooRowInd, idxBase));
    }
    private native static int cusparseXcsr2cooNative(cusparseHandle handle, Pointer csrRowPtr, int nnz, int m, Pointer cooRowInd, int idxBase);

    //CSR to CSC  (transpose)       
    public static int cusparseScsr2csc(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase)
    {
        return checkResult(cusparseScsr2cscNative(handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase));
    }
    private native static int cusparseScsr2cscNative(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase);

    public static int cusparseDcsr2csc(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase)
    {
        return checkResult(cusparseDcsr2cscNative(handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase));
    }
    private native static int cusparseDcsr2cscNative(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase);

    public static int cusparseCcsr2csc(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase)
    {
        return checkResult(cusparseCcsr2cscNative(handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase));
    }
    private native static int cusparseCcsr2cscNative(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase);

    public static int cusparseZcsr2csc(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase)
    {
        return checkResult(cusparseZcsr2cscNative(handle, m, n, csrVal, csrRowPtr, csrColInd, cscVal, cscRowInd, cscColPtr, copyValues, idxBase));
    }
    private native static int cusparseZcsr2cscNative(cusparseHandle handle, int m, int n, Pointer csrVal, Pointer csrRowPtr, Pointer csrColInd, Pointer cscVal, Pointer cscRowInd, Pointer cscColPtr, int copyValues, int idxBase);


}
