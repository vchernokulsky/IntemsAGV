import 'package:flutter/cupertino.dart';

import 'DecimalInput.dart';
import 'NumericInput.dart';

class RobotDrive extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        NumericInput(
          title: "Max PWD allowed",
          minValue: 0,
          maxValue: 255,
        ),
        DecimalInput(
          title: "Rad per tick",
          minValue: 0.0,
          maxValue: 3.2,
        ),
      ],
    );
  }
}
