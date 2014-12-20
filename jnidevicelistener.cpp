#include <jni.h>
#include <myo/myo.hpp>
#include <map>
#include "jnidevicelistener.hpp"
#include "handle.h"

JniDeviceListener::JniDeviceListener(JavaVM *javavm, jobject javaDeviceListener, std::map<myo::Myo*, jobject> myoMap) {
	this->javaDeviceListener = javaDeviceListener;
	this->javavm = javavm;
	this->myoMap = myoMap;
}

JNIEnv* JniDeviceListener::getEnvironmentFromVm() {
	JNIEnv *jenv;
	jint returnStatus = javavm->GetEnv(reinterpret_cast<void**>(&jenv), JNI_VERSION_1_6);
	if (returnStatus == JNI_OK) {
		return jenv;
	} 
	return NULL;
}

template<typename T>
jobject JniDeviceListener::createJavaObjectFromQuaternion(const myo::Quaternion<T>& quaternion) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jclass quaternionClass = jenv->FindClass("com/thalmic/myo/Quaternion");
	jmethodID quaternionInitMethodID = jenv->GetMethodID(quaternionClass, "<init>", "(FFFF)V");
	jobject javaQuaternionObject = jenv->NewObject(quaternionClass, quaternionInitMethodID, quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
	return javaQuaternionObject;
}

template<typename T>
jobject JniDeviceListener::createJavaObjectFromVector3(const myo::Vector3<T>& vector) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jclass vector3Class = jenv->FindClass("com/thalmic/myo/Vector3");
	jmethodID vector3InitMethodID = jenv->GetMethodID(vector3Class, "<init>", "(FFF)V");
	jobject javaVector3Object = jenv->NewObject(vector3Class, vector3InitMethodID, vector.x(), vector.y(), vector.z());
	return javaVector3Object;
}

jobject JniDeviceListener::createJavaObjectFromPose(myo::Pose pose) {
	std::string poseType;
	switch(pose.type()) {
		case myo::Pose::rest:
			poseType = "REST";
			break;
		case myo::Pose::fist:
			poseType = "FIST";
			break;
		case myo::Pose::waveIn:
			poseType = "WAVE_IN";
			break;
		case myo::Pose::waveOut:
			poseType = "WAVE_OUT";
			break;
		case myo::Pose::fingersSpread:
			poseType = "FINGERS_SPREAD";
			break;
		case myo::Pose::doubleTap:
			poseType = "DOUBLE_TAP";
			break;
		case myo::Pose::unknown:
		default:
			poseType = "UNKNOWN";
			break;
	};

	JNIEnv* jenv = getEnvironmentFromVm();
	jclass poseTypeClass = jenv->FindClass("com/thalmic/myo/enums/PoseType");
	jfieldID poseTypeFieldID = jenv->GetStaticFieldID(poseTypeClass,  poseType.c_str(), "Lcom/thalmic/myo/enums/PoseType;");
	jobject javaPoseTypeObject = jenv->GetStaticObjectField(poseTypeClass, poseTypeFieldID);

	jclass poseClass = jenv->FindClass("com/thalmic/myo/Pose");
	jmethodID poseInitMethodID = jenv->GetMethodID(poseClass, "<init>", "(Lcom/thalmic/myo/enums/PoseType;)V");
	jobject javaPoseObject = jenv->NewObject(poseClass, poseInitMethodID, javaPoseTypeObject);
	return javaPoseObject;
}

jobject JniDeviceListener::createJavaObjectFromFirmwareVersion(myo::FirmwareVersion firmwareVersion){
	JNIEnv* jenv = getEnvironmentFromVm();
	jclass firmwareVersionClass = jenv->FindClass("com/thalmic/myo/FirmwareVersion");
	jmethodID firmwareVersionInitMethodID = jenv->GetMethodID(firmwareVersionClass, "<init>", "(IIII)V");
	jobject javafirmwareVersionObject = jenv->NewObject(firmwareVersionClass, firmwareVersionInitMethodID, firmwareVersion.firmwareVersionMajor, firmwareVersion.firmwareVersionMinor, firmwareVersion.firmwareVersionPatch, firmwareVersion.firmwareVersionHardwareRev);
	return javafirmwareVersionObject;
}

jobject JniDeviceListener::createJavaObjectFromArm(myo::Arm arm){
	std::string armType;
	switch(arm) {
		case myo::armLeft:
			armType = "ARM_LEFT";
			break;
		case myo::armRight:
			armType = "ARM_RIGHT";
			break;
		case myo::armUnknown:
		default:
			armType = "UNKNOWN";
			break;
	};

	JNIEnv* jenv = getEnvironmentFromVm();
	jclass armClass = jenv->FindClass("com/thalmic/myo/enums/Arm");
	jfieldID armFieldID = jenv->GetStaticFieldID(armClass,  armType.c_str(), "Lcom/thalmic/myo/enums/Arm;");
	jobject javaArmObject = jenv->GetStaticObjectField(armClass, armFieldID);

	return javaArmObject;
}

jobject JniDeviceListener::createJavaObjectFromXDirection(myo::XDirection xDirection) {
	std::string xDirectionType;
	switch(xDirection) {
		case myo::xDirectionTowardWrist:
			xDirectionType = "X_DIRECTION_TOWARDS_WRIST";
			break;
		case myo::xDirectionTowardElbow:
			xDirectionType = "X_DIRECTION_TOWARDS_ELBOW";
			break;
		case myo::xDirectionUnknown:
		default:
			xDirectionType = "X_DIRECTION_UNKNOWN";
			break;
	};

	JNIEnv* jenv = getEnvironmentFromVm();
	jclass xDirectionClass = jenv->FindClass("com/thalmic/myo/enums/XDirection");
	jfieldID xDirectionFieldID = jenv->GetStaticFieldID(xDirectionClass,  xDirectionType.c_str(), "Lcom/thalmic/myo/enums/XDirection;");
	jobject javaXDirectionObject = jenv->GetStaticObjectField(xDirectionClass, xDirectionFieldID);

	return javaXDirectionObject;
}

void JniDeviceListener::onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaFirmwareVersion = createJavaObjectFromFirmwareVersion(firmwareVersion);

	jmethodID onPairMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onPair", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/FirmwareVersion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onPairMethodId, javaMyo, timestamp, javaFirmwareVersion);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onUnpair(myo::Myo* myo, uint64_t timestamp) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	
	jmethodID onUnpairMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onUnpair", "(Lcom/thalmic/myo/Myo;J)V");
	jenv->CallVoidMethod(javaDeviceListener, onUnpairMethodId, javaMyo, timestamp);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaFirmwareVersion = createJavaObjectFromFirmwareVersion(firmwareVersion);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onConnect", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/FirmwareVersion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp, javaFirmwareVersion);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onDisconnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onDisconnect", "(Lcom/thalmic/myo/Myo;J)V");
	jenv->CallVoidMethod(javaDeviceListener, onDisconnectMethodId, javaMyo, timestamp);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaArm = createJavaObjectFromArm(arm);
	jobject javaXDirection = createJavaObjectFromXDirection(xDirection);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onArmSync", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/enums/Arm;Lcom/thalmic/myo/enums/XDirection;)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp, javaArm, javaXDirection);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onArmUnsync", "(Lcom/thalmic/myo/Myo;J)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onUnlock(myo::Myo* myo, uint64_t timestamp) {
        JNIEnv* jenv = getEnvironmentFromVm();
        jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

        jmethodID onUnlockMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onUnlock", "(Lcom/thalmic/myo/Myo;J)V");
        jenv->CallVoidMethod(javaDeviceListener, onUnlockMethodId, javaMyo, timestamp);
        jthrowable thrownException = jenv->ExceptionOccurred();
        if (thrownException) {
                jenv->Throw(thrownException);
        }
}

void JniDeviceListener::onLock(myo::Myo* myo, uint64_t timestamp) {
        JNIEnv* jenv = getEnvironmentFromVm();
        jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

        jmethodID onLockMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onLock", "(Lcom/thalmic/myo/Myo;J)V");
        jenv->CallVoidMethod(javaDeviceListener, onLockMethodId, javaMyo, timestamp);
        jthrowable thrownException = jenv->ExceptionOccurred();
        if (thrownException) {
                jenv->Throw(thrownException);
        }
}

void JniDeviceListener::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaPose = createJavaObjectFromPose(pose);

	jmethodID onPoseMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onPose", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Pose;)V");
	jenv->CallVoidMethod(javaDeviceListener, onPoseMethodId, javaMyo, timestamp, javaPose);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quaternion) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaQuaternionObject = createJavaObjectFromQuaternion(quaternion);

	jmethodID onOrientationDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onOrientationData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Quaternion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onOrientationDataMethodId, javaMyo, timestamp, javaQuaternionObject);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaVector3Object = createJavaObjectFromVector3(accel);

	jmethodID onAccelerometerDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onAccelerometerData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Vector3;)V");
	jenv->CallVoidMethod(javaDeviceListener, onAccelerometerDataMethodId, javaMyo, timestamp, javaVector3Object);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaVector3Object = createJavaObjectFromVector3(gyro);

	jmethodID onGyroscopeDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onGyroscopeData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Vector3;)V");
	jenv->CallVoidMethod(javaDeviceListener, onGyroscopeDataMethodId, javaMyo, timestamp, javaVector3Object);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onRssiMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onRssi", "(Lcom/thalmic/myo/Myo;JI)V");
	jenv->CallVoidMethod(javaDeviceListener, onRssiMethodId, javaMyo, timestamp, rssi);
	jthrowable thrownException = jenv->ExceptionOccurred();
	if (thrownException) {
		jenv->Throw(thrownException);
	}
}

void JniDeviceListener::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) {
	std::cout << "CAlling onEmgData";
        JNIEnv* jenv = getEnvironmentFromVm();
        jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

        jmethodID onEmgMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onEmgData", "(Lcom/thalmic/myo/Myo;J[B)V");
        jenv->CallVoidMethod(javaDeviceListener, onEmgMethodId, javaMyo, timestamp, emg);
        jthrowable thrownException = jenv->ExceptionOccurred();
        if (thrownException) {
                jenv->Throw(thrownException);
        }
}

jobject JniDeviceListener::createOrRetrieveMyoJavaObject(myo::Myo *myo) {
	JNIEnv* jenv = getEnvironmentFromVm();
	jobject javaMyoObject = JniDeviceListener::myoMap[myo];
	if (javaMyoObject == NULL) {
		jclass cls = jenv->FindClass("com/thalmic/myo/Myo");
		jmethodID initMethodID = jenv->GetMethodID(cls, "<init>", "()V");

		jobject javaMyoObject = jenv->NewGlobalRef(jenv->NewObject(cls, initMethodID))	;
		setHandle(jenv, javaMyoObject, myo);

		myoMap[myo] = javaMyoObject;
	}
	return javaMyoObject;
}
