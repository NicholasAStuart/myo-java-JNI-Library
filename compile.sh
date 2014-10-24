export JAVA_HOME=`/usr/libexec/java_home -v 1.7`

g++ -arch x86_64 -arch i386 -c \
-I $JAVA_HOME/include \
-I $JAVA_HOME/include/darwin \
-F $MYO_HOME \
	com_thalmic_myo_Myo.cpp \
	com_thalmic_myo_Hub.cpp \
	handle.cpp \
	jnidevicelistener.cpp 

g++ -dynamiclib -arch x86_64 -arch i386 \
-Wl,-rpath,@loader_path \
-F $MYO_HOME -framework myo -framework JavaVM -o libmyo.jnilib \
	com_thalmic_myo_Myo.o \
	com_thalmic_myo_Hub.o \
	handle.o \
	jnidevicelistener.o
