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
  TopicNameInput cmdVelInput;

  _RosTopic() {
    rosSerialNodeIpInput = IpInput(title: "Ros SerialNode IP");
    rosSerialNodePortInput = NumericInput(
        title: "Ros SerialNode PORT", minValue: 0, maxValue: 65555);
    cmdVelInput = TopicNameInput(title: "cmd_vel topic");
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
    if (!cmdVelInput.isCorrect()) {
      showBadToast("Can not save: Wrong cmd_vel topic name");
      return;
    }
    SocketData.serialNodeIp = rosSerialNodeIpInput.controller.text;
    SocketData.serialNodePort = rosSerialNodePortInput.controller.text;
    SocketData.cmdVelTopic = cmdVelInput.controller.text;
    showGoodToast("Data saved");
  }

  @override
  Widget build(BuildContext context) {
    rosSerialNodeIpInput.controller.text = SocketData.serialNodeIp;
    rosSerialNodePortInput.controller.text =  SocketData.serialNodePort;
    cmdVelInput.controller.text = SocketData.cmdVelTopic;

    return Column(
      children: <Widget>[
        rosSerialNodeIpInput,
        rosSerialNodePortInput,
        cmdVelInput,
        TopicNameInput(title: "odometry topic"),
        TopicNameInput(title: "base frame"),
        TopicNameInput(title: "odometry frame"),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            RaisedButton(child: Text("Reset"), onPressed: reset),
            RaisedButton(child: Text("Save"), onPressed: save),
          ],
        ),
      ],
    );
  }
}
