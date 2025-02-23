# wordle-client-qt

# Структутра
Из этого репозитория клиент собирается в:
1. *.deb пакет для запуска на рабочем столе Linux 
2. файлы для сайта https://wordle-client-qt-25-02.repotest.ru/wordle-client-qt.html
3. файлы для сайта https://wordle-client-qt-25-03.repotest.ru/wordle-client-qt.html

# Сборка
<!--Подготовить хост-->
## Подготовить хост для сборки.

Установить docker 
```bash
source <(curl https://raw.githubusercontent.com/alec-chicherini/development-scripts/refs/heads/main/docker/install_docker.sh)
```
<!--Версии-->
## Версии
| Version | State | Comment |
|-|-|-|
|25.02| Done |первая версия клиента. Standalone. Игра устанавливается полностью с дистрибутивом.|
|25.03| In Progress| Версия клиента для микросервисов. Не работает без остальных сервтисов|
<!--Собрать клиент и deb пакет для linux-->
## Собрать latest клиент и deb пакет для linux ubuntu
Собрать в docker 
```bash
git clone https://github.com/alec-chicherini/wordle-client-qt.git
cd wordle-client-qt
git checkout 25.02
#git checkout 25.03
git submodule init
git submodule update
docker build --target=qt_from_repo . -t wordle-client-qt-build-desktop
#TODO docker build --target=qt_from_source . -t wordle-client-qt-build-desktop #другая опция собрать Qt из исходников.
#Скопировать .deb пакет из образа в .
idTempContainer=$(docker create wordle-client-qt-build-desktop)
docker cp "$idTempContainer":/result/*.deb .
docker rm "$idTempContainer"
```

<!--Собрать клиент wasm и запустить для дебага-->
## Собрать latest клиент в wasm для использования на веб сервере
Собрать в docker и запустить python http.server для дебага
```bash
git clone https://github.com/alec-chicherini/wordle-client-qt.git
cd wordle-client-qt
git checkout 25.02
#git checkout 25.03
git submodule init
git submodule update
docker build --target=qt_wasm_build_from_source . -t wordle-client-qt-build-wasm
docker run --rm -d -p 80:8000 wordle-client-qt-build-wasm
```

# Запуск в Linux
## Подготовить чистую машину для тестов Ubuntu 20.04
```
apt update
apt install ubuntu-desktop
apt install xrdp
#passwd

dpkg -i wordle-client-qt_25.02_amd64.deb
apt-get install -f -y
```
## Запустить игру
Пуск - Стандартные - wordle-client-qt
 
## Добавить слова в игру для версии 25.02
```bash
echo "УЕЫАО ЭЯИЮЙ" > /opt/wordle-client-qt/words/new_words.txt
``` 