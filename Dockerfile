FROM alpine:latest

RUN apk add --no-cache make

RUN apk add build-base

RUN apk add --no-cache clang lld llvm

RUN apk add --no-cache boost-dev

RUN apk add --no-cache perf linux-headers util-linux

RUN apk add --no-cache compiler-rt


WORKDIR /app

COPY . .