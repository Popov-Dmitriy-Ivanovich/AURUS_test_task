FROM gcc:latest

RUN apt-get update && \
    apt-get install -y\
      cmake

WORKDIR /app
COPY src /app/src
COPY tests /app/tests
COPY naive.cpp /app
COPY CMakeLists.txt /app
COPY example.flt /app

WORKDIR /app/build
RUN cmake ..
RUN cmake --build .

CMD ["./AURUS_RUN" , "../example.flt"]

