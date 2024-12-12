#!/bin/bash
SOURCE_FOLDER=$(pwd)
DEST_FOLDER=/usr/app
IMAGE=renginecompiler
CONTAINER=renginecompilerrunner

# Run Docker container with the specified volume
if ! docker build . -t $IMAGE; then
    echo "Error: cannot build renginecompiler image."
    exit 84
fi

docker stop $CONTAINER 2> /dev/null
docker rm $CONTAINER 2> /dev/null
rm -rf build/ || true
docker run --rm \
    --name $CONTAINER \
    -v "$SOURCE_FOLDER:$DEST_FOLDER:rw" \
    renginecompiler
