FROM ubuntu:22.04

RUN apt-get update && apt-get install -y cmake python3 python3-pip pkg-config && pip install conan
RUN conan profile detect

WORKDIR /usr/app
COPY . .
CMD ./compile.sh
