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

function listen() {
  Serial3.on('data', function(ch){
    BUFFER += ch;
  });
}

function nextState() {
  //{'L': -1.0, 'R': 1.0}
  var beginIdx = BUFFER.indexOf('OK+');
  if(beginIdx > -1) {
    CMD = CONNECTED;
  }
}

function initialize(begin_fn, end_fn) {
    Serial3.write('AT');
    setTimeout(function(){
      data = Serial3.read(2);
      if(data == 'OK') {
        STATE = WAIT;
        digitalWrite(RED_LED, true);
        begin_fn();
      }
      else {
        startBlink(RED_LED);
        end_fn();
      }
    }, 200);
}

function loop() {
  // FSM MAIN LOOP
  if(STATE == WAIT) {
    if(CMD == CONNECTED) {
      state = RUN;
      digitalWrite(RED_LED, false);
      digitalWrite(GREEN_LED, true);
    }
  }
  else if(STATE == RUN) {
  }
  else if(STATE == WALK) {
  }
  else if(STATE == STOP) {
  }
  else {
    digitalWrite(RED_LED, false);
    console.log('UNEXPECTED STATE');
  }

  nextState();
}

function main() {
  listen();
  setInterval(function(){
    loop();
  }, 50);
}

function error() {
}

Serial3.setup(9600);
initialize(main, error);
