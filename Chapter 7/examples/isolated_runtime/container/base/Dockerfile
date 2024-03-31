FROM ubuntu:jammy

WORKDIR /tmp

COPY loader_seccomp.c ./loader_seccomp.c

RUN sudo apt-get update && sudo apt-get install -y gcc deb-essentials libseccomp-dev 
RUN gcc -lseccomp -o loader_seccomp loader_seccomp.c
