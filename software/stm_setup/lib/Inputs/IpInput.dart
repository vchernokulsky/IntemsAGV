import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';
import 'package:hex/hex.dart';

class IpInput extends StatefulWidget {
  final String title;
  final controller;

  const IpInput({Key key, this.title, this.controller})
      : super(key: key);

  _IpInput createState() => _IpInput(title, controller);

  static bool isCorrect(String numStr) {
    RegExp regExp = new RegExp(
      r'^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$',
      caseSensitive: false,
      multiLine: false,
    );

      if (regExp.hasMatch(numStr)) {
        return true;
      } else {
        return false;
      }
  }
  static Uint8List stringToBytes(String string){
    Uint8List ret = Uint8List.fromList([0, 0, 0, 0]);
    List<String> retStr = string.split(".");
    if (retStr.length == 4){
      ret = Uint8List.fromList(retStr.map(int.parse).toList());
    }
    ret.forEach((i) => print("ip $i")) ;
    return ret;
  }
  static String bytesToString(Uint8List data){
    String ret = "";
    if(data.length == 4) {
      data.forEach((i) => ret += '$i.');
      ret = ret.substring(0, ret.length - 1);
    }
    return ret;
  }
}

class _IpInput extends State<IpInput> {
  _IpInput(this.title, this.controller);

  final String title;
  final controller;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    setState(() {
      if (numStr.isEmpty || IpInput.isCorrect(numStr)) {
        errorMsg = "";
      } else {
        errorMsg = "wrong ip";
      }
    });
  }

  Widget build(BuildContext context) {
    numberValidate(controller.text);
    return Center(
      child: Column(
        children: <Widget>[
          TextField(
            controller: controller,
            decoration: InputDecoration(
              labelText: "$title",
              border: OutlineInputBorder(),
            ),
            keyboardType: TextInputType.number,
            onSubmitted: (text) {
              numberValidate(text);
            },
            inputFormatters: <TextInputFormatter>[
              WhitelistingTextInputFormatter(RegExp(r'[0-9]|\.'))
            ], // Only numbers can be entered
          ),
          Padding(
            padding: EdgeInsets.all(5.0),
            child: Text(
              "$errorMsg",
              style: TextStyle(
                  color: Colors.red.withOpacity(1.0),
                  fontWeight: FontWeight.bold),
            ),
          ),
        ],
      ),
    );
  }
}
