using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Rosapi;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class TfPublisher : UnityPublisher<MessageTypes.Geometry.TransformStamped>
    {
        private TransformStamped msg;

        private Queue<TransformStamped> msgQueue;
        
        // Start is called before the first frame update
        protected override void Start()
        {
            Topic = "/tf";
            msgQueue = new Queue<TransformStamped>();
            base.Start();
        }

        // Update is called once per frame
        protected void FixUpdate()
        {
            if (msgQueue.Count > 0)
            {
                msg = msgQueue.Dequeue();
                Publish(msg);
            }
        }

        public void SedTransform(TransformStamped tf_msg)
        {
           msgQueue.Enqueue(tf_msg);
        }
        
        
    }
}
