# docker run --rm -ti -v $PWD:/opt/sources alpine:3.7 /bin/sh
FROM alpine:3.7 as builder
RUN echo "ipv6" >> /etc/modules

# ASK ABOUT VERA AND libcppunit-dev

RUN apk update && \
    apk --no-cache add \
        ca-certificates \
        cmake \
        g++ \
	doxygen \
	cppcheck \
	cppunit \
	valgrind

