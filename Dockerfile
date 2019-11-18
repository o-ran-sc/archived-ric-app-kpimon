ARG STAGE_DIR=/tmp/kpi-xapp

#==================================================================================
FROM nexus3.o-ran-sc.org:10001/ubuntu:16.04 as ricbuild

# to override repo base, pass in repo argument when running docker build:
# docker build --build-arg REPOBASE=http://abc.def.org . ....
ARG REPOBASE=https://gerrit.oran-osc.org/r
ARG STAGE_DIR

# Install necessary packages
WORKDIR ${STAGE_DIR}
RUN apt-get update  \
     && apt-get install -y \
     libcurl4-openssl-dev \
     libcurl3 \
     cmake \
     git \
     build-essential \
     automake \
     autoconf-archive \
     autoconf \
     pkg-config \
     gawk \
     libtool \
     wget \
     zlib1g-dev \
     libffi-dev \
     && apt-get clean

# Install mdclog using debian package hosted at packagecloud.io
ARG MDC_VER=0.0.3-1
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/master/packages/debian/stretch/mdclog_${MDC_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/master/packages/debian/stretch/mdclog-dev_${MDC_VER}_amd64.deb/download.deb
RUN dpkg -i mdclog_${MDC_VER}_amd64.deb
RUN dpkg -i mdclog-dev_${MDC_VER}_amd64.deb

# Install RMR using debian package hosted at packagecloud.io
ARG RMR_VER=1.3.0
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/rmr_${RMR_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/rmr-dev_${RMR_VER}_amd64.deb/download.deb
RUN dpkg -i rmr_${RMR_VER}_amd64.deb
RUN dpkg -i rmr-dev_${RMR_VER}_amd64.deb

##-----------------------------------
# Now install the program
#------------------------------------
COPY ./ ${STAGE_DIR}
RUN export CPATH=$CPATH:/usr/local/include && \ 
    cd src && \
    make clean && \
    make install 

#---------------------------------------------
# Build the final version
#FROM nexus3.o-ran-sc.org:10004/bldr-ubuntu16-c-go:1-u16.04-nng1.1.1

FROM ubuntu:16.04

ARG STAGE_DIR

# Install Hiredis Library
RUN cd /tmp/
RUN git clone https://github.com/redis/hiredis.git
RUN cd hiredis
RUN make all

# copy just the needed libraries install it into the final image
RUN cd ${STAGE_DIR}/src
COPY --from=ricbuild ${STAGE_DIR}/*.deb /tmp/
COPY --from=ricbuild /usr/local/lib/libnng* /usr/local/lib/
RUN dpkg -i /tmp/*.deb
RUN apt-get update && \
    apt-get install -y libcurl3 && \
    apt-get clean
COPY --from=ricbuild /etc/xapp/* /etc/xapp/
COPY --from=ricbuild /usr/local/bin/kpi-xapp /usr/local/bin/kpi-xapp
#COPY --from=ricbuild /usr/local/bin/e2e-test-client /usr/local/bin/e2e-test-client
#COPY --from=ricbuild /usr/local/bin/mock-e2term-server /usr/local/bin/mock-e2term-server
#COPY --from=ricbuild /usr/local/bin/e2e-perf-client /usr/local/bin/e2e-perf-client
#COPY --from=ricbuild /usr/local/bin/e2e-perf-server /usr/local/bin/e2e-perf-server
#COPY --from=ricbuild /usr/local/bin/mock-a1-server /usr/local/bin/mock-a1-server


RUN ldconfig

ENV  RMR_RTG_SVC="127.0.0.1" \
     NAME=KPI_XAPP \
     PORT=tcp:4560 \
     THREADS=1

ARG gNodeB=NYC123,ABC345,CDR331
ARG THREADS=1

CMD ./kpi-xapp -g $gNodeB -t $THREADS
