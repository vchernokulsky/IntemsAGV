import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class DecimalInput extends StatefulWidget {
  final String title;
  final double minValue;
  final double maxValue;
  final TextEditingController controller = TextEditingController();

  DecimalInput({Key key, this.title, this.minValue, this.maxValue})
      : super(key: key);

  _DecimalInput createState() => _DecimalInput(title, minValue, maxValue, controller);

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
}

class _DecimalInput extends State<DecimalInput> {
  _DecimalInput(this.title, this.minValue, this.maxValue, this.controller);

  final String title;

  final double minValue;
  final double maxValue;

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
    numberValidate(controller.text);
    return Center(
      child: Column(
        children: <Widget>[
          TextField(
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
