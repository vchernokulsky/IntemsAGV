#!/usr/bin/env python

from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import SocketServer
import json
import cgi
import rospy
from sys import argv
from std_msgs.msg import Float32

SERVER_NODE = None

def constrain(value, low, high):
    if value < low:
        value = low
    elif value > high:
        value = high
    return value

class ServerNode:
    def __init__(self):
        rospy.init_node('cube_robot_teleop_server')
        self.name = rospy.get_name()
        rospy.loginfo("{0} started".format(self.name))
        self.maxLinVel = float(rospy.get_param('~max_lin_vel', 3.82))
        self.maxAngVel = float(rospy.get_param('~max_ang_vel', 3.82))

        self.linearVelocity = 0.0
        self.angularVelocity = 0.0

        self.linPub = rospy.Publisher("lin_vel", Float32, queue_size=1)
        self.angPub = rospy.Publisher("ang_vel", Float32, queue_size=1)

    def publish_angular(self):
        self.angPub.publish(self.angularVelocity)

    def publish_linear(self):
        self.linPub.publish(self.linearVelocity)

    def set_linear_velocity(self, vel):
        self.linearVelocity = constrain(vel, -self.maxLinVel, self.maxLinVel)

    def set_angular_velocity(self, vel):
        self.angularVelocity = constrain(vel, -self.maxAngVel, self.maxAngVel)

    def show_info(self):
            rospy.loginfo(self.twist_msg())

    def twist_msg(self):
        return "Vel:\tlinear vel %s\t angular vel %s " % (self.linearVelocity, self.angularVelocity)


class Server(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()

    def do_HEAD(self):
        self._set_headers()

    # GET sends back a Hello world message
    def do_GET(self):
        self._set_headers()
        self.wfile.write(json.dumps({'hello': 'world', 'received': 'ok'}))

    # POST echoes the message adding a JSON field
    def do_POST(self):
        ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))

        # refuse to receive non-json content
        if ctype != 'application/json':
            self.send_response(400)
            self.end_headers()
            return

        # read the message and convert it into a python dictionary
        length = int(self.headers.getheader('content-length'))
        message = json.loads(self.rfile.read(length))
        rospy.loginfo(message)
        SERVER_NODE.set_linear_velocity(message['lin'])
        SERVER_NODE.set_angular_velocity(message['ang'])
        SERVER_NODE.show_info()
        SERVER_NODE.publish_angular()
        SERVER_NODE.publish_linear()
        message['received'] = 'ok'

        # send the message back
        self._set_headers()
        self.wfile.write(json.dumps(message))


def run(server_class=HTTPServer, handler_class=Server, port=8008):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print('Starting httpd on port %d...' % port)
    httpd.serve_forever()


if __name__ == "__main__":
    SERVER_NODE = ServerNode()
    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
