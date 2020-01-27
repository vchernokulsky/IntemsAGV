import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';

import 'Inputs/NumericInput.dart';

class Wiznet extends StatefulWidget {
  final String macAddress;

  const Wiznet({Key key, this.macAddress}) : super(key: key);

  _Wiznet createState() => _Wiznet(macAddress);
}

class _Wiznet extends State<Wiznet> {
  final String macAddress;
  var macAddressController;

  _Wiznet(this.macAddress) {
    macAddressController = TextEditingController();
  }

  void initState() {
    super.initState();
    macAddressController.text = macAddress;
  }

  void updateState() {
    setState(() {
      macAddressController.text = macAddress;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: <Widget>[
        MacInput(title: "STM MAC address", controller: macAddressController),
        IpInput(title: "STM IP address"),
        NumericInput(
          title: "STM host",
          minValue: 0,
          maxValue: 65555,
        ),
        IpInput(title: "network mask"),
        IpInput(title: "gate away"),
        RaisedButton(child: Text("Reset"), onPressed: updateState),
      ],
    );
  }
}
