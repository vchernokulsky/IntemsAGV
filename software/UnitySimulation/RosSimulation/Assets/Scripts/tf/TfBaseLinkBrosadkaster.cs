using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using UnityEngine;
using Quaternion = RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion;
using Vector3 = RosSharp.RosBridgeClient.MessageTypes.Geometry.Vector3;

public class TfBaseLinkBrosadkaster : TfTransformBroadcaster
{

    protected override void SetTransform()
    {
        msg.transform.translation = new Vector3();
        msg.transform.translation.z = publishedTransform.position.y;
        msg.transform.rotation = new Quaternion();
        msg.transform.rotation.w = 1.0;
    }
}
