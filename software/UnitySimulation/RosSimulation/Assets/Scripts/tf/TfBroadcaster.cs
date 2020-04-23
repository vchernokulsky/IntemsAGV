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
    public class TfBroadcaster : MonoBehaviour
    {
        public string frameId = "base_link";
        public Transform publishedTransform;
        public float rate = 10.0f;
        
        private float time_pass;
        private TfPublisher tf;
        private TransformStamped msg;

        // Start is called before the first frame update
        void Start()
        {
            tf = GetComponent<TfPublisher>();
            time_pass = 0.0f;
        }

        // Update is called once per frame
        void Update()
        {
            time_pass += UnityEngine.Time.deltaTime;
            if (time_pass >= (1.0f / rate))
            {
                UpdateMsg();
               tf.SedTransform(msg);
               time_pass = 0.0f;
            }
        }
        
        private void InitializeMessage()
        {
            msg = new TransformStamped();
            msg.header = new Header();
            msg.header.frame_id = frameId;
            msg.child_frame_id = publishedTransform.name;

        }

        private void UpdateMsg()
        {
            msg.header.Update();
            msg.transform.translation = GetTranslation(publishedTransform.position.Unity2Ros());
            msg.transform.rotation = GetRotation(publishedTransform.rotation.Unity2Ros());
        }

        private Vector3 GetTranslation(UnityEngine.Vector3 _vector)
        {
            Vector3 trans = new Vector3();
            trans.x = _vector.x;
            trans.y = _vector.y;
            trans.z = _vector.z;
            
            return trans;
        }
        
        private Quaternion GetRotation(UnityEngine.Quaternion q)
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
