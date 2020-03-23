import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class TopicNameInput extends StatefulWidget {
  final String title;
  final TextEditingController controller = TextEditingController();

  TopicNameInput({Key key, this.title}) : super(key: key);

  _TopicNameInput createState() => _TopicNameInput(title, controller);

  bool isCorrect({String numStr}) {
    if (numStr == null) {
      numStr = controller.text;
    }
    RegExp regExp = new RegExp(
      r'^([a-z|A-Z]|/|~)([0-9|a-z|A-Z]|/|_)+$',
      caseSensitive: false,
      multiLine: false,
    );
    return regExp.hasMatch(numStr) && numStr.length <= 255;
  }

  static List<int>  stringToBytes(String string){
    List<int> ret = [0, 0];
    int num = string.length;
    if ( (num > 0) && (num ~/ 256 < 256)){
      List<int> list = string.codeUnits;
      List<int> len = [ num % 256, num ~/256];
      ret = len + list;
    }
    return ret;
  }

  static String bytesToString(Uint8List data) {
    return String.fromCharCodes(data);
  }
}

class _TopicNameInput extends State<TopicNameInput> {
  _TopicNameInput(this.title, this.controller);

  final String title;
  final controller;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    setState(() {
      if (numStr.isEmpty || super.widget.isCorrect(numStr: numStr)) {
        errorMsg = "";
      } else {
        errorMsg = "wrong topic name";
      }
    });
  }

  Widget build(BuildContext context) {
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
              WhitelistingTextInputFormatter(RegExp(r'[0-9|a-z|A-Z]|/|~|_'))
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
