using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using UnityEngine;

public class CmdVelWheelSub : CmdVelSubscriber
{
    public JoyAxisJointMotorWriter leftWheel;
    public JoyAxisJointMotorWriter rightWheel;

    public float radius = 0.035f;
    public float wheelSeparation = 0.23f;
    
    private float leftLin = 0.0f;
    private float rigthLin = 0.0f;
    private float magikCoeff = 1.0f;
    
    protected override void MakeStep()
    {
        calculate_lin_speeds();
        rightWheel.WriteRad( -rigthLin / radius);
        leftWheel.WriteRad( -leftLin / radius);
    }
    void adjust_speeds()
    {
        if ((leftLin > maxLinVel)||(rigthLin > maxLinVel))
        {
            float factor = maxLinVel / Mathf.Max(leftLin, rigthLin);
            leftLin *= factor;
            rigthLin *= factor;
        }
        if ((leftLin < -maxLinVel)||(rigthLin < -maxLinVel))
        {
            float factor = maxLinVel / Mathf.Min(leftLin, rigthLin);
            leftLin *= factor;
            rigthLin *= factor;
        }
    }
    void calculate_lin_speeds(){
        leftLin = (2.0f * linearVelocity + wheelSeparation * angularVelocity) / 2.0f;
        rigthLin = (2.0f * linearVelocity - wheelSeparation * angularVelocity) / 2.0f;
        adjust_speeds();
    }
}
