# AURUS_test_task
Tестовое задание на должность инженера программиста. ООО АУРУС Входящие

# Сборка и запуск проекта
1. с использованием cmake (тестировалось на archlinux)
   1. ```mkdir build```
   2. ```cd build```
   3. ```cmake ..```
   4. ```cmake --build .```
   5. ```./AURUS_RUN ../example.flt```
2. с использование docker
   1. ``` docker build -t aurus_test .```
   2. ``` docker run aurus_test```