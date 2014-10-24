#include "com_thalmic_myo_Hub.h"
#include "handle.h"
#include "jnidevicelistener.hpp"
#include <map>
#include <myo/myo.hpp>

std::map<jint, JniDeviceListener*> deviceListenerMap;
std::map<myo::Myo*, jobject> myoMap;

/*
 * Class:     com_thalmic_myo_Hub
 * Method:    initialize
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_thalmic_myo_Hub_initialize (JNIEnv *jenv, jobject thisObject, jstring applicationIdentifier) {
	setbuf(stdout, NULL);
	try {
		const char* mystring = jenv->GetStringUTFChars(applicationIdentifier, NULL);
		myo::Hub *hub = new myo::Hub(mystring);
		setHandle(jenv, thisObject, hub);
	} catch (const std::exception& e) {
		jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
		jenv->ThrowNew(exceptionClass, e.what());
	}
}

/*
 * Class:     com_thalmic_myo_Hub
 * Method:    waitForMyo
 * Signature: (I)Lcom/thalmic/myo/Myo;
 */
JNIEXPORT jobject JNICALL Java_com_thalmic_myo_Hub_waitForMyo(JNIEnv *jenv, jobject thisObject, jint timeout) {
	myo::Hub *hub = getHandle<myo::Hub>(jenv, thisObject);
	myo::Myo *myo = hub->waitForMyo(timeout);
	if (!myo) {
		return NULL;
	}
	jclass cls = jenv->FindClass("com/thalmic/myo/Myo");
	jmethodID initMethodID = jenv->GetMethodID(cls, "<init>", "()V");

	jobject javaMyoObject = jenv->NewGlobalRef(jenv->NewObject(cls, initMethodID));
	setHandle(jenv, javaMyoObject, myo);
	myoMap[myo] = javaMyoObject;
	return javaMyoObject;
}

/*
* Class:     com_thalmic_myo_Hub
* Method:    addListener
* Signature: (Lcom/thalmic/myo/DeviceListener;)V
*/
JNIEXPORT void JNICALL Java_com_thalmic_myo_Hub_addListener(JNIEnv *jenv, jobject thisObject, jobject javaDeviceListener) {
	myo::Hub *hub = getHandle<myo::Hub>(jenv, thisObject);
	if (hub != NULL) {
		jmethodID hashCodeMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "hashCode", "()I");
		jint javaDeviceListenerHashCode = jenv->CallIntMethod(javaDeviceListener, hashCodeMethodId);

		if (deviceListenerMap.count(javaDeviceListenerHashCode) == 0) {
			jobject newJavaObject = jenv->NewGlobalRef(javaDeviceListener);
			JniDeviceListener *jniDeviceListener = new JniDeviceListener(jenv, newJavaObject, myoMap);
			deviceListenerMap[javaDeviceListenerHashCode] = jniDeviceListener;
			hub->addListener(jniDeviceListener);
		}
	}
}

/*
* Class:     com_thalmic_myo_Hub
* Method:    removeListener
* Signature: (Lcom/thalmic/myo/DeviceListener;)V
*/
JNIEXPORT void JNICALL Java_com_thalmic_myo_Hub_removeListener(JNIEnv *jenv, jobject thisObject, jobject javaDeviceListener) {
	myo::Hub *hub = getHandle<myo::Hub>(jenv, thisObject);
	if (hub != NULL) {
		jmethodID hashCodeMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "hashCode", "()I");
		jint javaDeviceListenerHashCode = jenv->CallIntMethod(javaDeviceListener, hashCodeMethodId);

		JniDeviceListener *jniDeviceListener = deviceListenerMap[javaDeviceListenerHashCode];
		if (hub != NULL && jniDeviceListener != NULL) {
			hub->removeListener(jniDeviceListener);
			deviceListenerMap.erase(javaDeviceListenerHashCode);
		}
	}
}

/*
* Class:     com_thalmic_myo_Hub
* Method:    run
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_com_thalmic_myo_Hub_run(JNIEnv *jenv, jobject thisObject, jint duration) {
	try {
		myo::Hub *hub = getHandle<myo::Hub>(jenv, thisObject);
		if (hub != NULL) {
			hub->run(duration);
		}
	} catch (const std::exception& e) {
		jclass exceptionClass = jenv->FindClass("java/lang/RuntimeException");
		jenv->ThrowNew(exceptionClass, e.what());
	}
}

/*
* Class:     com_thalmic_myo_Hub
* Method:    runOnce
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_com_thalmic_myo_Hub_runOnce(JNIEnv *jenv, jobject thisObject, jint duration) {
	myo::Hub *hub = getHandle<myo::Hub>(jenv, thisObject);
	if (hub != NULL) {
		hub->runOnce(duration);
	}
}