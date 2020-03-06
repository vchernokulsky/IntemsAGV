import 'package:flutter/material.dart';
import 'package:stm_setup/RobotConnect.dart';

import 'RobotSetUp.dart';



class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Center(
        child: Container(
          child: RobotConnect(),
        ),
      ),
    );
  }
}

Future<void> main() async {
  runApp(MyApp());
}
