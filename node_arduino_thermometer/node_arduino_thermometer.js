const https = require('https');
const SerialPort = require('serialport');
const port = new SerialPort('COM4', {
  baudRate: 115200
});

// Which day/time are we showing?
var forecast_index = 0;
// The stored forecast forecastData
var forecastData;

// Web endpoint
const options = {
  hostname: 'api.weather.gov',
  port: 443,
  path: '/gridpoints/LOX/158,47/forecast',
  method: 'GET',
  headers: { 'User-Agent': 'Mozilla/5.0' }
};

// We'll use this function to regularly get the NWS API data
function getData() {
  var req = https.request(options, res => {
    console.log('statusCode: ' + res.statusCode);
    var body = '';

    // Store the data we get in the response
    res.on('data', d => {
      body += d;
    })

    // When we get it, send the data to the Arduino and store it
    res.on('end', function(){
      var response = JSON.parse(body);
      forecastData = response.properties.periods;
      sendTempToArduino();
    })
  });

  req.on('error', error => {
    console.error(error)
  });

  req.end();
}

// Just writes the number literally -- since we're in LA it'll always be within the range
function sendTempToArduino() {
  console.log("Temperature for " + forecastData[forecast_index].name + ": " + forecastData[forecast_index].temperature);
  port.write(forecastData[forecast_index].temperature + "\n");
}

// Change which day we're showing the forecast for
function readSerialData(d) {
  forecast_index = Number(d) % forecastData.length;
  sendTempToArduino();
}

// Initialize the data; get a new version every hour
getData();
setInterval(getData, 3600000);

// Read data from Serial
port.on('data', readSerialData);
