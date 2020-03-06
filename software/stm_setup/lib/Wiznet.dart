import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';
import 'package:stm_setup/SocketData.dart';

import 'Inputs/NumericInput.dart';

class Wiznet extends StatefulWidget {
  const Wiznet({Key key}) : super(key: key);

  _Wiznet createState() => _Wiznet();
}

class _Wiznet extends State<Wiznet> {
  var ipAddressController;
  var networkMaskController;
  var gateAwayController;

  _Wiznet() {
    ipAddressController = TextEditingController();
    networkMaskController = TextEditingController();
    gateAwayController = TextEditingController();
  }

  void initState() {
    super.initState();
  }

  void showGoodToast(String text) {
    Fluttertoast.showToast(
        msg: "$text",
        toastLength: Toast.LENGTH_SHORT,
        gravity: ToastGravity.BOTTOM,
        timeInSecForIos: 1,
        backgroundColor: Colors.blueGrey,
        textColor: Colors.white,
        fontSize: 16.0);
  }

  void showBadToast(String text) {
    Fluttertoast.showToast(
        msg: "$text",
        toastLength: Toast.LENGTH_SHORT,
        gravity: ToastGravity.BOTTOM,
        timeInSecForIos: 1,
        backgroundColor: Colors.redAccent[700],
        textColor: Colors.white,
        fontSize: 16.0);
  }

  void reset() {
    setState(() {
      showGoodToast("Data reseted");
    });
  }

  void save() {
    if (IpInput.isCorrect(ipAddressController.text)) {
      SocketData.localIpAddress = ipAddressController.text;
      showGoodToast("Data saved");
    } else {
      showBadToast("Can not save: wrong mac address");
    }
  }

  @override
  Widget build(BuildContext context) {
    ipAddressController.text = SocketData.localIpAddress;
    return Column(
      children: <Widget>[
        IpInput(title: "STM IP address", controller: ipAddressController),
        NumericInput(
          title: "STM host",
          minValue: 0,
          maxValue: 65555,
        ),
        IpInput(
          title: "network mask",
          controller: networkMaskController,
        ),
        IpInput(
          title: "gate away",
          controller: gateAwayController,
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
