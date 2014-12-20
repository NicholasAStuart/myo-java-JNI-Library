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

/*
 * Class:     com_thalmic_myo_Myo
 * Method:    unlock
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_unlock(JNIEnv *jenv, jobject thisObject, jint unlockType) {
	myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
        try {
                if (unlockType == 1) {
                        myo->unlock(myo::Myo::unlockTimed);
                } else if (unlockType == 2) {
                        myo->unlock(myo::Myo::unlockHold);
                }
        } catch (const std::exception &e) {
                jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
                jenv->ThrowNew(exceptionClass, e.what());
        }
}

/*
 * Class:     com_thalmic_myo_Myo
 * Method:    lock
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_lock(JNIEnv *jenv, jobject thisObject) {
        myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
        try {
                myo->lock();
        } catch (const std::exception &e) {
                jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
                jenv->ThrowNew(exceptionClass, e.what());
        }
}

/*
 * Class:     com_thalmic_myo_Myo
 * Method:    notifyUserAction
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_notifyUserAction(JNIEnv *jenv, jobject thisObject) {
        myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
        try {
                myo->notifyUserAction();
        } catch (const std::exception &e) {
                jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
                jenv->ThrowNew(exceptionClass, e.what());
        }
}

JNIEXPORT void JNICALL Java_com_thalmic_myo_Myo_setStreamEmg(JNIEnv *jenv, jobject thisObject, jint streamEmgType) {
        myo::Myo *myo = getHandle<myo::Myo>(jenv, thisObject);
        try {
                if (streamEmgType == 1) {
                        myo->setStreamEmg(myo::Myo::streamEmgDisabled);
                } else if (streamEmgType == 2) {
                        myo->setStreamEmg(myo::Myo::streamEmgEnabled);
                }
        } catch (const std::exception &e) {
                jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
                jenv->ThrowNew(exceptionClass, e.what());
        }
}
