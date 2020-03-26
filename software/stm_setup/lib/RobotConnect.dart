import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:stm_setup/Inputs/IpInput.dart';
import 'package:stm_setup/RobotSetUp.dart';
import 'package:stm_setup/SocketData.dart';

import 'Inputs/NumericInput.dart';
import 'ShowToast.dart';

class RobotConnect extends StatefulWidget {
  _RobotConnect createState() => _RobotConnect();
}

class _RobotConnect extends State<RobotConnect> {
  NumericInput portInput;
  IpInput ipInput;

  _RobotConnect() {
    ipInput = IpInput(title: "STM IP address");
    portInput = NumericInput(
      title: "STM host",
      minValue: 0,
      maxValue: 65555,
    );
  }

  void initState() {
    super.initState();
  }

  void connect() {
    if (ipInput.isCorrect()) {
      if (portInput.isCorrect()) {
        SocketData.connectHost = ipInput.controller.text;
        SocketData.connectPort = int.parse(portInput.controller.text);
        showGoodToast("Trying to connect...");
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
    ipInput.controller.text = SocketData.connectHost;
    portInput.controller.text = '${SocketData.connectPort}';

    return Scaffold(
      appBar: AppBar(title: Text("Connect to robot"),  leading: new Container(),),
      resizeToAvoidBottomInset : false,
      body: Scrollbar(
        child:
    Stack(
          children: <Widget>[
            ConstrainedBox (
              constraints: const BoxConstraints(minWidth: double.infinity,minHeight: double.infinity),
              child: Opacity(
                opacity: 0.11,
                child: Image.asset(
                  'images/back3.png',
                  fit: BoxFit.none,
                  repeat: ImageRepeat.repeat,
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.fromLTRB(16.0, 16.0, 16.0, 0.0),
              child: ListView(
                shrinkWrap: true,
                children: <Widget>[
                  Column(
                    children: <Widget>[
                      ipInput,
                      portInput,
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[
                          Expanded(
                            child: RaisedButton(
                              child: Text("Connect"),
                              onPressed: connect,
                              padding: EdgeInsets.symmetric(vertical: 15),
                            ),
                          )
                        ],
                      ),
                    ],
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
