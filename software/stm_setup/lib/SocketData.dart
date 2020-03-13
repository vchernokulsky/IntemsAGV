import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:fluttertoast/fluttertoast.dart';
import "package:hex/hex.dart";
import 'package:stm_setup/Inputs/MacInput.dart';
import 'package:stm_setup/Inputs/NumericInput.dart';

import 'Inputs/IpInput.dart';
import 'ShowToast.dart';

class SocketData extends ChangeNotifier {
  final int msgSize = 26;
  final int setFlagSize = 3;
  final int ipSize = 4;
  final int numSize = 2;

  final int setFlagOffset = 0;
  final int localIpOffset = 4;
  final int networkMaskOffset = 8;
  final int gateAwayOffset = 12;
  final int rosClientPortOffset = 16;
  final int setupServerPortOffset = 18;
  final int serialNodeIpOffset = 20;
  final int serialNodePortOffset = 24;

  static String connectHost = "192.168.2.114";
  static int connectPort = 11511;

  static bool getData = false;

  static String localIpAddress = "";
  static String networkMask = "";
  static String gateAway = "";
  static String rosClientPort = "";
  static String setupServerPort = "";
  static String serialNodeIp = "";
  static String serialNodePort = "";

  static String wheelRadius = "";
  static String wheelSeparation = "";
  static String maxLinVelocity = "";
  static String maxAngVelocity = "";
  static String maxPwdAllowed = "";
  static String radPerTick = "";

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
        IpInput.stringToBytes(localIpAddress) +
        IpInput.stringToBytes(networkMask) +
        IpInput.stringToBytes(gateAway) +
        NumericInput.stringToBytes(rosClientPort) +
        NumericInput.stringToBytes(setupServerPort) +
        IpInput.stringToBytes(serialNodeIp) +
        NumericInput.stringToBytes(serialNodePort));
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
    if (data.length != msgSize) {
      return;
    }
    String string = String.fromCharCodes(
        data.sublist(setFlagOffset, setFlagOffset + setFlagSize));
    if (string == 'set') {
      localIpAddress = IpInput.bytesToString(
          data.sublist(localIpOffset, localIpOffset + ipSize));
      networkMask = IpInput.bytesToString(
          data.sublist(networkMaskOffset, networkMaskOffset + ipSize));
      gateAway = IpInput.bytesToString(
          data.sublist(gateAwayOffset, gateAwayOffset + ipSize));
      rosClientPort = NumericInput.bytesToString(
          data.sublist(rosClientPortOffset, rosClientPortOffset + numSize));
      setupServerPort = NumericInput.bytesToString(
          data.sublist(setupServerPortOffset, setupServerPortOffset + numSize));
      serialNodeIp = IpInput.bytesToString(
          data.sublist(serialNodeIpOffset, serialNodeIpOffset + ipSize));
      serialNodePort = NumericInput.bytesToString(
          data.sublist(serialNodePortOffset, serialNodePortOffset + numSize));
    }
  }
}
