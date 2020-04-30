using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraKeyControl : MonoBehaviour
{
    
    [Header("Translate")]
    public float mainSpeed = 100.0f; //regular speed
    public float speedUp = 250.0f; //multiplied by how long shift is held.  Basically running
    public float maxSpeed = 1000.0f; //Maximum speed when holdin gshift
   
    [Header("Rotate")]
    public float mouseAngularFactor = 0.25f; //How sensitive it with mouse
    public float arrowRotateStep = 1.0f;
    
    [Header("Follow to")]
    public Transform robot;
    public float offsetX = 0.0f;
    public float offsetY = 0.5f;
    public float offsetZ = -1.0f;
    
    private Vector3 lastMouse = new Vector3(255, 255, 255); //kind of in the middle of the screen, rather than at the top (play)
    private Vector3 curMouse = new Vector3(255, 255, 255); //kind of in the middle of the screen, rather than at the top (play)
    private float totalRun = 1.0f;
     
    void Update () {
        if (Input.GetKey(KeyCode.Space))
        {
            GoToRobot();
            return;
        }

        if (ArrowPressed())
        {
            RotateAroundViewPoint();
            return;
        }
        
        SetPosition();
        transform.eulerAngles = GetOrientation();
        
    }

    private void GoToRobot()
    {
        if (robot != null && !robot.Equals(null))
        {
            Vector3 pos = new Vector3(robot.position.x + offsetX, robot.position.y + offsetY, robot.position.z + offsetZ);
            transform.SetPositionAndRotation(pos, new Quaternion());
            transform.LookAt(robot);
        }
    }

    private bool ArrowPressed()
    {
        return Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.UpArrow) ||
            Input.GetKey(KeyCode.DownArrow);
    }

    private void RotateAroundViewPoint()
    {
        float z_cos = Mathf.Cos(transform.eulerAngles.z);
        float len = (z_cos == 0)? 0 : transform.position.y / z_cos;
        Vector3 temp = transform.TransformPoint(Vector3.forward * len);
        transform.RotateAround(temp, RotationAxis(), arrowRotateStep);
        transform.LookAt(temp);
    }

    private Vector3 RotationAxis()
    {
        Vector3 ret = new Vector3();
        if (Input.GetKey(KeyCode.RightArrow))
        {
            ret = Vector3.down;
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            ret = Vector3.up;
        }
        if (Input.GetKey(KeyCode.UpArrow))
        {
            ret = Vector3.forward;
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
           ret = Vector3.back;
        }

        return ret;
    }
    
    private void SetPosition()
    {
        
        Vector3 p = GetBaseInput();
        if (Input.GetKey (KeyCode.LeftShift)){
            totalRun += Time.deltaTime;
            p  = p * totalRun * speedUp;
            p.x = Mathf.Clamp(p.x, -maxSpeed, maxSpeed);
            p.y = Mathf.Clamp(p.y, -maxSpeed, maxSpeed);
            p.z = Mathf.Clamp(p.z, -maxSpeed, maxSpeed);
        }
        else{
            totalRun = Mathf.Clamp(totalRun * 0.5f, 1f, 1000f);
            p = p * mainSpeed;
        }
        p = p * Time.deltaTime;
        transform.Translate(p);
    }

    private Vector3 GetOrientation()
    {
        lastMouse = curMouse;
        curMouse = Input.mousePosition;
        return new Vector3(transform.eulerAngles.x - mouseAngularFactor * (curMouse.y - lastMouse.y) , 
            transform.eulerAngles.y + mouseAngularFactor * (curMouse.x - lastMouse.x), 0);
    }
    private Vector3 GetRotateInput() { //returns the basic values, if it's 0 than it's not active.
        if (Input.GetKey (KeyCode.A)){
            return Vector3.up;
        }
        if (Input.GetKey (KeyCode.D)){
            return Vector3.down;
        }
        return new Vector3();
    }
     
    private Vector3 GetBaseInput() { //returns the basic values, if it's 0 than it's not active.
        Vector3 p_Velocity = new Vector3();
        if (Input.GetKey (KeyCode.W)){
            p_Velocity += Vector3.forward;
        }
        if (Input.GetKey (KeyCode.S)){
            p_Velocity += Vector3.back;
        }
        if (Input.GetKey (KeyCode.A)){
            p_Velocity += Vector3.left;
        }
        if (Input.GetKey (KeyCode.D)){
            p_Velocity += Vector3.right;
        }
        if (Input.GetKey (KeyCode.E)){
            p_Velocity += Vector3.up;
        }
        if (Input.GetKey (KeyCode.Q)){
            p_Velocity += Vector3.down;
        }
        return p_Velocity;
    }
}


