import 'package:flutter/cupertino.dart';
import 'package:stm_setup/TopicNameInput.dart';

class RosTopic extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        TopicNameInput(
          title: "cmd_vel topic",
        ),
        TopicNameInput(
          title: "odometry topic",
        ),
        TopicNameInput(
          title: "base frame",
        ),
        TopicNameInput(
          title: "odometry frame",
        ),
      ],
    );
  }
}
