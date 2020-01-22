import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

class TopicNameInput extends StatefulWidget {
  final String title;

  const TopicNameInput({Key key, this.title})
      : super(key: key);

  _TopicNameInput createState() => _TopicNameInput(title);
}

class _TopicNameInput extends State<TopicNameInput> {
  _TopicNameInput(this.title);

  final String title;

  String errorMsg;

  void initState() {
    super.initState();
    errorMsg = "";
  }

  void numberValidate(String numStr) {
    RegExp regExp = new RegExp(
      r'^([a-z|A-Z]|/|~)([0-9|a-z|A-Z]|/|_)+$',
      caseSensitive: false,
      multiLine: false,
    );
    setState(() {
      if (regExp.hasMatch(numStr)) {
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
