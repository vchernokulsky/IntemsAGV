import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:stm_setup/RobotConnect.dart';
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

  void reconnect() {
    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => RobotConnect()),
    );
  }

  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Robot Set Up"), actions: <Widget>[
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
        child: Stack(
          children: <Widget>[
            ConstrainedBox(
              constraints: const BoxConstraints(
                  minWidth: double.infinity, minHeight: double.infinity),
              child: Opacity(
                opacity: 0.11,
                child: Image.asset(
                  'images/back3.png',
                  fit: BoxFit.none,
                  repeat: ImageRepeat.repeat,
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.fromLTRB(16.0, 16.0, 16.0, 16.0),
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
          ],
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
              decoration: BoxDecoration(
                  color: Theme.of(context).primaryColorDark,
                  image: DecorationImage(
                      alignment: Alignment.center,
                      image: AssetImage("images/LogoWhite.png"),
                      fit: BoxFit.none)),
              child: Container(
                alignment: Alignment.bottomCenter,
                child: Text(
                  "DiffDrive Set Up",
                  style: TextStyle(color: Colors.white, fontSize: 25),
                ),
              ),
            ),
            ListTile(
              leading: CircleAvatar(
                backgroundColor: Colors.transparent,
//                backgroundImage: AssetImage("images/LogoWhite.png"),
                child: Container(
                    width: 100.0,
                    height: 100.0,
                    decoration: BoxDecoration(
                        image: DecorationImage(
                            alignment: Alignment.center,
                            image: AssetImage("images/robot.png"),
                            fit: BoxFit.fill))),
              ),
              title: Text(
                'Robot geometry',
                style: TextStyle(fontSize: 15),
              ),
              onTap: () => setWidgets(ROBOT_GEOM),
            ),
            ListTile(
              leading: CircleAvatar(
                backgroundColor: Colors.transparent,
//                backgroundImage: AssetImage("images/LogoWhite.png"),
                child: Container(
                    width: 100.0,
                    height: 100.0,
                    decoration: BoxDecoration(
                        image: DecorationImage(
                            alignment: Alignment.center,
                            image: AssetImage("images/ros_logo.png"),
                            fit: BoxFit.fitWidth))),
              ),
              title: Text(
                'Topics and frames',
                style: TextStyle(fontSize: 15),
              ),
              onTap: () => setWidgets(ROS_TOPICS),
            ),
            ListTile(
              leading: CircleAvatar(
                backgroundColor: Colors.transparent,
//                backgroundImage: AssetImage("images/LogoWhite.png"),
                child: Container(
                    width: 100.0,
                    height: 100.0,
                    decoration: BoxDecoration(
                        image: DecorationImage(
                            alignment: Alignment.center,
                            image: AssetImage("images/network.png"),
                            fit: BoxFit.fitWidth))),
              ),
              title: Text(
                'Network configuration',
                style: TextStyle(fontSize: 15),
              ),
              onTap: () => setWidgets(WIZNET),
            ),
            ListTile(
                leading: RaisedButton(
                  child: Text(
                    "Reconnect to robot",
                    style: TextStyle(color: Colors.white),
                  ),
                  color: Color(0xff3f3f3f),
                  onPressed: () => reconnect(),
                  padding: EdgeInsets.symmetric(vertical: 15, horizontal: 70),
                ),
                onTap: () => null),
          ],
        ),
      ),
    );
  }
}
