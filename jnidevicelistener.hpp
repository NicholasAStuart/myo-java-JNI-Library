#include <jni.h>
#include <myo/myo.hpp>
#include <map>

#ifndef __JNI_DEVICE_LISTENER_INCLUDED__ 
#define __JNI_DEVICE_LISTENER_INCLUDED__ 

class JniDeviceListener : public myo::DeviceListener {
private:
	jobject javaDeviceListener;
	JavaVM *javavm;
	std::map<myo::Myo*, jobject> myoMap;
	
	JNIEnv* getEnvironmentFromVm();
	
	template<typename T>
	jobject createJavaObjectFromQuaternion(const myo::Quaternion<T>&);

	template<typename T>
	jobject createJavaObjectFromVector3(const myo::Vector3<T>&);

	jobject createJavaObjectFromPose(myo::Pose pose);

	jobject createOrRetrieveMyoJavaObject(myo::Myo *myo);

	jobject createJavaObjectFromFirmwareVersion(myo::FirmwareVersion firmwareVersion);

	jobject createJavaObjectFromArm(myo::Arm arm);

	jobject createJavaObjectFromXDirection(myo::XDirection xDirection);

    jobject createJavaObjectFromWarmupResult(myo::WarmupResult warmupResult);

    jobject createJavaObjectFromWarmupState(myo::WarmupState warmupState);
public:
    JniDeviceListener(JavaVM *, jobject, std::map<myo::Myo*, jobject>);

    /// Called when a Myo has been paired.
    void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion);

    /// Called when a Myo has been unpaired.
    void onUnpair(myo::Myo* myo, uint64_t timestamp);

    /// Called when a paired Myo has been connected.
    void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion);

    /// Called when a paired Myo has been disconnected.
    void onDisconnect(myo::Myo* myo, uint64_t timestamp);

    /// Called when a paired Myo recognizes that it is on an arm.
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, myo::WarmupState warmupState);

    /// Called when a paired Myo is moved or removed from the arm.
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp);

    /// Called when a paired Myo becomes unlocked. 
    void onUnlock(myo::Myo* myo, uint64_t timestamp);

    /// Called when a paired Myo becomes locked. 
    void onLock(myo::Myo* myo, uint64_t timestamp);

    /// Called when a paired Myo has provided a new pose.
    void onPose(myo::Myo* myo, uint64_t timestamp,myo::Pose pose);

    /// Called when a paired Myo has provided new orientation data.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation);

    /// Called when a paired Myo has provided new accelerometer data in units of g.
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel);

    /// Called when a paired Myo has provided new gyroscope data in units of deg/s.
    void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro);

    /// Called when a paired Myo has provided a new RSSI value.
    /// @see Myo::requestRssi() to request an RSSI value from the Myo.
    void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi);

    /// Called when a paired Myo receives an battery level update.
    void onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level);

    /// Called when a paired Myo has provided new EMG data.
    /// emg is an array of 8 elements, each corresponding to one sensor.
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg);
    
    /// Called when the warmup period for a Myo has completed.
    void onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, myo::WarmupResult warmupResult);
};
#endif
