using System.Collections;
using System.Collections.Generic;
using RosSharp.RosBridgeClient;
using RosSharp.RosBridgeClient.MessageTypes.Std;
using RosSharp.Urdf;
using UnityEditor;
using UnityEngine;
using RosSharp.RosBridgeClient.MessageTypes.Sensor;

public class GpsPublisher : UnityPublisher<NavSatFix>
{
    public double zeroLat = 60.128703;
    public double zeroLon = 30.442558;
    public string frameId = "gps";
    public float rate = 10.0f;

    public Transform robot;
    
    private const double EarthRadius = 6371000;

    private double Lat;
    private double Lon;

    private NavSatFix msg;
    private float time_pass;
    
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        InitializeMessage();
        time_pass = 0.0f;
    }
    private void FixedUpdate()
    {
        UpdateMessage();
    }
    
    private void UpdateMessage()
    {
        msg.header.Update();
        UpdateLL();

        time_pass += UnityEngine.Time.deltaTime;
        if (time_pass >= (1.0f / rate))
        {
            msg.latitude = Lat;
            msg.longitude = Lon;
            Publish(msg);
            time_pass = 0.0f;
        }
    }

    // Update is called once per frame
    void UpdateLL()
    {
        Lat = zeroLat + robot.position.z / EarthRadius;
        Lon = zeroLon + robot.position.x / EarthRadius;
    }

    private void InitializeMessage()
    {
        msg = new NavSatFix();
        msg.header = new Header();
        msg.header.frame_id = frameId;
        msg.status.status = NavSatStatus.STATUS_FIX;
        msg.status.service = NavSatStatus.SERVICE_GPS;
        msg.position_covariance_type = NavSatFix.COVARIANCE_TYPE_UNKNOWN;
    }
}
