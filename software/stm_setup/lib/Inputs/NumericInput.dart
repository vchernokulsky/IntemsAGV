import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class NumericInput extends StatefulWidget {
  final String title;
  final int minValue;
  final int maxValue;
  final TextEditingController controller = TextEditingController();

  NumericInput({Key key, this.title, this.minValue, this.maxValue})
      : super(key: key);

  _NumericInput createState() => _NumericInput(title, minValue, maxValue, controller);

  bool isCorrect({String numStr}) {
    if(numStr == null){
      numStr = controller.text;
    }
    if(numStr.isEmpty){
      return true;
    }
    int num = int.parse(numStr);
      return (num >= minValue && num <= maxValue);
  }

  static List<int> stringToBytes(String string){
    List<int> ret =[0, 0];
    int num = int.parse(string);
    if (num ~/ 256 < 256){
      ret = [ num % 256, num ~/256];
    }
    return ret;
  }
  static Uint8List intToBytes(int num){
    Uint8List ret = Uint8List.fromList([0, 0]);
    if (num ~/ 256 < 256){
      ret = Uint8List.fromList([ num % 256, num ~/256]);
    }
    return ret;
  }
  static String bytesToString(Uint8List data){
    String ret = "";
    if(data.length == 2) {
      ret = '${data[1] * 256 + data[0]}';
    }
    return ret;
  }

  static int bytesToInt(Uint8List data){
    int ret = 0;
    if(data.length == 2) {
      ret = data[1] * 256 + data[0];
    }
    return ret;
  }
}

class _NumericInput extends State<NumericInput> {
  _NumericInput(this.title, this.minValue, this.maxValue, this.controller);
  final controller;
  final String title;

  final int minValue;
  final int maxValue;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void setErrorText(String numStr){
    if (super.widget.isCorrect(numStr: numStr)) {
      errorMsg = "";
    } else {
      errorMsg = "out of range(should be between $minValue and $maxValue)";
    }
  }

  void numberValidate(String numStr) {
    setState(() {
      setErrorText(numStr);
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
            keyboardType: TextInputType.number,
            onSubmitted: (text) {
              numberValidate(text);
            },
            inputFormatters: <TextInputFormatter>[
              WhitelistingTextInputFormatter.digitsOnly
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
