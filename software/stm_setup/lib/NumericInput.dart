import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class NumericInput extends StatefulWidget {
  final String title;
  final int minValue;
  final int maxValue;

  const NumericInput({Key key, this.title, this.minValue, this.maxValue}) : super(key: key);
  _NumericInput createState() => _NumericInput( title, minValue, maxValue);
}

class _NumericInput extends State<NumericInput> {

   _NumericInput(this.title, this.minValue, this.maxValue);

  final String title;

  final int minValue;
  final int maxValue;

  String errorMsg;




  void initState() {
    super.initState();
    errorMsg = "";
  }


  void numberValidate(String numStr) {
    int num = int.parse(numStr);
    setState(() {
      if(num >= minValue && num <= maxValue){
        errorMsg = "";
      } else {
        errorMsg = "out of range(should be between $minValue and $maxValue)";
      }
    });
  }


  Widget build(BuildContext context) {
    return
      Center(
        child:  Column(
          children: <Widget>[
            TextField(
              decoration: InputDecoration(labelText: "$title"),
              keyboardType: TextInputType.number,
              onSubmitted: (text){
                numberValidate(text);
              },
              inputFormatters: <TextInputFormatter>[
                WhitelistingTextInputFormatter.digitsOnly
              ], // Only numbers can be entered
            ),
            Text("$errorMsg", style: TextStyle(color: Colors.red.withOpacity(1.0),
                fontWeight: FontWeight.bold),),
          ],
        ),
      );

  }
}