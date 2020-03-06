import 'package:flutter/material.dart';
import 'package:stm_setup/RobotDrive.dart';
import 'package:stm_setup/RobotGeometry.dart';
import 'package:stm_setup/RosTopics.dart';
import 'package:stm_setup/Wiznet.dart';

import 'Delay.dart';
import 'SocketData.dart';

class RobotSetUp extends StatefulWidget {
  _RobotSetUpState createState() => _RobotSetUpState();
}

class _RobotSetUpState extends State<RobotSetUp> {

  final GlobalKey<ScaffoldState> _scaffoldKey = new GlobalKey<ScaffoldState>();
  static const int ROBOT_GEOM = 0;
  static const int ROBOT_DRIVE = 1;
  static const int ROS_TOPICS = 2;
  static const int WIZNET = 3;
  static const int DELAY = 4;

  static const String RADIUS_ERROR_MSG = "Wrong radius value";

  final model = SocketData();

  int curItem;
  String radiusErrorMsg;
  double val;

  void initState() {
    super.initState();
    val = 0;
    curItem = ROBOT_GEOM;
    radiusErrorMsg = "";

    model.addListener(updateState);
    model.getInfo();
  }

  void updateState() {
    setState(() {});
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
          actions: <Widget>[
            IconButton(
              icon: new Icon(Icons.arrow_back),
              onPressed: () => Navigator.of(context).pop(),
            ),
            IconButton(
              icon: const Icon(Icons.cloud_download),
              tooltip: 'get settings from STM',
              onPressed: () => model.getInfo(force: true),
            ),
            IconButton(
              icon: const Icon(Icons.cloud_upload),
              tooltip: 'Send settings to STM',
              onPressed: () => model.sendInfo(),
            )
          ]),
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
              Visibility(
                visible: curItem == WIZNET,
                child: Wiznet(),
              ),
              Visibility(
                visible: curItem == DELAY,
                child: Delay(),
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
              title: Text('Ros'),
              onTap: () => setWidgets(ROS_TOPICS),
            ),
            ListTile(
              title: Text('Wiznet'),
              onTap: () => setWidgets(WIZNET),
            ),
            ListTile(
              title: Text('Delay'),
              onTap: () => setWidgets(DELAY),
            ),
          ],
        ),
      ),
    );
  }
}
