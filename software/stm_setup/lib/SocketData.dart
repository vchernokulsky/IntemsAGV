import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/widgets.dart';
import "package:hex/hex.dart";
import 'package:stm_setup/Inputs/MacInput.dart';

class SocketData extends ChangeNotifier{
  static bool getData = false;
  static String macAddress = "";


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

  void sendInfo() async {

      final Socket client = await Socket.connect('192.168.55.10', 11511);
      client.add(Uint8List.fromList([255, 254]) + MacInput.stringToBytes(macAddress));
      client.listen(
              (Uint8List data) {
              data.forEach((i) => print("got $i")) ;
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

  }
  void parseIntoVariables(Uint8List data){
    String string = String.fromCharCodes(data.sublist(0,3));
    if (string == 'set') {
      macAddress = MacInput.bytesToString(data.sublist(4,10));
    }
  }

}