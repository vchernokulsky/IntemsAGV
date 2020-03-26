import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/NumericInput.dart';
import 'package:stm_setup/Inputs/TopicNameInput.dart';

import 'ShowToast.dart';
import 'SocketData.dart';

class RosTopic extends StatefulWidget {
  const RosTopic({Key key}) : super(key: key);

  _RosTopic createState() => _RosTopic();
}

class _RosTopic extends State<RosTopic> {
  IpInput rosSerialNodeIpInput;
  NumericInput rosSerialNodePortInput;
  TopicNameInput cmdVelTopicInput;
  TopicNameInput odomTopicInput;
  TopicNameInput baseFrameInput;
  TopicNameInput odomFrameInput;

  _RosTopic() {
    rosSerialNodeIpInput = IpInput(title: "Ros SerialNode IP");
    rosSerialNodePortInput = NumericInput(
        title: "Ros SerialNode PORT", minValue: 0, maxValue: 65555);
    cmdVelTopicInput = TopicNameInput(title: "cmd_vel topic");
    odomTopicInput = TopicNameInput(title: "odometry topic");
    baseFrameInput = TopicNameInput(title: "base frame");
    odomFrameInput = TopicNameInput(title: "odometry frame");
  }

  void initState() {
    super.initState();
  }

  void reset() {
    setState(() {
      showGoodToast("Data reseted");
    });
  }

  void save() {
    if (!rosSerialNodeIpInput.isCorrect()) {
      showBadToast("Can not save: Wrong ROS SerialNode IP address");
      return;
    }
    if (!rosSerialNodePortInput.isCorrect()) {
      showBadToast("Can not save: Wrong ROS SerialNode port");
      return;
    }
    if (!cmdVelTopicInput.isCorrect()) {
      showBadToast("Can not save: Wrong cmd_vel topic name");
      return;
    }
    if (!odomTopicInput.isCorrect()) {
      showBadToast("Can not save: Wrong odometry topic name");
      return;
    }
    if (!baseFrameInput.isCorrect()) {
      showBadToast("Can not save: Wrong base frame name");
      return;
    }
    if (!odomTopicInput.isCorrect()) {
      showBadToast("Can not save: Wrong odometry frame name");
      return;
    }
    SocketData.serialNodeIp = rosSerialNodeIpInput.controller.text;
    SocketData.serialNodePort = rosSerialNodePortInput.controller.text;
    SocketData.cmdVelTopic = cmdVelTopicInput.controller.text;
    SocketData.odomTopic = odomTopicInput.controller.text;
    SocketData.baseFrame = baseFrameInput.controller.text;
    SocketData.odomFrame = odomFrameInput.controller.text;
    showGoodToast("Data saved");
  }

  @override
  Widget build(BuildContext context) {
    rosSerialNodeIpInput.controller.text = SocketData.serialNodeIp;
    rosSerialNodePortInput.controller.text = SocketData.serialNodePort;
    cmdVelTopicInput.controller.text = SocketData.cmdVelTopic;
    odomTopicInput.controller.text = SocketData.odomTopic;
    baseFrameInput.controller.text = SocketData.baseFrame;
    odomFrameInput.controller.text = SocketData.odomFrame;

    return Column(
      children: <Widget>[
        rosSerialNodeIpInput,
        rosSerialNodePortInput,
        cmdVelTopicInput,
        odomTopicInput,
        baseFrameInput,
        odomFrameInput,
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal:2.0),
                child: RaisedButton(
                  child: Text("Reset"), onPressed: reset,
                  padding: EdgeInsets.symmetric(vertical: 15),),
              ),
            ),
            Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal:2.0),
                child: RaisedButton(
                  child: Text("Save"), onPressed: save,
                  padding: EdgeInsets.symmetric(vertical: 15),),
              ),
            ),
//            RaisedButton(child: Text("Reset"), onPressed: reset),
//            RaisedButton(child: Text("Save"), onPressed: save),
          ],
        )
      ],
    );
  }
}
