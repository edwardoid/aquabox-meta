#!/bin/bash

mkdir -p /var/jenkins_home
mkdir -p /root/ci
mkdir -p /root/artifactory

SOURCE="${BASH_SOURCE[0]}"
LAST_DIR=$PWD
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

cd $DIR/..
git submodule -q foreach git pull
cd $DIR

docker-compose up -d --no-deps --build

cd $DIR
cd $LAST_DIR
