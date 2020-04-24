using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    abstract public class OdomPublisher : UnityPublisher<MessageTypes.Nav.Odometry>
    {
        public float rate = 10.0f;
        public string FrameId = "odom";
        public string ChildId = "base_link";
        
        public Transform leftWheel;
        public Transform rightWheel;

        public float wheelRadius = 0.035f;
        public float wheelSeparation = 0.23f;
        
        public bool tfBroadcasting = true;

        private MessageTypes.Nav.Odometry odom;
        protected HingeJoint leftJoint;
        protected HingeJoint rightJoint;
        private float time_pass;

        private TfOdomBroadcaster tfBroadcaster;

        // Start is called before the first frame update
        protected override void Start()
        {
            base.Start();
            GetJoints();
            InitializeMessage();
            time_pass = 0.0f;
            tfBroadcaster = GetComponent<TfOdomBroadcaster>();
        }

        private void FixedUpdate()
        {
            UpdateMessage();
        }

        private void GetJoints()
        {
            if (leftWheel == null || leftWheel.Equals(null))
            {
                Debug.LogWarning("left wheel transform is null");
            }
            else
            {
                leftJoint = leftWheel.GetComponent<HingeJoint>();
            }
            if (rightWheel == null || rightWheel.Equals(null))
            {
                Debug.LogWarning("right wheel transform is null");
            }
            else
            {
                rightJoint = rightWheel.GetComponent<HingeJoint>();
            }
        }

        private void InitializeMessage()
        {
            odom = new MessageTypes.Nav.Odometry();
            odom.header = new MessageTypes.Std.Header();
            odom.header.frame_id = FrameId;
            odom.child_frame_id = ChildId;
        }
        
        private void UpdateMessage()
        {
            odom.header.Update();
            setTwist(odom.twist);
            setPose(odom.pose);
            
            time_pass += Time.deltaTime;
            if (time_pass >= (1.0f / rate))
            {
                Publish(odom);
                if (tfBroadcasting && tfBroadcaster != null && !tfBroadcaster.Equals(null))
                {
                    tfBroadcaster.Write(odom);
                }
                time_pass = 0.0f;
            }
        }

        abstract protected void setPose(MessageTypes.Geometry.PoseWithCovariance pose);

        private void setTwist(MessageTypes.Geometry.TwistWithCovariance twist)
        {
            twist.twist.linear.x = (leftJoint.velocity + rightJoint.velocity) * Mathf.Deg2Rad * wheelRadius / 2.0f;
            twist.twist.angular.z = (leftJoint.velocity - rightJoint.velocity) * Mathf.Deg2Rad * wheelRadius / wheelSeparation;
        }
        
        protected void GetGeometryPoint(UnityEngine.Vector3 position, MessageTypes.Geometry.Point geometryPoint)
        {
            geometryPoint.x = position.x;
            geometryPoint.y = position.y;
            geometryPoint.z = position.z;
        }

        protected void GetGeometryQuaternion(UnityEngine.Quaternion quaternion, MessageTypes.Geometry.Quaternion geometryQuaternion)
        {
            geometryQuaternion.x = quaternion.x * Mathf.Deg2Rad;
            geometryQuaternion.y = quaternion.y * Mathf.Deg2Rad;
            geometryQuaternion.z = quaternion.z * Mathf.Deg2Rad;
            geometryQuaternion.w = - quaternion.w * Mathf.Deg2Rad;
        }
    }
}
