/*
 * Copyright (C) 2011 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

package org.ros.android.controllerSample;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import org.ros.address.InetAddressFactory;
import org.ros.android.MasterChooser;
import org.ros.android.MessageCallable;
import org.ros.android.MessagePub;
import org.ros.android.MessageSub;
import org.ros.android.RosActivity;
import org.ros.node.NodeConfiguration;
import org.ros.node.NodeMainExecutor;

import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import nav_msgs.Odometry;


public class MainActivity extends RosActivity {

  private volatile boolean publishLinVelocity;
  private volatile boolean publishAngVelocity;


  private Context mContext;


  private MessageSub<nav_msgs.Odometry> subOdometry;
  private MessagePub<geometry_msgs.Twist> pubTwist;
  double twistLinear;
  double twistAngular;

  SeekBar seekBar;
  TextView textView;

  TextView x_coord;
  TextView y_coord;
  TextView lin_vel;
  TextView ang_vel;
  View root_view;

  SeekBar seekBar2;
  TextView textView2;
  int min = 0, max = 100, current = 0;


  public MainActivity() {
    // The RosActivity constructor configures the notification title and ticker
    // messages.
    super("android controller", "android controller");
  }

  @SuppressWarnings("unchecked")
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);

    mContext = getApplicationContext();
    root_view = findViewById(android.R.id.content).getRootView();

    textView = findViewById(R.id.textView);
    seekBar = findViewById(R.id.seekBar);

    x_coord = findViewById(R.id.x_coord);
    y_coord = findViewById(R.id.y_coord);
    lin_vel = findViewById(R.id.lin_vel);
    ang_vel = findViewById(R.id.ang_vel);

    seekBar.setProgress(max - min);
    seekBar.setProgress(current - min);
    textView.setText("" + current);

    textView2 = findViewById(R.id.textView2);
    seekBar2 = findViewById(R.id.seekBar2);
    textView2.setText("" + 0);

    seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
      @Override
      public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        twistLinear = (double)progress / 100.0;
        textView.setText("" + progress);
      }

      @Override
      public void onStartTrackingTouch(SeekBar seekBar) {
        publishLinVelocity = true;
      }

      @Override
      public void onStopTrackingTouch(SeekBar sBar) {
        seekBar.setProgress(0);
        twistLinear = 0;
        setTwist();
        publishLinVelocity = false;
      }
    });

    seekBar2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
      @Override
      public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        twistAngular = - (double)progress / 100.0;
        textView2.setText("" + progress);
      }

      @Override
      public void onStartTrackingTouch(SeekBar seekBar) {
        publishAngVelocity = true;
      }

      @Override
      public void onStopTrackingTouch(SeekBar sBar) {
        seekBar2.setProgress(0);
        twistAngular = 0;
        setTwist();
        publishAngVelocity = false;
      }
    });


    startPub();
  }

  @Override
  protected void init(NodeMainExecutor nodeMainExecutor) {

    NodeConfiguration nodeConfiguration =
            NodeConfiguration.newPublic(InetAddressFactory.newNonLoopback().getHostAddress(),
                    getMasterUri());

    ///////////////subscribe Odometry//
    subOdometry =  new MessageSub<nav_msgs.Odometry>(mContext) ;
    subOdometry.setTopicName(MasterChooser.odom_topic);
    subOdometry.setMessageType(nav_msgs.Odometry._TYPE);

    subOdometry.setMessageCallable(new MessageCallable< String, Odometry>() {
      @Override
      public String call(nav_msgs.Odometry message) {

       setOdometry(message);
        return null ;
      }
    });
    nodeMainExecutor.execute(subOdometry,
            nodeConfiguration.setNodeName("android/subscribe_odom"));



    ////////////////publish Twist //
    pubTwist =  new MessagePub<geometry_msgs.Twist>(mContext) ;
    pubTwist.setTopicName(MasterChooser.cmd_vel_topic);
    pubTwist.setMessageType(geometry_msgs.Twist._TYPE);
    nodeMainExecutor.execute(pubTwist,
            nodeConfiguration.setNodeName("android/publish_twist"));

  }


  public void setTwist(){

      pubTwist.message.getLinear().setX(twistLinear);
      pubTwist.message.getLinear().setY(0);
      pubTwist.message.getLinear().setZ(0);
      pubTwist.message.getAngular().setX(0);
      pubTwist.message.getAngular().setY(0);
      pubTwist.message.getAngular().setZ(twistAngular);
      pubTwist.publish();

  }

  public void setTwist(double[] lin, double[] b){

    pubTwist.message.getLinear().setX(0);
    pubTwist.message.getLinear().setY(0);
    pubTwist.message.getLinear().setZ(0);
    pubTwist.message.getAngular().setX(0);
    pubTwist.message.getAngular().setY(0);
    pubTwist.message.getAngular().setZ(0);
    pubTwist.publish();

  }
  public void setOdometry(final nav_msgs.Odometry message) {

 root_view.post(new Runnable() {
   @Override
   public void run() {
     x_coord.setText(String.format("%.2f", message.getPose().getPose().getPosition().getX()));
     y_coord.setText(String.format("%.2f", message.getPose().getPose().getPosition().getY()));
     lin_vel.setText(String.format("%.2f", message.getTwist().getTwist().getLinear().getX()));
     ang_vel.setText(String.format("%.2f",  message.getTwist().getTwist().getAngular().getZ()));

   }
 });

  }

  public void startPub() {

    Timer publisherTimer = new Timer();
    publisherTimer.schedule(new TimerTask() {
      @Override
      public void run() {
        if (publishLinVelocity || publishAngVelocity) {
          setTwist(  );
        }
      }
    }, 0, 80);
  }
}
