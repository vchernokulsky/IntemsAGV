using System.Collections;
using System.Collections.Generic;
using System.Threading;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using UnityEngine;
using Quaternion = UnityEngine.Quaternion;
using Vector3 = UnityEngine.Vector3;

namespace RosSharp.RosBridgeClient
{
    public class OdomWheelPub : OdomPublisher
    {
        public float start_x;
        public float start_y;
        public float start_z;
        public float y_ang;
        
        private float x_pose;
        private float z_pose;
        private float theta;

        protected override void Start()
        {
	        base.Start();
	        x_pose = start_x;
	        z_pose = start_z;
	        theta = y_ang;
        }
        
        protected override void setPose(MessageTypes.Geometry.PoseWithCovariance pose)
        {
            UpdatePoseRotation();
            Vector3 cur_pose = new Vector3(-x_pose, -start_y, -z_pose);
            Quaternion cur_rot = Quaternion.Euler(0, theta * Mathf.Rad2Deg, 0);
            GetGeometryPoint(cur_pose.Unity2Ros(), pose.pose.position);
            GetGeometryQuaternion(cur_rot.Unity2Ros(), pose.pose.orientation);

           
        }

        private void UpdatePoseRotation()
        {
            float left_travel = leftJoint.velocity * Mathf.Deg2Rad * Time.deltaTime * wheelRadius;
            float right_travel = rightJoint.velocity * Mathf.Deg2Rad * Time.deltaTime * wheelRadius;

            float delta_travel = (right_travel + left_travel) / 2.0f;
            float delta_theta = (right_travel - left_travel) / wheelSeparation;
	        
            float delta_z = 0;
            float delta_x = 0;
    
            if (right_travel == left_travel)
            {
                delta_z = left_travel * Mathf.Cos(theta);
                delta_x = left_travel * Mathf.Sin(theta);
            }
            else
            {
    
                float radius = (delta_theta == 0)? 0 : delta_travel / delta_theta;
    
                float icc_z = z_pose - radius * Mathf.Sin(theta);
                float icc_x = x_pose + radius * Mathf.Cos(theta);

                delta_z = Mathf.Cos(delta_theta) * (z_pose - icc_z) - Mathf.Sin(delta_theta) * (x_pose - icc_x) +
                    icc_z - z_pose;
                delta_x = Mathf.Sin(delta_theta) * (z_pose - icc_z) + Mathf.Cos(delta_theta) * (x_pose - icc_x) +
                    icc_x - x_pose;
            }
            
            
            z_pose += delta_z;
            x_pose += delta_x;
            theta += delta_theta;
            
            while (theta > 2.0f * Mathf.PI){
                theta -= 2.0f * Mathf.PI;
            }

        }
    }
}
