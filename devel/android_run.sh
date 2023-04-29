#!/bin/bash


DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

DOCKER_FILE_PLATFORM=android
FRONTEND_SRC=$DIR/../frontend

export DOCKER_EXTRA_PORTS="-p 8001:8001 -p 8100:8100"

export DOCKER_EXTRA_ARGS_INT="${DOCKER_EXTRA_PORTS}"

export SOURCES_DIR=$DIR/..
export CPU_COUNT=$(nproc --all)

echo "Sources located in ${SOURCES_DIR}"

export IMAGE=aquabox_${DOCKER_FILE_PLATFORM}_image

REMOTE_PATH=/home/aquabox

docker image build -t ${IMAGE} - < $DIR/Dockerfile.${DOCKER_FILE_PLATFORM} &&

docker run --privileged \
           --name ${DOCKER_FILE_PLATFORM}_devel \
           --network=host \
           -v ${FRONTEND_SRC}:${REMOTE_PATH}/frontend \
           -v /etc/timezone:/etc/timezone:ro \
           -v /etc/localtime:/etc/localtime:ro \
           ${DOCKER_EXTRA_ARGS_INT} \
           -it --rm $IMAGE:latest \
            /bin/bash
