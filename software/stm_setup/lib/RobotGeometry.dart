import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'Inputs/DecimalInput.dart';
import 'Inputs/NumericInput.dart';
import 'ShowToast.dart';
import 'SocketData.dart';

class RobotGeometry extends StatefulWidget {
  const RobotGeometry({Key key}) : super(key: key);

  _RobotGeometry createState() => _RobotGeometry();
}

class _RobotGeometry extends State<RobotGeometry> {
  NumericInput wheelRadius;
  NumericInput wheelSeparation;
  DecimalInput maxLinVelocity;
  DecimalInput maxAngVelocity;
  NumericInput maxPwdAllowed;
  DecimalInput radPerTick;

  _RobotGeometry() {
    wheelRadius =
        NumericInput(title: "Wheel radius(mm)", minValue: 10, maxValue: 500);
    wheelSeparation = NumericInput(
        title: "Wheel separation(mm)", minValue: 10, maxValue: 2000);
    maxLinVelocity = DecimalInput(
        title: "max linear velocity", minValue: 0.1, maxValue: 30.0);
    maxAngVelocity = DecimalInput(
        title: "max angular velocity", minValue: 0.1, maxValue: 10.0);
    maxPwdAllowed =
        NumericInput(title: "Max PWD allowed", minValue: 0, maxValue: 255);
    radPerTick =
        DecimalInput(title: "Rad per tick", minValue: 0.0, maxValue: 0.6, decimal: 5);
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
    if (!wheelRadius.isCorrect()) {
      showBadToast("Can not save: Wrong wheel radius");
      return;
    }
    if (!wheelSeparation.isCorrect()) {
      showBadToast("Can not save: Wrong wheel separation");
      return;
    }
    if (!maxLinVelocity.isCorrect()) {
      showBadToast("Can not save: Wrong max linear velocity");
      return;
    }
    if (!maxAngVelocity.isCorrect()) {
      showBadToast("Can not save: Wrong max angular velocity");
      return;
    }
    if (!maxPwdAllowed.isCorrect()) {
      showBadToast("Can not save: Wrong max pwd allowed");
      return;
    }
    if (!radPerTick.isCorrect()) {
      showBadToast("Can not save: Wrong radius per tick");
      return;
    }

    SocketData.wheelRadius = wheelRadius.controller.text;
    SocketData.wheelSeparation = wheelSeparation.controller.text;
    SocketData.maxLinVelocity = maxLinVelocity.controller.text;
    SocketData.maxAngVelocity = maxAngVelocity.controller.text;
    SocketData.maxPwdAllowed = maxPwdAllowed.controller.text;
    SocketData.radPerTick = radPerTick.controller.text;
    showGoodToast("Data saved");
  }

  @override
  Widget build(BuildContext context) {
    wheelRadius.controller.text = SocketData.wheelRadius;
    wheelSeparation.controller.text = SocketData.wheelSeparation;
    maxLinVelocity.controller.text = SocketData.maxLinVelocity;
    maxAngVelocity.controller.text = SocketData.maxAngVelocity;
    maxPwdAllowed.controller.text = SocketData.maxPwdAllowed;
    radPerTick.controller.text = SocketData.radPerTick;
    return Column(
      children: <Widget>[
        wheelRadius,
        wheelSeparation,
        maxLinVelocity,
        maxAngVelocity,
        maxPwdAllowed,
        radPerTick,
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            RaisedButton(child: Text("Reset"), onPressed: reset),
            RaisedButton(child: Text("Save"), onPressed: save),
          ],
        )
      ],
    );
  }
}
