#include <jni.h>
#include <iostream>

#ifndef _HANDLE_H_INCLUDED_
#define _HANDLE_H_INCLUDED_
extern jfieldID getHandleField(JNIEnv *, jobject );

/*
Used to retrieve the object that is represented by the Java object using the handle field
*/
template <typename T> 
T *getHandle(JNIEnv *jenv, jobject obj) {
	jlong handle = jenv->GetLongField(obj, getHandleField(jenv, obj));

	if (jenv->ExceptionCheck()) {
		jenv->Throw(jenv->ExceptionOccurred());
		return NULL;
	}

	return reinterpret_cast<T *>(handle);
}

template <typename T> 
void setHandle(JNIEnv *jenv, jobject obj, T *t) {
	try {
		jlong handle = reinterpret_cast<jlong>(t);
		jenv->SetLongField(obj, getHandleField(jenv, obj), handle);

		if (jenv->ExceptionCheck()) {
			jenv->Throw(jenv->ExceptionOccurred());
		}
	} catch (const std::exception &e) {
		jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
		jenv->ThrowNew(exceptionClass, e.what());
	}
}
#endif