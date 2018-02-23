# docker run --rm -ti -v $PWD:/opt/sources ubuntu:16.04 /bin/sh
FROM ubuntu:16.04 as builder
RUN apt-get update && \
    apt-get install -y --no-install-recommends software-properties-common && \
    add-apt-repository ppa:chrberger/libcluon -y && \
    apt-get update && \
    apt-get install -y \
        ca-certificates \
        doxygen \
        libcluon \
		cppcheck \
		libcppunit-dev \
		valgrind \
		vera++ \
        cmake \
        g++ \
        make
ADD . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources && \
    mkdir build -p && \
    cd build/ && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && make test && cp -r src/FirstSub/Communicator /tmp

# Deploy.
FROM ubuntu:16.04
RUN apt-get update && \
    apt-get install -y --no-install-recommends software-properties-common && \
    add-apt-repository ppa:chrberger/libcluon -y && \
    apt-get update && \
    apt-get install libcluon && \
    mkdir -p /opt
WORKDIR /opt
COPY --from=builder /tmp/Communicator .
CMD ["/opt/Communicator"]
