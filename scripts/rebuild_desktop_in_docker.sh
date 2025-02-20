#!/bin/bash
set -e
$(sudo apt remove wordle-client-qt -y || echo ok)
rm result -rf
git pull
docker build --target=qt_from_repo . -t wordle-client-qt-build-desktop
idTempContainer=$(docker create wordle-client-qt-build-desktop)
docker cp "$idTempContainer":/result .
docker rm "$idTempContainer"
sudo dpkg -i result/*.deb