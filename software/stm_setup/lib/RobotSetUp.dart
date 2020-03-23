import 'package:flutter/material.dart';
import 'package:stm_setup/RobotGeometry.dart';
import 'package:stm_setup/RosTopics.dart';
import 'package:stm_setup/Wiznet.dart';

import 'SocketData.dart';

class RobotSetUp extends StatefulWidget {
  _RobotSetUpState createState() => _RobotSetUpState();
}

class _RobotSetUpState extends State<RobotSetUp> {

  static const int ROBOT_GEOM = 0;
  static const int ROS_TOPICS = 1;
  static const int WIZNET = 2;

  final model = SocketData();
  int curItem;


  void initState() {
    super.initState();
    curItem = ROBOT_GEOM;
    model.addListener(updateState);
    model.getInfo();
  }

  void updateState() {
    setState(() {});
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
                visible: curItem == ROS_TOPICS,
                child: RosTopic(),
              ),
              Visibility(
                visible: curItem == WIZNET,
                child: Wiznet(),
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
              title: Text('Ros'),
              onTap: () => setWidgets(ROS_TOPICS),
            ),
            ListTile(
              title: Text('Wiznet'),
              onTap: () => setWidgets(WIZNET),
            ),
          ],
        ),
      ),
    );
  }
}
