
Проект моделирует работу производственного цеха с последовательной обработкой изделий и динамическим распределением изделий по станкам.

---

## Требования

* C++20
* CMake 3.16+
* GCC / Clang / MinGW
* Только стандартная библиотека C++
* GoogleTest используется только для тестов и подключается как git submodule

---

## Сборка проекта

Склонируйте репозиторий:

```bash
git clone https://github.com/ElizavetaFrolova/production-workshop-simulator
cd production-workshop-simulator
````

Загрузите submodule с GoogleTest:

```bash
git submodule update --init --recursive
```

Соберите проект из корня:

```bash
cmake -S . -B build
cmake --build build
```

Полная пересборка:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

## Запуск программы

```bash
./build/simulator input.txt
```

Сохранить вывод в файл:

```bash
./build/simulator input.txt > output.txt
```

## Запуск тестов

```bash
cmake --build build --target all_tests
./build/tests/all_tests
```

## Примера из технического задания

В техническом задании есть полный пример моделирования, однако он противоречит правилам задания.

Программа ориентируется на формальные требования, а не на пример.

Присутствует несовпадение порядка сообщений

В задании указано, что при одинаковом времени порядок вывода должен быть:

```text
finish -> start -> wait -> ready
```

Но в примере из задания местами используется другой порядок.

Например, в момент `t = 4` в примере указано:

```text
finish 4 3 1 1
ready 4 3 1
start 4 4 0 1
```

Однако по формальному правилу `start` должен идти раньше `ready`:

```text
finish 4 3 1 1
start 4 4 0 1
ready 4 3 1
```
Так же в примере есть несовпадение времен обработки

