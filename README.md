# wordle-client-qt

# Структутра
Из этого репозитория клиент собирается в:
1. *.deb пакет для запуска на рабочем столе Linux 
2. файлы для сайта https://wordle-client-qt.repotest.ru

# Сборка
<!--Подготовить хост-->
## Подготовить хост для сборки.

Установить docker 
```bash
source <(curl https://raw.githubusercontent.com/alec-chicherini/wordle-task/refs/heads/main/scripts/install_docker.sh)
```

<!--Собрать клиент и deb пакет для linux-->
## Собрать клиент и deb пакет для linux ubuntu
Собрать в docker
```bash
git clone https://github.com/alec-chicherini/wordle-task.git
cd wordle-task
docker build --target=qt_from_repo . -t wordle-task-build
#TODO docker build --target=qt_from_source . -t wordle-task-build-source #другая опция собрать Qt из исходников.
mkdir result #Тут будет инсталятор 
docker run -v ./result:/result wordle-task-build
```

<!--Собрать клиент wasm и запустить для дебага-->
## Собрать клиент в wasm для использования на веб сервере
Собрать в docker и запустить python http.server для дебага
```bash
git clone https://github.com/alec-chicherini/wordle-task.git
cd wordle-task
docker build --target=qt_wasm_build_from_source . -t wordle-task-build-wasm
docker run --rm -d -p 80:8000 wordle-task-build-wasm
```

# Запуск в Linux
## Подготовить чистую машину для тестов Ubuntu 20.04
```
apt update
apt install ubuntu-desktop
apt install xrdp
#passwd

dpkg -i wordle-task_24.12_amd64.deb
apt-get install -f -y
```
## Запустить игру
Пуск - Стандартные - wordle-task
 
## Добавить слова в игру
```bash
echo "УЕЫАО ЭЯИЮЙ" > /opt/wordle-task/words/new_words.txt
``` 