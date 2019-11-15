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

Для улучшения работы  можно настроить параметры в соответствии с определенным роботом 
(файлы costmap_common_params_cube_robot.yaml и dwa_local_planner_params_cube_robot.yaml). 

### Топики
Для работы NavigationStack взаимодействует со следующими топиками:
- _cmd_vel_ - публикует управляющие команды для робота (geometry_msgs/Twist)
- _odom_ - слушает текущее положение робота (nav_msgs/Odometry)
- _scan_ - слушает данные полученые с лидара (sensor_msgs/LaserScan)
- _tf_ - слушает преобразование координат частей робота и их взаимосвязь с координатами мира

## Slam (построение карты)
Сначала необходимо запустить симуляцию `roslaunch cube_robot_gazebo cube_world.launch`,

затем Slam `roslaunch cube_robot_slam cube_robot_slam.launch`

и teleop для управления роботом `roslaunch cube_robot_teleop cube_robot_teleop_key.launch`.

После того как карта будет построена её необходимо сохранить с помощью команды ` rosrun map_server map_saver -f <путь до папки maps>/map`

Для корректной работы необходимо чтобы топики были настроены также как и в NavigationStack.

В сгенерированном файле следует заменить значение параметра _image_ на map.pgm