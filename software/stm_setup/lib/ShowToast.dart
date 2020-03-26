import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';

void showGoodToast(String text) {
  Fluttertoast.showToast(
      msg: "$text",
      toastLength: Toast.LENGTH_SHORT,
      gravity: ToastGravity.BOTTOM,
      timeInSecForIos: 1,
      backgroundColor: Color(0xff2ada9a),
      textColor: Colors.white,
      fontSize: 16.0);
}

void showBadToast(String text) {
  Fluttertoast.showToast(
      msg: "$text",
      toastLength: Toast.LENGTH_SHORT,
      gravity: ToastGravity.BOTTOM,
      timeInSecForIos: 1,
      backgroundColor: Color(0xffff4545),
      textColor: Colors.white,
      fontSize: 16.0);
}