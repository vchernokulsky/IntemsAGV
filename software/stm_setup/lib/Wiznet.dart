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
  NumericInput localRosPort;
  NumericInput setUpServerPort;

  _Wiznet() {
    ipInput = IpInput(title: "Robot IP");
    networkMaskInput = IpInput(title: "network mask");
    gateAwayInput = IpInput(title: "gate away");
    localRosPort = NumericInput(
        title: "Roboto ROS client port", minValue: 0, maxValue: 65555);
    setUpServerPort =
        NumericInput(title: "Robot SetUpServer port", minValue: 0, maxValue: 65555);
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
    if (!ipInput.isCorrect()) {
      showBadToast("Can not save: Wrong STM IP address");
      return;
    }
    if (!networkMaskInput.isCorrect()) {
      showBadToast("Can not save: Wrong network mask");
      return;
    }
    if (!gateAwayInput.isCorrect()) {
      showBadToast("Can not save: Wrong gate away");
      return;
    }
    if (!localRosPort.isCorrect()) {
      showBadToast("Can not save: Wrong STM ros client port");
      return;
    }
    if (!setUpServerPort.isCorrect()) {
      showBadToast("Can not save: Wrong setup server port");
      return;
    }
    SocketData.localIpAddress = ipInput.controller.text;
    SocketData.networkMask = networkMaskInput.controller.text;
    SocketData.gateAway = gateAwayInput.controller.text;
    SocketData.rosClientPort = localRosPort.controller.text;
    SocketData.setupServerPort = setUpServerPort.controller.text;
    showGoodToast("Data saved");
  }

  @override
  Widget build(BuildContext context) {
    ipInput.controller.text = SocketData.localIpAddress;
    networkMaskInput.controller.text = SocketData.networkMask;
    gateAwayInput.controller.text = SocketData.gateAway;
    localRosPort.controller.text = SocketData.rosClientPort;
    setUpServerPort.controller.text = SocketData.setupServerPort;
    return Column(
      children: <Widget>[
        ipInput,
        localRosPort,
        setUpServerPort,
        networkMaskInput,
        gateAwayInput,
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal:2.0),
                child: RaisedButton(
                  child: Text("Reset"), onPressed: reset,
                  padding: EdgeInsets.symmetric(vertical: 15),),
              ),
            ),
            Expanded(
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal:2.0),
                child: RaisedButton(
                  child: Text("Save"), onPressed: save,
                  padding: EdgeInsets.symmetric(vertical: 15),),
              ),
            ),
//            RaisedButton(child: Text("Reset"), onPressed: reset),
//            RaisedButton(child: Text("Save"), onPressed: save),
          ],
        )
      ],
    );
  }
}
