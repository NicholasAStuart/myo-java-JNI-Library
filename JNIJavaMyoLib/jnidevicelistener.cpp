#include <jni.h>
#include <myo/myo.hpp>
#include <map>
#include "jnidevicelistener.hpp"
#include "handle.h"

JniDeviceListener::JniDeviceListener(JNIEnv *jenv, jobject javaDeviceListener, std::map<myo::Myo*, jobject> myoMap) {
	this->javaDeviceListener = javaDeviceListener;
	this->jenv = jenv;
	this->myoMap = myoMap;
}

template<typename T>
jobject JniDeviceListener::createJavaObjectFromQuaternion(const myo::Quaternion<T>& quaternion) {
	jclass quaternionClass = jenv->FindClass("com/thalmic/myo/Quaternion");
	jmethodID quaternionInitMethodID = jenv->GetMethodID(quaternionClass, "<init>", "(FFFF)V");
	jobject javaQuaternionObject = jenv->NewObject(quaternionClass, quaternionInitMethodID, quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
	return javaQuaternionObject;
}

template<typename T>
jobject JniDeviceListener::createJavaObjectFromVector3(const myo::Vector3<T>& vector) {
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
		case myo::Pose::reserved1:
			poseType = "RESERVED_1";
			break;
		case myo::Pose::thumbToPinky:
			poseType = "THUMB_TO_PINKY";
			break;
		case myo::Pose::unknown:
		default:
			poseType = "UNKNOWN";
			break;
	};

	jclass poseTypeClass = jenv->FindClass("com/thalmic/myo/enums/PoseType");
	jfieldID poseTypeFieldID = jenv->GetStaticFieldID(poseTypeClass,  poseType.c_str(), "Lcom/thalmic/myo/enums/PoseType;");
	jobject javaPoseTypeObject = jenv->GetStaticObjectField(poseTypeClass, poseTypeFieldID);

	jclass poseClass = jenv->FindClass("com/thalmic/myo/Pose");
	jmethodID poseInitMethodID = jenv->GetMethodID(poseClass, "<init>", "(Lcom/thalmic/myo/enums/PoseType;)V");
	jobject javaPoseObject = jenv->NewObject(poseClass, poseInitMethodID, javaPoseTypeObject);
	return javaPoseObject;
}

jobject JniDeviceListener::createJavaObjectFromFirmwareVersion(myo::FirmwareVersion firmwareVersion){
	jclass firmwareVersionClass = jenv->FindClass("com/thalmic/myo/FirmwareVersion");
	jmethodID firmwareVersionInitMethodID = jenv->GetMethodID(firmwareVersionClass, "<init>", "(IIII)V");
	jobject javafirmwareVersionObject = jenv->NewObject(firmwareVersionClass, firmwareVersionInitMethodID, firmwareVersion.firmwareVersionMajor, firmwareVersion.firmwareVersionMinor, firmwareVersion.firmwareVersionPatch, firmwareVersion.firmwareVersionHardwareRev);
	return javafirmwareVersionObject;
}

jobject JniDeviceListener::createJavaObjectFromArm(myo::Arm arm){
	std::string armType;
	switch(arm) {
		case myo::Arm::armLeft:
			armType = "ARM_LEFT";
			break;
		case myo::Arm::armRight:
			armType = "ARM_LEFT";
			break;
		case myo::Arm::armUnknown:
		default:
			armType = "UNKNOWN";
			break;
	};

	jclass armClass = jenv->FindClass("com/thalmic/myo/enums/Arm");
	jfieldID armFieldID = jenv->GetStaticFieldID(armClass,  armType.c_str(), "Lcom/thalmic/myo/enums/Arm;");
	jobject javaArmObject = jenv->GetStaticObjectField(armClass, armFieldID);

	return javaArmObject;
}

jobject JniDeviceListener::createJavaObjectFromXDirection(myo::XDirection xDirection) {
	std::string xDirectionType;
	switch(xDirection) {
		case myo::XDirection::xDirectionTowardWrist:
			xDirectionType = "X_DIRECTION_TOWARDS_WRIST";
			break;
		case myo::XDirection::xDirectionTowardElbow:
			xDirectionType = "X_DIRECTION_TOWARDS_ELBOW";
			break;
		case myo::XDirection::xDirectionUnknown:
		default:
			xDirectionType = "X_DIRECTION_UNKNOWN";
			break;
	};

	jclass xDirectionClass = jenv->FindClass("com/thalmic/myo/enums/XDirection");
	jfieldID xDirectionFieldID = jenv->GetStaticFieldID(xDirectionClass,  xDirectionType.c_str(), "Lcom/thalmic/myo/enums/XDirection;");
	jobject javaXDirectionObject = jenv->GetStaticObjectField(xDirectionClass, xDirectionFieldID);

	return javaXDirectionObject;
}

void JniDeviceListener::onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaFirmwareVersion = createJavaObjectFromFirmwareVersion(firmwareVersion);

	jmethodID onPairMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onPair", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/FirmwareVersion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onPairMethodId, javaMyo, timestamp, javaFirmwareVersion);
}

void JniDeviceListener::onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaFirmwareVersion = createJavaObjectFromFirmwareVersion(firmwareVersion);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onConnect", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/FirmwareVersion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp, javaFirmwareVersion);
}

void JniDeviceListener::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onDisconnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onDisconnect", "(Lcom/thalmic/myo/Myo;J)V");
	jenv->CallVoidMethod(javaDeviceListener, onDisconnectMethodId, javaMyo, timestamp);
}

void JniDeviceListener::onArmRecognized(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaArm = createJavaObjectFromArm(arm);
	jobject javaXDirection = createJavaObjectFromXDirection(xDirection);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onArmRecognized", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/enums/Arm;Lcom/thalmic/myo/enums/XDirection;)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp, javaArm, javaXDirection);
}

void JniDeviceListener::onArmLost(myo::Myo* myo, uint64_t timestamp) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onArmLost", "(Lcom/thalmic/myo/Myo;J)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp);
}

void JniDeviceListener::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaPose = createJavaObjectFromPose(pose);

	jmethodID onPoseMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onPose", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Pose;)V");
	jenv->CallVoidMethod(javaDeviceListener, onPoseMethodId, javaMyo, timestamp, javaPose);
}

void JniDeviceListener::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quaternion) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaQuaternionObject = createJavaObjectFromQuaternion(quaternion);

	jmethodID onOrientationDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onOrientationData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Quaternion;)V");
	jenv->CallVoidMethod(javaDeviceListener, onOrientationDataMethodId, javaMyo, timestamp, javaQuaternionObject);
}

void JniDeviceListener::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaVector3Object = createJavaObjectFromVector3(accel);

	jmethodID onAccelerometerDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onAccelerometerData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Vector3;)V");
	jenv->CallVoidMethod(javaDeviceListener, onAccelerometerDataMethodId, javaMyo, timestamp, javaVector3Object);
}

void JniDeviceListener::onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);
	jobject javaVector3Object = createJavaObjectFromVector3(gyro);

	jmethodID onAccelerometerDataMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onGyroscopeData", "(Lcom/thalmic/myo/Myo;JLcom/thalmic/myo/Vector3;)V");
	jenv->CallVoidMethod(javaDeviceListener, onAccelerometerDataMethodId, javaMyo, timestamp, javaVector3Object);
}

void JniDeviceListener::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
	jobject javaMyo = createOrRetrieveMyoJavaObject(myo);

	jmethodID onConnectMethodId = jenv->GetMethodID(jenv->GetObjectClass(javaDeviceListener), "onRssi", "(Lcom/thalmic/myo/Myo;JI)V");
	jenv->CallVoidMethod(javaDeviceListener, onConnectMethodId, javaMyo, timestamp, rssi);
}

jobject JniDeviceListener::createOrRetrieveMyoJavaObject(myo::Myo *myo) {
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