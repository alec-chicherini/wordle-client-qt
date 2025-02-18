FROM ubuntu:24.04 AS ubuntu2404_common_deps
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    git \
    python3 \
    build-essential \
    xz-utils \
    wget

COPY --chmod=777 scripts/install_cmake.sh ./
RUN ./install_cmake.sh

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
COPY . /wordle-task
RUN mkdir /result
ENTRYPOINT ["bash", "/wordle-task/client_qt/deploy/rebuild.sh"]

FROM ubuntu:20.04 AS qt_from_repo
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    git \
    g++-10 \ 
    qt5-default
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-10 100
COPY --chmod=777 scripts/install_cmake.sh ./
RUN ./install_cmake.sh

COPY . /wordle-task
RUN mkdir /result
ENTRYPOINT ["bash", "/wordle-task/client_qt/deploy/rebuild.sh"]

FROM ubuntu2404_common_deps AS site_repotest_ru_build
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
    clang

COPY . /wordle-task

RUN cd /wordle-task/site_repotest_ru && mkdir build && cd build && \
    git submodule init && git submodule update && \
    cmake .. && \
    cmake --build . && \
    /wordle-task/site_repotest_ru/build/site-repotest-ru > /wordle-task/site_repotest_ru/build/index.html

RUN chmod 755 /wordle-task/scripts/run_python_http_server_wasm.sh
ENTRYPOINT ["/wordle-task/scripts/run_python_http_server_wasm.sh", "/wordle-task/site_repotest_ru/build"]

FROM ubuntu2404_qt_deps AS qt_wasm_build_from_source
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

COPY . /wordle-task
RUN cd wordle-task/client_qt && mkdir build_wasm && cd build_wasm && \
    /Qt-6.7.3-wasm/bin/./qt-cmake .. && \
    cmake --build .
RUN chmod 755 /wordle-task/scripts/run_python_http_server_wasm.sh
ENTRYPOINT ["/wordle-task/scripts/run_python_http_server_wasm.sh", "/wordle-task/client_qt/build_wasm"]

FROM ubuntu2404_common_deps AS http_server_build
ENV DEBIAN_FRONTEND=noninteractive

RUN DEPS_FILE="https://raw.githubusercontent.com/userver-framework/userver/refs/heads/develop/scripts/docs/en/deps/ubuntu-24.04.md" && \
    apt install --allow-downgrades -y $(wget -q -O - ${DEPS_FILE})

RUN wget https://github.com/userver-framework/userver/releases/download/v2.7/ubuntu24.04-libuserver-all-dev_2.7_amd64.deb && \
    dpkg -i ubuntu24.04-libuserver-all-dev_2.7_amd64.deb

COPY . /wordle-task
RUN cd /wordle-task/server_http && mkdir build && cd build && \
    cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 && \
    cmake --build . && \
    cpack . && \
    dpkg -i *.deb

COPY --from=qt_wasm_build_from_source /wordle-task/client_qt/build_wasm/ /var/www/wordle-task.repotest.ru/
COPY --from=site_repotest_ru_build /wordle-task/site_repotest_ru/build/index.html /var/www/repotest.ru/ 

ENTRYPOINT ["server-http", "--config", "/etc/server-http/static_config.yaml"]