import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/widgets.dart';
import "package:hex/hex.dart";

class SocketData extends ChangeNotifier{
  static bool getData = false;
  static String _macAddress = "";

  String get macAddress => SocketData._macAddress;
  set macAddress(String string) => _macAddress = string;



  void getInfo({bool force = false}) async {
    if(force || !getData) {
      final Socket client = await Socket.connect('192.168.55.10', 11511);
      client.add(Uint8List.fromList([255, 255]));
      client.listen(
              (Uint8List data) {
            parseIntoVariables(data);
            getData = true;
            notifyListeners();
          },
          onDone: () {
            print('Done');
            client.close();
          },
          onError: (e) {
            print('Got error $e');
            client.close();
          });
      print('main done');
    } else {
      notifyListeners();
    }
    }
  void parseIntoVariables(Uint8List data){
    String string = String.fromCharCodes(data.sublist(0,3));
    if (string == 'set') {
      String mac = "";
      data.sublist(4,10).forEach((i)=> mac+='${HEX.encode([i])}:');
      mac = mac.substring(0,mac.length-1);
      _macAddress = mac;
    }
  }

}