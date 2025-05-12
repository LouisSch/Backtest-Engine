FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

CMD ["./build/BacktestEngine"]