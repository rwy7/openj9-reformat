/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ibm_cuda_CudaLinker */

#ifndef _Included_com_ibm_cuda_CudaLinker
#define _Included_com_ibm_cuda_CudaLinker
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_ibm_cuda_CudaLinker
 * Method:    add
 * Signature: (IJI[BLjava/lang/String;J)V
 */
void JNICALL Java_com_ibm_cuda_CudaLinker_add(JNIEnv*, jclass, jint, jlong, jint, jbyteArray, jstring, jlong);

/*
 * Class:     com_ibm_cuda_CudaLinker
 * Method:    complete
 * Signature: (IJ)[B
 */
jbyteArray JNICALL Java_com_ibm_cuda_CudaLinker_complete(JNIEnv*, jclass, jint, jlong);

/*
 * Class:     com_ibm_cuda_CudaLinker
 * Method:    create
 * Signature: (IJ)J
 */
jlong JNICALL Java_com_ibm_cuda_CudaLinker_create(JNIEnv*, jclass, jint, jlong);

/*
 * Class:     com_ibm_cuda_CudaLinker
 * Method:    destroy
 * Signature: (IJ)V
 */
void JNICALL Java_com_ibm_cuda_CudaLinker_destroy(JNIEnv*, jclass, jint, jlong);

#ifdef __cplusplus
}
#endif
#endif
