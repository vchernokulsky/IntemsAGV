import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class MacInput extends StatefulWidget {
  final String title;

  const MacInput({Key key, this.title})
      : super(key: key);

  _MacInput createState() => _MacInput(title);
}

class _MacInput extends State<MacInput> {
  _MacInput(this.title);

  final String title;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    RegExp regExp = new RegExp(
      r'^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$',
      caseSensitive: false,
      multiLine: false,
    );
    setState(() {
      if (numStr.isEmpty || regExp.hasMatch(numStr)) {
        errorMsg = "";
      } else {
        errorMsg = "wrong mac address";
      }
    });
  }

  Widget build(BuildContext context) {
    return Center(
      child: Column(
        children: <Widget>[
          TextField(
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
