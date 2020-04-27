﻿/*
© CentraleSupelec, 2017
Author: Dr. Jeremy Fix (jeremy.fix@centralesupelec.fr)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
<http://www.apache.org/licenses/LICENSE-2.0>.
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// Adjustments to new Publication Timing and Execution Framework
// © Siemens AG, 2018, Dr. Martin Bischoff (martin.bischoff@siemens.com)

using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    abstract public class CmdVelSubscriber : UnitySubscriber<MessageTypes.Geometry.Twist>
    {
        public float CmdVelTimeoutSec = 10.0f;
        public float maxLinVel = 1.9f;
        public float maxAngVel = 0.52f;
        
        protected float linearVelocity = 0.0f;
        protected float angularVelocity = 0.0f;
        
        private float msgTime;
        private bool hasMsg;
        
        protected override void Start()
        {
            base.Start();
            msgTime = Time.time;
            hasMsg = false;
        }

        protected override void ReceiveMessage(MessageTypes.Geometry.Twist message)
        {
            linearVelocity = Mathf.Clamp((float)message.linear.x, -maxLinVel, maxLinVel);
            angularVelocity = -Mathf.Clamp((float)message.angular.z, -maxAngVel, maxAngVel);
            hasMsg = true;
        }
        
        private void FixedUpdate()
        {
            CheckTime();
            MakeStep();
        }

        abstract protected void MakeStep();

        private void CheckTime()
        {
            if (hasMsg)
            {
                msgTime = Time.time;
                hasMsg = false;
            }
            if (Time.time - msgTime > CmdVelTimeoutSec)
            {
                linearVelocity = 0.0f;
                angularVelocity = 0.0f;
                msgTime = Time.time;
            }
        }
        
    }
}