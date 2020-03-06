import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';

void showGoodToast(String text) {
  Fluttertoast.showToast(
      msg: "$text",
      toastLength: Toast.LENGTH_SHORT,
      gravity: ToastGravity.BOTTOM,
      timeInSecForIos: 1,
      backgroundColor: Colors.blueGrey,
      textColor: Colors.white,
      fontSize: 16.0);
}

void showBadToast(String text) {
  Fluttertoast.showToast(
      msg: "$text",
      toastLength: Toast.LENGTH_SHORT,
      gravity: ToastGravity.BOTTOM,
      timeInSecForIos: 1,
      backgroundColor: Colors.redAccent[700],
      textColor: Colors.white,
      fontSize: 16.0);
}