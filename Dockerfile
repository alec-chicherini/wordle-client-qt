FROM ubuntu:24.04 AS ubuntu2404_common_deps
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    git \
    python3 \
    build-essential \
    xz-utils \
    wget \
    clang-format

RUN wget -O - https://raw.githubusercontent.com/alec-chicherini/development-scripts/refs/heads/main/cmake/install_cmake.sh 2>/dev/null | bash

FROM ubuntu2404_common_deps AS ubuntu2404_qt_deps
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    libgl1-mesa-dev \
    libfontconfig1-dev \
    libfreetype-dev \
    libx11-dev \
    libx11-xcb-dev \
    libxcb-cursor-dev \
    libxcb-glx0-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-shape0-dev \
    libxcb-shm0-dev \
    libxcb-sync-dev \
    libxcb-util-dev \
    libxcb-xfixes0-dev \
    libxcb-xinerama0-dev \
    libxcb-xkb-dev \
    libxcb1-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxrender-dev

FROM ubuntu2404_qt_deps AS qt_from_source
RUN apt update && \
    apt install -y \
    g++-13 

RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-13 100

RUN wget https://mirror.yandex.ru/mirrors/qt.io/archive/qt/6.7/6.7.3/single/qt-everywhere-src-6.7.3.tar.xz && \
    tar xf qt-everywhere-src-6.7.3.tar.xz

RUN mkdir qt-build && cd qt-build
RUN   ../qt-everywhere-src-6.7.3/configure -release -submodules qtbase -nomake tests -nomake examples
RUN cmake --build . --parallel 4
RUN cmake --install . --prefix /Qt-6.7.3

ENV QT_BUILDED_FROM_SOURCE_PATH=/Qt-6.7.3
COPY . /wordle-client-qt
RUN mkdir /result
ENTRYPOINT ["bash", "/wordle-client-qt/deploy/rebuild.sh"]

FROM ubuntu:20.04 AS qt_from_repo
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    git \
    g++-10 \ 
    qt5-default \
    wget
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-10 100
RUN wget -O - https://raw.githubusercontent.com/alec-chicherini/development-scripts/refs/heads/main/cmake/install_cmake.sh 2>/dev/null | bash

COPY . /wordle-client-qt
RUN mkdir /result
RUN bash /wordle-client-qt/deploy/rebuild.sh
ENTRYPOINT ["ls", "/result"]

FROM ubuntu2404_qt_deps AS ubuntu2404_qt_lib_wasm_build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    clang \
    libclang-18-dev

RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install 3.1.50 && \
    ./emsdk activate 3.1.50 && \
    bash ./emsdk_env.sh

ENV EMSDK=/emsdk
ENV EMSDK_NODE=/emsdk/node/20.18.0_64bit/bin/node
ENV PATH=/emsdk:/emsdk/upstream/emscripten:/emsdk/node/20.18.0_64bit/bin:${PATH}

RUN wget https://mirror.yandex.ru/mirrors/qt.io/archive/qt/6.7/6.7.3/single/qt-everywhere-src-6.7.3.tar.xz && \
    tar xf qt-everywhere-src-6.7.3.tar.xz  && \
    cd qt-everywhere-src-6.7.3 && \
    mkdir qt-build-base && cd qt-build-base && \
   ../configure -release -submodules qtbase,qtshadertools,qttools,qtquick3d,qtremoteobjects,qtscxml -nomake tests -nomake examples
RUN cd /qt-everywhere-src-6.7.3/qt-build-base && cmake --build . --parallel 4
RUN cd /qt-everywhere-src-6.7.3/qt-build-base && cmake --install . --prefix /Qt-6.7.3-base

RUN cd /qt-everywhere-src-6.7.3 && mkdir qt-build-wasm && cd qt-build-wasm && \
    ../configure -qt-host-path /Qt-6.7.3-base -platform wasm-emscripten -prefix /Qt-6.7.3-wasm
RUN cd /qt-everywhere-src-6.7.3/qt-build-wasm && cmake --build . --parallel 4
RUN cd /qt-everywhere-src-6.7.3/qt-build-wasm && cmake --install . --prefix /Qt-6.7.3-wasm

FROM ubuntu2404_qt_lib_wasm_build AS ubuntu2404_protobuf_abseil_qt_wasm_build
#abseil for protobuf v30.0
RUN git clone https://github.com/abseil/abseil-cpp.git && \
    cd abseil-cpp && \
    git checkout 4a2ba8cadeb9890322697a4f99f71094b6fd3ceb && \
    mkdir build && cd build && \
    emcmake cmake .. && \
    cmake --build . && \
    cmake --install . && \
    cd /
#protobuf v30.0
RUN git clone https://github.com/protocolbuffers/protobuf.git && \
    cd protobuf && \
    git checkout d295af5c3002c08e1bfd9d7f9e175d0a4d015f1e && \
    mkdir build && cd build && \
    emcmake cmake -Dabsl_DIR=/emsdk/upstream/emscripten/cache/sysroot/lib/cmake/absl/ -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_PROTOC_BINARIES=OFF -Dprotobuf_BUILD_LIBUPB=OFF ..  && \
    cmake --build . && \
    cmake --install .

FROM ubuntu2404_protobuf_abseil_qt_wasm_build AS qt_wasm_build_from_source
RUN apt install -y protobuf-compiler unzip

RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v30.0/protoc-30.0-linux-x86_64.zip && \
    unzip -o protoc-30.0-linux-x86_64.zip

COPY . /wordle-client-qt
RUN cd wordle-client-qt && mkdir build_wasm && cd build_wasm && \
    /Qt-6.7.3-wasm/bin/./qt-cmake -Dabsl_DIR=/emsdk/upstream/emscripten/cache/sysroot/lib/cmake/absl/ .. && \
    cmake --build . && \
    mkdir /result && \
    cp wordle-client-qt.* /result && \
    cp qtloader.js /result && \
    cp qtlogo.svg /result

RUN chmod 755 /wordle-client-qt/scripts/run_python_http_server_wasm.sh
ENTRYPOINT ["/wordle-client-qt/scripts/run_python_http_server_wasm.sh", "/result"]
