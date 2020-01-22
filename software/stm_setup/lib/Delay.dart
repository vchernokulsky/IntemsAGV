import 'package:flutter/cupertino.dart';

import 'Inputs/NumericInput.dart';

class Delay extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        NumericInput(
          title: "wiznet check errors delay",
          minValue: 0,
          maxValue: 65555,
        ),
        NumericInput(
          title: "wiznet freeze timeout",
          minValue: 0,
          maxValue: 65555,
        ),
        NumericInput(
          title: "spin once delay",
          minValue: 0,
          maxValue: 65555,
        ),
        NumericInput(
          title: "set speed delay",
          minValue: 0,
          maxValue: 65555,
        ),
        NumericInput(
          title: "get tick delay",
          minValue: 0,
          maxValue: 65555,
        ),
      ],
    );
  }
}
