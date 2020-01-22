import 'package:flutter/cupertino.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/NumericInput.dart';
import 'package:stm_setup/Inputs/TopicNameInput.dart';

class RosTopic extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        IpInput(title: "Ros master IP"),
        NumericInput(
          title: "Ros master PORT",
          minValue: 0,
          maxValue: 65555,
        ),
        TopicNameInput(title: "cmd_vel topic"),
        TopicNameInput(title: "odometry topic"),
        TopicNameInput(title: "base frame"),
        TopicNameInput(title: "odometry frame"),
      ],
    );
  }
}
