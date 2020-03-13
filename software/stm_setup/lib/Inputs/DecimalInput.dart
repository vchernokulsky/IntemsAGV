import 'dart:math';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class DecimalInput extends StatefulWidget {
  final String title;
  final double minValue;
  final double maxValue;
  final int decimal;
  final TextEditingController controller = TextEditingController();

  DecimalInput({Key key, this.title, this.minValue, this.maxValue, this.decimal = 3})
      : super(key: key);

  _DecimalInput createState() => _DecimalInput(title, minValue, maxValue, controller, decimal);

  bool isCorrect({String numStr}) {
    if (numStr == null) {
      numStr = controller.text;
    }
    RegExp regExp = new RegExp(
      r'^(\d*\.)?\d+$',
      caseSensitive: false,
      multiLine: false,
    );
    if (regExp.hasMatch(numStr)) {
      double num = double.parse(numStr);
      if (num >= minValue && num <= maxValue) {
       return true;
      }
    }
    return false;
  }

  static String bytesToString(Uint8List data, int pwr){
    String ret = "";
    if(data.length == 2) {
      double num = (data[1] * 256 + data[0]) / pow(10, pwr);
      ret = '$num';
    }
    return ret;
  }

  static Uint8List stringToBytes(String string, int pwr){
    Uint8List ret = Uint8List.fromList([0, 0]);
    int num = (double.parse(string) * pow(10, pwr)).round();
    if (num ~/ 256 < 256){
      ret = Uint8List.fromList([ num % 256, num ~/256]);
    }
    return ret;
  }
}

class _DecimalInput extends State<DecimalInput> {
  _DecimalInput(this.title, this.minValue, this.maxValue, this.controller, this.decimal);

  final String title;

  final double minValue;
  final double maxValue;
  final int decimal;

  String errorMsg;
  final controller;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    RegExp regExp = new RegExp(
      r'^(\d*\.)?\d+$',
      caseSensitive: false,
      multiLine: false,
    );
    setState(() {
      if(numStr.isEmpty){
        errorMsg = "";
      } else {
        if (regExp.hasMatch(numStr)) {
          double num = double.parse(numStr);

          if (num >= minValue && num <= maxValue) {
            errorMsg = "";
            controller.text = num.toStringAsFixed(decimal);
          } else {
            errorMsg =
            "out of range(should be between $minValue and $maxValue)";
          }
        } else {
          errorMsg = "wrong format(should be decimal)";
        }
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
            keyboardType: TextInputType.number,
            onSubmitted: (text) {
              numberValidate(text);
            },
            inputFormatters: <TextInputFormatter>[
              WhitelistingTextInputFormatter(RegExp(r'\d+|\.|,'))
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
