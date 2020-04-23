using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Geometry;
using RosSharp.RosBridgeClient.MessageTypes.Std;
using RosSharp.Urdf;
using UnityEngine;
using Quaternion = RosSharp.RosBridgeClient.MessageTypes.Geometry.Quaternion;
using Transform = UnityEngine.Transform;
using Vector3 = RosSharp.RosBridgeClient.MessageTypes.Geometry.Vector3;


namespace RosSharp.RosBridgeClient
{
    [RequireComponent(typeof(TfPublisher))]
    abstract public class TfBroadcaster : MonoBehaviour
    {
        public string frameId = "base_link";
        public float rate = 10.0f;
        
        private float time_pass;
        private TfPublisher tf;
        protected TransformStamped msg;

        // Start is called before the first frame update
        protected void Start()
        {
            tf = GetComponent<TfPublisher>();
            time_pass = 0.0f;
            InitializeMessage();
        }

        // Update is called once per frame
        protected void FixedUpdate()
        {
            time_pass += UnityEngine.Time.deltaTime;
            if (time_pass >= (1.0f / rate))
            {
                UpdateMsg(); 
                tf.SendTransform(msg);
               time_pass = 0.0f;
            }
        }
        
        private void InitializeMessage()
        {
            msg = new TransformStamped();
            msg.header = new Header();
            msg.header.frame_id = frameId;
        }

        abstract protected void SetChildFrame();
        abstract protected void SetTransform();

        private void UpdateMsg()
        {
            msg.header.Update();
            SetChildFrame();
            SetTransform();
        }

        protected Vector3 GetTranslation(UnityEngine.Vector3 _vector)
        {
            Vector3 trans = new Vector3();
            trans.x = _vector.x;
            trans.y = _vector.y;
            trans.z = _vector.z;
            
            return trans;
        }
        
        protected Quaternion GetRotation(UnityEngine.Quaternion q)
        {
            Quaternion quat = new Quaternion();
            quat.w = q.w;
            quat.x = q.x;
            quat.y = q.y;
            quat.z = q.z;
            return quat;
        }
    }
}
