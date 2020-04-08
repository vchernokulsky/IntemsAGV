# Ackermann steering
## Модель робота
на основе https://github.com/lucasw/carbot.git создана модель, соответствующая радиоуправляемой машинке:
![Image of car](https://github.com/vchernokulsky/IntemsAGV/blob/dev/software/Simulation/ackermann_ws/img/car.jpg)

добавлена нода, публикующая tf преобразование odom->base_link по координатам из одометрии(одометрия публикуется плагином газебо).

удалена публикация tf преобразования map->base_link из acker.py

удален код, неприминимый для данной модели из cmd_vel_to_joint.py

### запуск
`roslaunch carbot_gazebo_control carbot_gazebo_control.launch`

## SLAM и gmapping

построение карты выполняется так же, как и для diff drive роботов.

### запуск построения карты
`roslaunch carbot_slam carbot_slam.launch`

### сохранить карту
`rosrun map_server map_saver -f ~/map`

## Navigation stack

автономное передвижение робота выолняется так же, как и для diff drive роботов.
Поскольку DWA local planner предполагает, что робот может поворачиваться на месте, он был заменен на TEB local planner(http://wiki.ros.org/teb_local_planner).

### запуск
`roslaunch carbot_navigation robot_carlike_in_stage.launch`
