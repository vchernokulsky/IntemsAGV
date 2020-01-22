import 'package:flutter/cupertino.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';

import 'Inputs/DecimalInput.dart';
import 'Inputs/NumericInput.dart';

class Wiznet extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        MacInput(title: "STM MAC address"),
        IpInput(title: "STM IP address"),
        NumericInput(
          title: "STM host",
          minValue: 0,
          maxValue: 65555,
        ),
       IpInput(title: "network mask"),
       IpInput(title: "gate away"),
      ],
    );
  }
}
