// GLOBAL VARIABLES

var GREEN_LED = A3;
var RED_LED   = A4;

// LEFT WHEEL
var LW_RPWM;
var LW_LPWM;
var LW_REN;
var LW_LEN;

// RIGHT WHEEL
var RW_RPWM;
var RW_LPWM;
var RW_REN;
var RW_LEN;

// ROBOT SPEED OBJECT
var oSPEED = {'L':0.0, 'R':0.0};

// COMMUNICATION VARIABLES
var BUFFER = "";
var CMD = "";

// COMMUNICATION CONSTANTS
const CONNECTED =    "OK+CONN";
const DISCONNECTED = "OK+LOST";

// FSM STATES
const WAIT = 'WAIT';
const RUN  = 'RUN';
const WALK = 'WALK';
const STOP = 'STOP';

var STATE = 'ERR';

function startBlink(led) {
  var value = false;
  var interval = setInterval(function() {
    value = !value;
    digitalWrite(led, value);
  }, 500);
  return interval;
}

function stopBlink(interval, led) {
  clearInterval(interval);
  digitalWrite(led, false);
}

function listen() {
  Serial3.on('data', function(ch){
    BUFFER += ch;
  });
}

function setWheelSpeed(left, right) {
  // left wheel control
  if(left < 0) {
  }
  else if(left >= 0) {
  }
  // right wheel control
  if(right < 0) {
  }
  else if(right >= 0) {
  }
}

var leftWhSpeed  = 0;
var rightWhSpeed = 0;
function nextState() {
  //{'L': -1.0, 'R': 1.0}
  // check for bluetooth disconnect
  var beginIdx = BUFFER.indexOf('OK+');
  if(beginIdx > -1) {
    var idx1 = BUFFER.indexOf('OK+CONN');
    var idx2 = BUFFER.indexOf('OK+LOST');
    if(idx1 > -1) {
      CMD = CONNECTED;
      BUFFER = "";
    }
    else if(idx2 > -1) {
      CMD = DISCONNECTED;
      BUFFER = "";
    }
  }
  // check for speed command
  beginIdx = BUFFER.indexOf('*');
  var endIdx = BUFFER.indexOf('#');
  if(beginIdx>-1 && endIdx > beginIdx) {
    var obj = 'undefined';
    var cmd = BUFFER.substr(beginIdx, endIdx - beginIdx + 1);
    console.log('CMD: ' + cmd);

    values = cmd.substr(beginIdx + 1, endIdx).split(':');
    leftWhSpeed  = parseInt(values[0], 10);
    rightWhSpeed = parseInt(values[1], 10);
    if(Math.abs(leftWhSpeed)>0 || Math.abs(rightWhSpeed)>0) {
      CMD = WALK;
    }
    else if(leftWhSpeed == 0 && rightWhSpeed == 0) {
      CMD = STOP;
    }

    // Trim buffer head
    if(endIdx+1 < BUFFER.length)
      BUFFER = BUFFER.substr(endIdx+1, BUFFER.length - endIdx);
    console.log(BUFFER);
  }
}

var blinkInt = 'undefined';
function loop() {
  // FSM MAIN LOOP
  if(STATE == WAIT) {
    if(CMD == CONNECTED) {
      STATE = RUN;
      digitalWrite(RED_LED, false);
      digitalWrite(GREEN_LED, true);
    }
  }
  else if(STATE == RUN) {
    if(CMD == DISCONNECTED) {
      STATE = WAIT;
      digitalWrite(RED_LED, true);
      digitalWrite(GREEN_LED, false);
    }
    else if(CMD == WALK) {
      STATE = WALK;
      setWheelSpeed(leftWhSpeed, rightWhSpeed);
      blinkInt = startBlink(GREEN_LED);
    }
  }
  else if(STATE == WALK) {
    if(CMD == DISCONNECTED) {
      STATE = WAIT;
      setWheelSpeed(0, 0);
      stopBlink(blinkInt, GREEN_LED);
      digitalWrite(RED_LED, true);
    }
    else if(CMD == STOP) {
      STATE = RUN;
      setWheelSpeed(0, 0);
      stopBlink(blinkInt, GREEN_LED);
      digitalWrite(GREEN_LED, true);
    }
  }
  else if(STATE == STOP) {
    if(CMD == DISCONNECTED) {
      STATE = WAIT;
      digitalWrite(RED_LED, true);
      digitalWrite(GREEN_LED, false);
    }
  }
  else {
    setWheelSpeed(0, 0);
    stopBlink(blinkInt, GREEN_LED);
    startBlink(RED_LED);
  }
  nextState();
}

function initialize(begin_fn, end_fn) {
  Serial3.setup(9600);
  Serial3.write('AT');
  console.log('Check bluetooth module');
  setTimeout(function(){
    data = Serial3.read(2);
    if(data == 'OK') {
      STATE = WAIT;
      digitalWrite(RED_LED, true);
      console.log(data);
      begin_fn();
    }
    else {
      startBlink(RED_LED);
      console.log('Bluetooth init error');
      end_fn();
    }
  }, 500);
}

function main() {
  listen();
  setInterval(function(){
    loop();
  }, 100);
}

function error() {
}


initialize(main, error);
