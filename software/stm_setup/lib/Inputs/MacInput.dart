import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class MacInput extends StatefulWidget {
  final String title;
  final String value;
  final controller;

  const MacInput({Key key, this.title, this.value, this.controller})
      : super(key: key);

  _MacInput createState() => _MacInput(title, value, controller);

  static bool isCorrect(String numStr){
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
      if (numStr.isEmpty || MacInput.isCorrect(numStr)) {
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
                  color: Colors.red.withOpacity(1.0),
                  fontWeight: FontWeight.bold),
            ),
          ),
        ],
      ),
    );
  }


}
