FROM ubuntu:latest

RUN apt-get update
RUN apt-get install sudo nano python3-pip openssh-server -y

RUN echo "ubuntu:abc123" | chpasswd
RUN echo "root:lprulz4e" | chpasswd