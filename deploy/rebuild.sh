#!/bin/bash
cd /wordle-task/client_qt
rm build_client -rf
mkdir build_client
cd build_client

if [ -n $QT_BUILDED_FROM_SOURCE_PATH ]
then
	cmake .. -DCMAKE_PREFIX_PATH=$QT_BUILDED_FROM_SOURCE_PATH/lib/cmake
else
	cmake ..
fi

cmake --build .
cpack .
mv *.deb /result