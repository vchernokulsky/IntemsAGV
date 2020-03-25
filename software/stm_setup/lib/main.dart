import 'package:flutter/material.dart';
import 'package:stm_setup/RobotConnect.dart';


class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
          primaryColor: Color(0xff5f86c4),
          accentColor: Color(0xff3f3f3f),
          accentColorBrightness: Brightness.dark,
          scaffoldBackgroundColor: Color(0xffffffff),

          canvasColor: Theme.of(context).primaryColorLight,
          cursorColor: Color(0xff5f86c4),
          buttonTheme: ButtonThemeData(
              buttonColor: Color(0xff3f3f3f),
              shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(4.0)),
              textTheme: ButtonTextTheme.primary,
          ),
          errorColor: Color(0xffff4545)
      ),
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
