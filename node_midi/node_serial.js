const SerialPort = require('serialport');
const port = new SerialPort('COM4', {
  baudRate: 115200
});

// This sends data to Arduino
function sendDataToArduino(data) {
  console.log("Sending to Arduino: " + data);
  port.write(data + "\n");
}
