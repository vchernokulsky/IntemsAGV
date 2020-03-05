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


  var macAddressController;


  _Wiznet() {
    macAddressController = TextEditingController();
  }

  void initState() {
    super.initState();
  }

  void showGoodToast(String text){
    Fluttertoast.showToast(
        msg: "$text",
        toastLength: Toast.LENGTH_SHORT,
        gravity: ToastGravity.BOTTOM,
        timeInSecForIos: 1,
        backgroundColor: Colors.blueGrey,
        textColor: Colors.white,
        fontSize: 16.0
    );
  }

  void showBadToast(String text){
    Fluttertoast.showToast(
        msg: "$text",
        toastLength: Toast.LENGTH_SHORT,
        gravity: ToastGravity.BOTTOM,
        timeInSecForIos: 1,
        backgroundColor: Colors.redAccent[700],
        textColor: Colors.white,
        fontSize: 16.0
    );
  }

  void reset() {
    setState(() {
      showGoodToast("Data reseted");
    });
  }

  void save(){
    if(MacInput.isCorrect(macAddressController.text)){
      SocketData.macAddress = macAddressController.text;
      showGoodToast("Data saved");
    } else {
      showBadToast("Can not save: wrong mac address");
    }
  }

  @override
  Widget build(BuildContext context) {
    macAddressController.text = SocketData.macAddress;
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
