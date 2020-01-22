import 'package:flutter/cupertino.dart';

import 'DecimalInput.dart';
import 'NumericInput.dart';

class RobotGeometry extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        NumericInput(
          title: "Wheel radius(mm)",
          minValue: 10,
          maxValue: 100,
        ),
        NumericInput(
          title: "Wheel separation(mm)",
          minValue: 10,
          maxValue: 100,
        ),
        DecimalInput(
          title: "max linear velocity",
          minValue: 0.1,
          maxValue: 15.0,
        ),
        DecimalInput(
          title: "max angular velocity",
          minValue: 0.1,
          maxValue: 15.0,
        ),
      ],
    );
  }
}