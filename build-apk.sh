#!/bin/bash

if [[ "$1" = "--help" ]]; then
	echo "Usage: ./build.sh <release|debug> <omp|noomp>"
	exit 0
fi

if [[ "$#" -ne "2" ]]; then
	echo "Illegal number of parameters, see ./build.sh --help"
	exit 0
fi

if [[ "$1" = "release" ]]; then
	VERBOSE="0"
	RELEASE="0"
else
	RELEASE="1 APP_CFLAGS=\"-gdwarf-3\""
	VERBOSE="1"
fi

if [[ "$2" = "omp" ]]; then
	OMP="1"
	NAME_POSTFIX="omp"
else
	OMP="0"
	NAME_POSTFIX="noomp"
fi

echo "*****************************************"
echo "* OMP: $OMP "
echo "* RELEASETYPE: $RELEASE "
echo "*****************************************"

set -x

# Regenerate PAK file
mkdir -p pak/
mkdir -p assets/
cp -r ../3rdparty/extras/* pak/
python2 makepak.py pak/ assets/extras.pak

# Build
$ANDROID_NDK_HOME/ndk-build -j8 NDK_TOOLCHAIN_VERSION=4.8 _CS16CLIENT_ENABLE_OPENMP=$OMP NDK_DEBUG=$RELEASE V=$VERBOSE
JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/ ANDROID_HOME=android-sdk/ ant debug

if [[ "$1" = "release" ]]; then
	jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1
	$HOME/.android/android-sdk-linux/build-tools/23.0.3/zipalign 4 bin/cs16-client-$1-unsigned.apk bin/cs16-client.apk
fi
mv bin/cs16-client.apk cs16-client-$1-$NAME_POSTFIX.apk
