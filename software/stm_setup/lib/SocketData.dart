import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:fluttertoast/fluttertoast.dart';
import "package:hex/hex.dart";
import 'package:stm_setup/Inputs/MacInput.dart';

import 'Inputs/IpInput.dart';

class SocketData extends ChangeNotifier {
  static String connectHost = "192.168.2.114";
  static int connectPort = 11511;

  static bool getData = false;
  static String localIpAddress = "";

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

  void getInfo({bool force = false}) async {
    if (force || !getData) {
      try {
        final Socket client = await Socket.connect(connectHost, connectPort);
        client.add(Uint8List.fromList([255, 255]));
        client.listen((Uint8List data) {
          parseIntoVariables(data);
          getData = true;
          notifyListeners();
        }, onDone: () {
          print('Done');
          client.close();
        }, onError: (e) {
          print('Got error $e');
          client.close();
        });
        print('main done');
      } on SocketException catch (e) {
        print(e.toString());
        showBadToast("cannot connect to robot");
      }
    } else {
      notifyListeners();
    }
  }

  void sendInfo() async {
    final Socket client = await Socket.connect(connectHost, connectPort);
    client.add(Uint8List.fromList([255, 254, 0, 0]) +
        IpInput.stringToBytes(localIpAddress));
    client.listen((Uint8List data) {
      data.forEach((i) => print("got $i"));
      if (data[0] == 7 && data[1] == 7 && data[2] == 7) {
        showGoodToast("Settings saved to robot");
      } else {
        if (data[0] == 6 && data[1] == 6 && data[2] == 6) {
          showBadToast("Saving failed");
        } else {
          showBadToast("got wrong answer");
        }
      }
    }, onDone: () {
      print('Done');
      client.close();
    }, onError: (e) {
      print('Got error $e');
      client.close();
    });
    print('main done');
  }

  void parseIntoVariables(Uint8List data) {
    data.forEach((i) => print("got $i"));
    String string = String.fromCharCodes(data.sublist(0, 3));
    if (string == 'set') {
      localIpAddress = IpInput.bytesToString(data.sublist(4, 8));
    }
  }
}
