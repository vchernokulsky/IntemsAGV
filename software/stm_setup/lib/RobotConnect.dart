import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';
import 'package:stm_setup/RobotSetUp.dart';
import 'package:stm_setup/SocketData.dart';
import 'package:ping_discover_network/ping_discover_network.dart';

import 'Inputs/NumericInput.dart';
import 'ShowToast.dart';

class RobotConnect extends StatefulWidget {
  _RobotConnect createState() => _RobotConnect();
}

class _RobotConnect extends State<RobotConnect> {
  var ipAddressController;
  var portController;

  NumericInput portInput;

  _RobotConnect() {
    ipAddressController = TextEditingController();
    portController = TextEditingController();
  }

  void initState() {
    super.initState();
  }

  void connect() {
    if (IpInput.isCorrect(ipAddressController.text)){
      if( NumericInput.isCorrect(portController.text, portInput.minValue, portInput.maxValue)){
        SocketData.connectHost = ipAddressController.text;
        SocketData.connectPort = int.parse(portController.text);
        showGoodToast("Data saved");
        Navigator.push(
          context,
          MaterialPageRoute(builder: (context) => RobotSetUp()),
        );
      } else {
        showBadToast("Wrong Robot PORT");
      }
    } else {
      showBadToast("Wrong Robot HOST");
    }

  }

  @override
  Widget build(BuildContext context) {
    ipAddressController.text = SocketData.connectHost;
    portController.text = '${SocketData.connectPort}';

    portInput = NumericInput(title: "STM host",
      controller: portController,
      minValue: 0,
      maxValue: 65555,);

    return Scaffold(
      appBar: AppBar(title: Text("Set up connection to robot")),
      body: Scrollbar(
        child: Padding(
          padding: EdgeInsets.fromLTRB(16.0, 16.0, 16.0, 0.0),
          child: ListView(
            shrinkWrap: true,
            children: <Widget>[
              Column(
                children: <Widget>[
                  IpInput(
                      title: "STM IP address", controller: ipAddressController),
                  portInput,
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: <Widget>[
                      RaisedButton(child: Text("Connect"), onPressed: connect),
                    ],
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
