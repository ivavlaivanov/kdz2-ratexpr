# Как запустить эту штуковину

1. находясь в деректории расположения этого файла, выполнить:

```bash
cmake -S . -B build-debug   -DCMAKE_BUILD_TYPE=Debug   && cmake --build build-debug   -j
```

или

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release && cmake --build build-release -j
```

соответсвенно
