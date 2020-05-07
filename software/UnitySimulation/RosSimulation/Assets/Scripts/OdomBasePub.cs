using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using UnityEngine;
using Transform = UnityEngine.Transform;

namespace RosSharp.RosBridgeClient
{
    public class OdomBasePub : OdomPublisher
    {
        public Transform base_link;
        
        protected override void setPose(MessageTypes.Geometry.PoseWithCovariance pose)
        {
            GetGeometryPoint(base_link.position.Unity2Ros(), pose.pose.position);
            pose.pose.position.z = 0.0;
            GetGeometryQuaternion(base_link.rotation.Unity2Ros(), pose.pose.orientation);
        }

    }
}
