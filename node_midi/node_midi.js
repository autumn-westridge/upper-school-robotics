const midi = require('midi');
const input = new midi.Input();

// Print out all the ports
for (var i = 0; i < input.getPortCount(); i++) {
  console.log(input.getPortName(i));
}

input.on('message', (deltaTime, message) => {
  // Message format is: message type, note number, velocity
  var message_type = message[0];
  var note = message[1];
  var velocity = message[2];

  // Message time 144 is note on
  if (message_type == 144) {
    console.log(note + " " + velocity);
  }
});

input.openPort(2);