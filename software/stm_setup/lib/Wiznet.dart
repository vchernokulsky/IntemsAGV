import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/SocketData.dart';

import 'Inputs/NumericInput.dart';
import 'ShowToast.dart';

class Wiznet extends StatefulWidget {
  const Wiznet({Key key}) : super(key: key);

  _Wiznet createState() => _Wiznet();
}

class _Wiznet extends State<Wiznet> {
  var networkMaskController;
  var gateAwayController;

  IpInput ipInput;

  _Wiznet() {
    networkMaskController = TextEditingController();
    gateAwayController = TextEditingController();

    ipInput =  IpInput(title: "STM IP address");

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
    if (ipInput.isCorrect()) {
      SocketData.localIpAddress = ipInput.controller.text;
      showGoodToast("Data saved");
    } else {
      showBadToast("Can not save: wrong mac address");
    }
  }

  @override
  Widget build(BuildContext context) {
    ipInput.controller.text = SocketData.localIpAddress;
    return Column(
      children: <Widget>[
        ipInput,
        NumericInput(
          title: "STM host",
          minValue: 0,
          maxValue: 65555,
        ),
        IpInput(
          title: "network mask",
        ),
        IpInput(
          title: "gate away",
        ),
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
