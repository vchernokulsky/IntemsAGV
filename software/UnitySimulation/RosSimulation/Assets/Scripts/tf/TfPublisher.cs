using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Rosapi;
using RosSharp.RosBridgeClient.MessageTypes.Tf2;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class TfPublisher : UnityPublisher<TFMessage>
    {
        private TFMessage msg;

        private Queue<TransformStamped> msgQueue;
        
        // Start is called before the first frame update
        protected override void Start()
        {
            Topic = "/tf";
            msgQueue = new Queue<TransformStamped>();
            msg = new TFMessage();
            msg.transforms = new TransformStamped[1];
            base.Start();
            
        }

        // Update is called once per frame
        protected void FixedUpdate()
        {
            if (msgQueue.Count > 0)
            {
                msg.transforms[0] = msgQueue.Dequeue();
                Publish(msg);
            }
        }

        public void SendTransform(TransformStamped tf_msg)
        {
           msgQueue.Enqueue(tf_msg);
        }
        
        
    }
}
