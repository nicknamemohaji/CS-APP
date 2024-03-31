FROM isolated_runtime

WORKDIR /tmp

COPY . .

RUN gcc main.c -o main

CMD ["./loader_seccomp", "./main"]