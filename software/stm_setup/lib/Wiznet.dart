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
  IpInput ipInput;
  IpInput networkMaskInput;
  IpInput gateAwayInput;

  _Wiznet() {
    ipInput = IpInput(title: "STM IP address");
    networkMaskInput = IpInput(title: "network mask");
    gateAwayInput = IpInput(title: "gate away");
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
      SocketData.networkMask = networkMaskInput.controller.text;
      SocketData.gateAway = gateAwayInput.controller.text;
      showGoodToast("Data saved");
    } else {
      showBadToast("Can not save: wrong mac address");
    }
  }

  @override
  Widget build(BuildContext context) {
    ipInput.controller.text = SocketData.localIpAddress;
    networkMaskInput.controller.text = SocketData.networkMask;
    gateAwayInput.controller.text = SocketData.gateAway;
    return Column(
      children: <Widget>[
        ipInput,
        NumericInput(
          title: "STM host",
          minValue: 0,
          maxValue: 65555,
        ),
        networkMaskInput,
        gateAwayInput,
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
