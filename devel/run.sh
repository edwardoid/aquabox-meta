#!/bin/bash


DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

SYSTEM_SRC=$DIR/../system
FIRMWARE_SRC=$DIR/../hardware
FRONTEND_SRC=$DIR/../frontend
ARDUINO_LIB_SRC=$DIR/../hardware/libraries
ESP_LIB_SRC=$DIR/../esp

ESP_DEVICE=/dev/ttyUSB0


export DOCKER_EXTRA_PORTS="-p 1213:1213 -p 1214:1214 -p 5555:5555 -p 5556:5556 -p 8974:8974 -p 2159:2159 -p 1800:1800 -p 1883:1883 -p 8001:8001 -p 8100:8100"
DOCKER_EXTRA_PORTS=""
export DOCKER_EXTRA_ARGS_INT="${DOCKER_EXTRA_PORTS} --hostname devel_aquabox -v /var/run/dbus/system_bus_socket:/var/run/dbus/system_bus_socket"

export SOURCES_DIR=$DIR/..
export CPU_COUNT=$(nproc --all)
export BUILD_DIR=$SOURCES_DIR/build/devel
export LOG_FILE=$SOURCES_DIR/build/devel.log

echo "Sources located in $SOURCES_DIR"
mkdir -p $BUILD_DIR
mkdir -p $BUILD_DIR/android
echo "Build will be done in $BUILD_DIR"
echo "Logs will be $LOG_FILE"

export IMAGE=aquabox_devel_image

REMOTE_PATH=/home/aquabox

docker image build -t $IMAGE - < $DIR/Dockerfile.dev &&

docker run --privileged \
           --name all_in_one_devel \
           -v $SYSTEM_SRC:$REMOTE_PATH/sources \
           -v $FRONTEND_SRC:$REMOTE_PATH/frontend \
           -v $FIRMWARE_SRC:$REMOTE_PATH/firmware \
           -v $ESP_LIB_SRC:$REMOTE_PATH/esp \
           -v $ARDUINO_LIB_SRC:$REMOTE_PATH/Arduino/libraries \
           -v $BUILD_DIR:$REMOTE_PATH/build \
           -v /etc/timezone:/etc/timezone:ro \
           -v /etc/localtime:/etc/localtime:ro \
           --device=$ESP_DEVICE \
           $DOCKER_EXTRA_ARGS_INT \
           -e BUILD_PLATFORM=armv6 \
           -e CONAN_CPU_COUNT=$CPU_COUNT \
           -e ESPBAUD=115200 \
           -it --rm $IMAGE:latest \
            /bin/bash
