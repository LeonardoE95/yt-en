# docker build -t example-ssh-tunneling .
# docker run --name example-ssh-tunneling --rm -d -p1338:22 example-ssh-tunneling
# ssh -p1338 -o "UserKnownHostsFile=/dev/null" -L1337:localhost:1337 root@127.0.0.1

FROM python:3.11-alpine

# -- ssh config
RUN apk --update add --no-cache openssh bash 
RUN sed -i s/#PermitRootLogin.*/PermitRootLogin\ yes/ /etc/ssh/sshd_config
RUN sed -i "s/AllowTcpForwarding no/AllowTcpForwarding yes/" /etc/ssh/sshd_config

RUN echo "root:password" | chpasswd
RUN cd /etc/ssh/ && ssh-keygen -A
EXPOSE 22

# -- flask app config
COPY ./app /app
WORKDIR /app
RUN pip install -r requirements.txt

COPY ./init.sh /init.sh

# CMD ["python", "app.py"]
# CMD ["/usr/sbin/sshd", "-D"]
CMD ["/init.sh"]