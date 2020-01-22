import 'package:flutter/material.dart';
import 'package:stm_setup/RobotDrive.dart';
import 'package:stm_setup/RobotGeometry.dart';
import 'package:stm_setup/RosTopics.dart';

class Counter extends StatefulWidget {
  _CounterState createState() => _CounterState();
}

class _CounterState extends State<Counter> {
  static const int ROBOT_GEOM = 0;
  static const int ROBOT_DRIVE = 1;
  static const int ROS_TOPICS = 2;

  static const String RADIUS_ERROR_MSG = "Wrong radius value";

  int curItem;
  String radiusErrorMsg;
  double val;

  void initState() {
    super.initState();
    val = 0;
    curItem = ROBOT_GEOM;
    radiusErrorMsg = "";
  }

  void change() {
    setState(() {
      val += 1;
    });
  }

  void numberValidate(String numStr) {
    int maxValue = 100;
    int minValue = 1;
    int num = int.parse(numStr);
    setState(() {
      if (num >= minValue && num <= maxValue) {
        radiusErrorMsg = "";
      } else {
        radiusErrorMsg = RADIUS_ERROR_MSG;
      }
    });
  }

  void setWidgets(int widget) {
    setState(() {
      curItem = widget;
    });
    Navigator.pop(context);
  }

  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("DiffDriveSTM"),
      ),
      body: Scrollbar(
        child: Padding(
          padding: EdgeInsets.fromLTRB(16.0, 16.0, 16.0, 0.0),
          child: ListView(
            shrinkWrap: true,
            children: <Widget>[
              Visibility(
                visible: curItem == ROBOT_GEOM,
                child: RobotGeometry(),
              ),
              Visibility(
                visible: curItem == ROBOT_DRIVE,
                child: RobotDrive(),
              ),
              Visibility(
                visible: curItem == ROS_TOPICS,
                child: RosTopic(),
              ),
            ],
          ),
        ),
      ),
      drawer: Drawer(
        // Add a ListView to the drawer. This ensures the user can scroll
        // through the options in the drawer if there isn't enough vertical
        // space to fit everything.
        child: ListView(
          // Important: Remove any padding from the ListView.
          padding: EdgeInsets.zero,
          children: <Widget>[
            DrawerHeader(
              child: Text("DiffDriveSTM"),
              decoration: BoxDecoration(
                color: Colors.blue,
              ),
            ),
            ListTile(
              title: Text('Robot geometry'),
              onTap: () => setWidgets(ROBOT_GEOM),
            ),
            ListTile(
              title: Text('Robot drive'),
              onTap: () => setWidgets(ROBOT_DRIVE),
            ),
            ListTile(
              title: Text('Ros topics'),
              onTap: () => setWidgets(ROS_TOPICS),
            ),
          ],
        ),
      ),
    );
  }
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Center(
        child: Container(
          child: Counter(),
        ),
      ),
    );
  }
}

Future<void> main() async {
  runApp(MyApp());
}
