#include "com_thalmic_myo_Myo.h"
#include "handle.h"
#include <myo/myo.hpp>

/*
 * Class:     com_thalmic_myo_Myo
 * Method:    requestRssi
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_requestRssi(JNIEnv *jenv, jobject thisObject) {
	myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
	try {
		myo->requestRssi();
	} catch (const std::exception &e) {
		jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
		jenv->ThrowNew(exceptionClass, e.what());
	}
}


/*
 * Class:     com_thalmic_myo_Myo
 * Method:    vibrate
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_vibrate(JNIEnv *jenv, jobject thisObject, jint vibrationType) {
	myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
	try {
		if (vibrationType == 1) {
			myo->vibrate(myo::Myo::vibrationShort);
		} else if (vibrationType == 2) {
			myo->vibrate(myo::Myo::vibrationMedium);
		} else if (vibrationType == 3) {
			myo->vibrate(myo::Myo::vibrationLong);
		}
	} catch (const std::exception &e) {
		jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
		jenv->ThrowNew(exceptionClass, e.what());
	}
}