FROM ubuntu:22.04

RUN apt-get -y update && apt-get install -y sudo cmake python3 python3-pip python3.10-venv pkg-config && pip install conan
RUN conan profile detect

WORKDIR /usr/app
COPY . .
CMD ./compile.sh
