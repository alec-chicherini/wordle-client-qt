#!/bin/bash
set -e
git pull
rm result -rf
trap 'sudo apt remove wordle-client-qt -y' ERR
docker build --target=qt_from_repo . -t wordle-client-qt-build-desktop
idTempContainer=$(docker create wordle-client-qt-build-desktop)
docker cp "$idTempContainer":/result .
docker rm "$idTempContainer"
sudo dpkg -i result/*.deb