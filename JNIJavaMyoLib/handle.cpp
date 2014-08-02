#include <jni.h>
#include "handle.h"

jfieldID getHandleField(JNIEnv *jenv, jobject obj) {
	jclass c = jenv->GetObjectClass(obj);
	jfieldID nativeHandleFieldID = jenv->GetFieldID(c, "nativeHandle", "J");

	if (jenv->ExceptionCheck()) {
		jenv->Throw(jenv->ExceptionOccurred());
	} else {
		return nativeHandleFieldID;
	}
	return NULL;
}