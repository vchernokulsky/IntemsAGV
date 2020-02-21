#!/usr/bin/python
 
import roslib
import rospy
import tf

if __name__ == '__main__':
    rospy.init_node('left_wheel_tf')
    rate = rospy.Rate(20)
    X_COORD = rospy.get_param('~x_coord')
    Y_COORD = rospy.get_param('~y_coord')
    T_COORD = rospy.get_param('~t_coord')
    TF_TO = rospy.get_param('~tf_to')
    br = tf.TransformBroadcaster()
    while not rospy.is_shutdown():
	 br.sendTransform((X_COORD, Y_COORD, 0),
		             tf.transformations.quaternion_from_euler(0, 0, T_COORD),
		             rospy.Time.now(),TF_TO,"/base_link")
         rate.sleep()
    rospy.spin()
