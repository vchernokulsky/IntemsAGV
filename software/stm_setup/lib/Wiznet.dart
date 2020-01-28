import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';
import 'package:stm_setup/SocketData.dart';

import 'Inputs/NumericInput.dart';

class Wiznet extends StatefulWidget {

  const Wiznet({Key key}) : super(key: key);

  _Wiznet createState() => _Wiznet();
}

class _Wiznet extends State<Wiznet> {
  var macAddressController;


  _Wiznet() {
    macAddressController = TextEditingController();
  }

  void initState() {
    super.initState();
    macAddressController.text = SocketData.macAddress;
  }

  void reset() {
    setState(() {
      macAddressController.text = SocketData.macAddress;
    });
  }

  void save(){

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
        RaisedButton(child: Text("Reset"), onPressed: reset),
      ],
    );
  }
}
