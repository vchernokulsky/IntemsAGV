# Robot Simulation
Симуляция робота в Gazebo реализована в рабочем пространстве `IntemsAGV/software/Simulation/catkin_ws`.

В каждом терминале рабочего пространства необходимо выполнить команду `source devel/setup.bash`.
## Симуляция
Запуск симуляции выполняется команда `roslaunch cube_robot_gazebo cube_world.launch`.

Параметры симуляции (в файле `cube_world.launch`):
- **_GAZEBO_RESOURCE_PATH_ - полный путь до текущего рабочего пространства**
- _x_pos, y_pos, z_pos_ - начальные координаты робота в мире
- _world_name_ - файл, описывающий мир

## NavigationStack
http://wiki.ros.org/navigation 

### Запуск

Сначала необходимо запустить симуляцию `roslaunch cube_robot_gazebo cube_world.launch`,

Затем в другом терменале NavigationStack `roslaunch cube_robot_navigation cube_robot_navigation.launch`.

### Настройка
В `cube_robot_navigation.launch` используется карта местности, файл которой указан в параметре _map_file_ (карта строистся с помоью slam).



