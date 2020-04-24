using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Nav;
using UnityEngine;

public class TfOdomBroadcaster : TfBroadcaster
{
    private Queue<Odometry> odomQueue;
    private Odometry curOdom;

    protected void Start()
    {
        base.Start();
        odomQueue = new Queue<Odometry>();
    }

    protected void FixedUpdate()
    {
        if (odomQueue.Count > 0)
        {
            curOdom = odomQueue.Dequeue();
            SendTransform();
        }
    }

    public void Write(Odometry _odometry)
    {
        odomQueue.Enqueue(_odometry);
    }

    protected override void SetChildFrame()
    {
        msg.header.frame_id = curOdom.header.frame_id;
        msg.child_frame_id = frameId;
    }

    protected override void SetTransform()
    {
        msg.transform.translation = GetTranslation(curOdom.pose.pose.position);
        msg.transform.rotation = GetRotation(curOdom.pose.pose.orientation);

    }
    

    
}
