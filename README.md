#ROS
В репозитории находятся два рабочих пространства:

- рабочее пространства робота (для jetson nano):
`IntemsAGV/software/catkin_ws`
- рабочее пространство с симуляциями в Gazebo
`IntemsAGV/software/Simulation/catkin_ws`

Для работы с Ros в каждом терминале, находясь в корне рабочего пространства
(папка catkin_ws) необходимо выполнить команду `source devel/setup.bash`

Для распределенного запуска нод в каждом терминале необходимо указать адрес 
Ros-мастера(jetson nano, порт 11311 является стандартным): `export ROS_MASTER_URI=http://192.168.0.150:11311` 

## Custom diff drive
Управление роботом написаное нами

- осуществляет отправку данных полученных из топика /cmd_vel на робота в формате [left_dir, left_speed, right_dir,right_speed]
- на основе данных считанных с энкодера (**линейные скорости** левого и правого колеса) публикует одометрию
- публикует tf состояние робота
- публикует данные с rplidar (LaserScan) в топик /scan
- ~~публикует данные с raspbery-cam~~

Для запуска: `roslaunch cube_robot cube_robot.launch`

## diff_drive (Python)
Сделано на основе https://github.com/merose/diff_drive

- осуществляет отправку данных полученных из топика /cmd_vel на робота в формате [left_dir, left_speed, right_dir,right_speed]
- на основе данных считанных с энкодера (**кол-во тиков** на левом и правом колесе) публикует одометрию

Настрока параметров роботов осуществляется в файле `IntemsAGV/software/catkin_ws/src/diff_drive/config/params.yaml`

Для запуска: `roslaunch diff_drive diff_drive.launch`

## differential drive (c++)
**!!!требует доработки для использования с нашим роботом!!!**

пока запущен только пример

Для использования необходимо установить ros-control, ros-controllers: 

`sudo apt-get install ros-melodic-ros-control ros-melodic-ros-controllers`

Документация по ros-control: https://github.com/ros-controls/ros_control/wiki

Пример использования https://github.com/eborghi10/my_ROS_mobile_robot.git 

Для запуска примера: ` roslaunch my_robot_bhw_control.launch`

Конфигурация робота для примера: `IntemsAGV/software/catkin_ws/src/my_ROS_mobile_robot/my_robot_control/config`

- на основе /cmd_vel вычисляет линейные скорости для левого и правого колеса и публикует в топики /my_robot/right_wheel_vel и /my_robot/left_wheel_vel
- (_может быть изменено_) публикует одометрию на основе на основе угла поворота колёс (угол поворота читается из топиков /my_robot/left_wheel_angle и /my_robot/right_wheel_angle)

    
## Запуск отельных нод
#### публикация tf состояния робота
`roslaunch cube_robot tf.launch`
#### Rplidar
`roslaunch rplidar_ros rplidar.launch`
#### teleop (из консоли)
`roslaunch cube_robot_teleop cube_robot_teleop_key.launch`
#### teleop (из приложения android)
`roslaunch cube_robot_teleop cube_robot_teleop_server.launch`

установщик для приложения находится в папке `IntemsAGV/software/RobotController/app/release`
