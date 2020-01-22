import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class IpInput extends StatefulWidget {
  final String title;

  const IpInput({Key key, this.title})
      : super(key: key);

  _IpInput createState() => _IpInput(title);
}

class _IpInput extends State<IpInput> {
  _IpInput(this.title);

  final String title;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    RegExp regExp = new RegExp(
      r'^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$',
      caseSensitive: false,
      multiLine: false,
    );
    setState(() {
      if (numStr.isEmpty || regExp.hasMatch(numStr)) {
        errorMsg = "";
      } else {
        errorMsg = "wrong ip";
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
