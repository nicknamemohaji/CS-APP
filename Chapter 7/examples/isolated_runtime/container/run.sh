#!/bin/bash

echo compiling...
# build loader binary
gcc -static loader.c -lseccomp -o loader
if [ $? -ne 0 ]; then
    echo error compiling loader
    exit $?
fi

# build target binary
gcc -static bin.c -o a.out
if [ $? -ne 0 ]; then
    echo error compiling target
    exit $?
fi
echo done!

# build container
IMAGE_NAME="isolated:$(openssl rand -hex 3)"
echo running image: $IMAGE_NAME

docker build . --tag "${IMAGE_NAME}"
if [ $? -ne 0 ]; then
    echo error building image
    exit $?
fi

docker run --rm "${IMAGE_NAME}" > outfile < infile
if [ $? -ne 0 ]; then
    echo error running container
    exit $?
fi

docker image rm "${IMAGE_NAME}"
if [ $? -ne 0 ]; then
    echo error removing image
    exit $?
fi
echo done!

# remove binaries built
rm -f a.out
rm -f loader

