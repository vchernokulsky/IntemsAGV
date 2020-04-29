using System.Collections;
using System.Collections.Generic;
using RosSharp;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Std;
using RosSharp.Urdf;
using UnityEngine;
using Transform = UnityEngine.Transform;

    public class TfTransformBroadcaster : TfBroadcaster
    {
        public Transform publishedTransform;
        
        protected override void SetChildFrame()
        {
            msg.child_frame_id = publishedTransform.name;
        }

        protected override void SetTransform()
        {
            msg.transform.translation = GetTranslation(publishedTransform.localPosition.Unity2Ros());
            msg.transform.rotation = GetRotation(publishedTransform.localRotation.Unity2Ros());
        }
        
    
}
