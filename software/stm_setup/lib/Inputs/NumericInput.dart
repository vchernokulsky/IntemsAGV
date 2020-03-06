import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class NumericInput extends StatefulWidget {
  final String title;
  final int minValue;
  final int maxValue;
  final controller;

  const NumericInput({Key key, this.title, this.minValue, this.maxValue, this.controller})
      : super(key: key);

  _NumericInput createState() => _NumericInput(title, minValue, maxValue, controller);

  static bool isCorrect(String numStr, int minValue, int maxValue) {
    int num = int.parse(numStr);
      return (num >= minValue && num <= maxValue);
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

  void numberValidate(String numStr) {
    setState(() {
      if (NumericInput.isCorrect(numStr, minValue, maxValue)) {
        errorMsg = "";
      } else {
        errorMsg = "out of range(should be between $minValue and $maxValue)";
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
              WhitelistingTextInputFormatter.digitsOnly
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
