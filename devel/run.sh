#!/bin/bash


DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

SYSTEM_SRC=$DIR/../system
FIRMWARE_SRC=$DIR/../hardware
FRONTEND_SRC=$DIR/../frontend
ESP_LIB_SRC=$DIR/../esp

ESP_DEVICE=/dev/ttyUSB0

export DOCKER_EXTRA_ARGS_INT="-p 1213:1213 -p 1214:1214 -p 5555:5555 -p 5556:5556 -p 8974:8974 -p 2159:2159 -p 1800:1800 -p 1883:1883 -p 8001:8001 --hostname devel_aquabox -v /var/run/dbus/system_bus_socket:/var/run/dbus/system_bus_socket"

export SOURCES_DIR=$DIR/..
export BUILD_DIR=$SOURCES_DIR/build/devel
export LOG_FILE=$SOURCES_DIR/build/devel.log

echo "Sources located in $SOURCES_DIR"
mkdir -p $BUILD_DIR
echo "Build will be done in $BUILD_DIR"
echo "Logs will be $LOG_FILE"

export IMAGE=aquabox_devel_image

docker image build -t $IMAGE - < $DIR/Dockerfile.dev &&

docker run --privileged \
           -v $SYSTEM_SRC:/sources \
           -v $FRONTEND_SRC:/frontend \
           -v $FIRMWARE_SRC:/firmware \
           -v $ESP_LIB_SRC:/esp \
           -v $BUILD_DIR:/build \
           -v /etc/timezone:/etc/timezone:ro \
           -v /etc/localtime:/etc/localtime:ro \
           --device=$ESP_DEVICE \
           $DOCKER_EXTRA_ARGS_INT \
           -e BUILD_PLATFORM=x86 \
           -e CONAN_CPU_COUNT=$CPU_COUNT \
           -it --rm $IMAGE:latest \
            /bin/bash
