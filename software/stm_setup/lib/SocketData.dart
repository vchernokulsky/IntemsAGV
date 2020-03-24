import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:fluttertoast/fluttertoast.dart';
import "package:hex/hex.dart";
import 'package:stm_setup/Inputs/DecimalInput.dart';
import 'package:stm_setup/Inputs/MacInput.dart';
import 'package:stm_setup/Inputs/NumericInput.dart';
import 'package:stm_setup/Inputs/TopicNameInput.dart';

import 'Inputs/IpInput.dart';
import 'ShowToast.dart';

class SocketData extends ChangeNotifier {
  final int headerSize = 6;
  final int chkSumSize = 1;
  final int minMsgSize = 50;
  final int maxMsgSize = 1070;

  final int setFlagSize = 3;
  final int ipSize = 4;
  final int numSize = 2;
  final Duration timeout = Duration(seconds: 1);

  final int setFlagOffset = 0;
  final int msgSizeOffset = 4;

  final int localIpOffset = 6;
  final int networkMaskOffset = 10;
  final int gateAwayOffset = 14;
  final int rosClientPortOffset = 18;
  final int setupServerPortOffset = 20;
  final int serialNodeIpOffset = 22;
  final int serialNodePortOffset = 26;

  final int wheelRadiusOffset = 28;
  final int wheelSeparationOffset = 30;
  final int maxLinVelocityOffset = 32;
  final int maxAngVelocityOffset = 34;
  final int radPerTickOffset = 36;
  final int maxPwdAllowedOffset = 38;

  final int topicsOffset = 40;

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

  static String cmdVelTopic = "";
  static String odomTopic = "";
  static String baseFrame = "";
  static String odomFrame = "";

  void getInfo({bool force = false}) async {
    if (force || !getData) {
      try {
        final Socket client =
        await Socket.connect(connectHost, connectPort, timeout: timeout);
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
    final Socket client =
    await Socket.connect(connectHost, connectPort, timeout: timeout);
    client.add(createMsg());
    client.listen((Uint8List data) {
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

  Uint8List createMsg() {
    List<int> msgBody = IpInput.stringToBytes(localIpAddress) +
        IpInput.stringToBytes(networkMask) +
        IpInput.stringToBytes(gateAway) +
        NumericInput.stringToBytes(rosClientPort) +
        NumericInput.stringToBytes(setupServerPort) +
        IpInput.stringToBytes(serialNodeIp) +
        NumericInput.stringToBytes(serialNodePort) +
        NumericInput.stringToBytes(wheelRadius) +
        NumericInput.stringToBytes(wheelSeparation) +
        DecimalInput.stringToBytes(maxLinVelocity, 3) +
        DecimalInput.stringToBytes(maxAngVelocity, 3) +
        DecimalInput.stringToBytes(radPerTick, 5) +
        NumericInput.stringToBytes(maxPwdAllowed) +
        TopicNameInput.stringToBytes(cmdVelTopic) +
        TopicNameInput.stringToBytes(odomTopic) +
        TopicNameInput.stringToBytes(baseFrame) +
        TopicNameInput.stringToBytes(odomFrame);
    List<int> msgSize = NumericInput.intToBytes(
        headerSize + msgBody.length + chkSumSize);
    List<int> byteList = [255, 254, 0, 0] + msgSize + msgBody;

    int chkSum = 0;
    for (int i in byteList) {
      chkSum += i;
      chkSum %= 256;
    }
    chkSum = 255 - chkSum;

    Uint8List ret = Uint8List.fromList(byteList + [chkSum]);
    return ret;
  }

  void parseIntoVariables(Uint8List data) {

    if (data.length < minMsgSize || data.length > maxMsgSize) {
      return;
    }
    String string = String.fromCharCodes(
        data.sublist(setFlagOffset, setFlagOffset + setFlagSize));
    if (string != 'set') {
      return;
    }
    int msgSize = NumericInput.bytesToInt(data.sublist(msgSizeOffset, msgSizeOffset + numSize));
    if(msgSize != data.length){
      return;
    }
    int chkSum = 0;
    for (int i in data) {
      chkSum += i;
      chkSum %= 256;
    }
    if(chkSum != 255){
      return;
    }

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

    wheelRadius = NumericInput.bytesToString(
        data.sublist(wheelRadiusOffset, wheelRadiusOffset + numSize));
    wheelSeparation = NumericInput.bytesToString(
        data.sublist(wheelSeparationOffset, wheelSeparationOffset + numSize));
    maxLinVelocity = DecimalInput.bytesToString(
        data.sublist(maxLinVelocityOffset, maxLinVelocityOffset + numSize),
        3);
    maxAngVelocity = DecimalInput.bytesToString(
        data.sublist(maxAngVelocityOffset, maxAngVelocityOffset + numSize),
        3);
    radPerTick = DecimalInput.bytesToString(
        data.sublist(radPerTickOffset, radPerTickOffset + numSize), 5);
    maxPwdAllowed = NumericInput.bytesToString(
        data.sublist(maxPwdAllowedOffset, maxPwdAllowedOffset + numSize));

    //*********** cmd_vel topic ************************************
    int strSize = NumericInput.bytesToInt(
        data.sublist(topicsOffset, topicsOffset + numSize));
    if (strSize < 1) {
      return;
    }
    int curOffset = topicsOffset + numSize;
    cmdVelTopic = TopicNameInput.bytesToString(
        data.sublist(curOffset, curOffset + strSize));
    curOffset += strSize;

    //*********** odometry topic ************************************
    strSize =
        NumericInput.bytesToInt(data.sublist(curOffset, curOffset + numSize));
    if (strSize < 1) {
      return;
    }
    curOffset += numSize;
    odomTopic = TopicNameInput.bytesToString(
        data.sublist(curOffset, curOffset + strSize));
    curOffset += strSize;

    //*********** base frame ************************************
    strSize =
        NumericInput.bytesToInt(data.sublist(curOffset, curOffset + numSize));
    if (strSize < 1) {
      return;
    }
    curOffset += numSize;
    baseFrame = TopicNameInput.bytesToString(
        data.sublist(curOffset, curOffset + strSize));
    curOffset += strSize;

    //*********** odometry frame ************************************
    strSize =
        NumericInput.bytesToInt(data.sublist(curOffset, curOffset + numSize));
    if (strSize < 1) {
      return;
    }
    curOffset += numSize;
    odomFrame = TopicNameInput.bytesToString(
        data.sublist(curOffset, curOffset + strSize));
    curOffset += strSize;
  }
}
