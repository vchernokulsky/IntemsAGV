import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';
import 'package:hex/hex.dart';

class MacInput extends StatefulWidget {
  final String title;
  final String value;
  final TextEditingController controller = TextEditingController();

  MacInput({Key key, this.title, this.value})
      : super(key: key);

  _MacInput createState() => _MacInput(title, value, controller);

  bool isCorrect(String numStr){
    RegExp regExp = new RegExp(
      r'^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$',
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
    Uint8List ret = HEX.decode( string.replaceAll(":", ""));
    return ret;
  }

  static String bytesToString(Uint8List data){
    String mac = "";
    if(data.length == 6) {
      data.forEach((i) => mac += '${HEX.encode([i])}:');
      mac = mac.substring(0, mac.length - 1);
    }
    return mac;
  }
}

class _MacInput extends State<MacInput> {
  final String title;
  final String value;
  String errorMsg;
  final controller;

  _MacInput(this.title, this.value, this.controller);

  void initState() {
    super.initState();
    errorMsg = "";
  }



  void numberValidate(String numStr) {
    setState(() {
      if (numStr.isEmpty || super.widget.isCorrect(numStr)) {
        errorMsg = "";
      } else {
        errorMsg = "wrong mac address";
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
            keyboardType: TextInputType.text,
            onSubmitted: (text) {
              numberValidate(text);
            },
            inputFormatters: <TextInputFormatter>[
              WhitelistingTextInputFormatter(RegExp(r'[0-9a-fA-F]|:|-'))
            ], // Only numbers can be entered
          ),
          Padding(
            padding: EdgeInsets.all(5.0),
            child: Text(
              "$errorMsg",
              style: TextStyle(
                  color: Theme.of(context).errorColor,
                  fontWeight: FontWeight.w500),
            ),
          ),
        ],
      ),
    );
  }


}
