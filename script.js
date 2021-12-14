// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);

// Create Temperature Gauge
var gaugeTempIn = new LinearGauge({
  renderTo: 'gauge-temperature-indoor',
  width: 120,
  height: 250,
  units: "Indoor Temperature F",
  minValue: 65,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 105,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 1,
  valueInt: 3,
  majorTicks: [
      "65",
      "70",
      "75",
      "80",
      "85",
      "90",
      "95",
      "100",
      "105"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 95,
          "to": 105,
          "color": "rgba(200, 50, 50, .75)"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw(); 
 
// Create Humidity Gauge
var gaugeHumIn = new RadialGauge({
  renderTo: 'gauge-humidity-indoor',
  width: 200,
  height: 250,
  units: "Indoor Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 0,  
  valueInt: 2,
  majorTicks: [
      "0",
      "20",
      "40",
      "60",
      "80",
      "100"

  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#03C0C1"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

var gaugeTempOut = new LinearGauge({
  renderTo: 'gauge-temperature-outdoor',
  width: 120,
  height: 250,
  units: "Outdoor Temperature F",
  minValue: 65,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 105,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 1,
  valueInt: 3,
  majorTicks: [
      "65",
      "70",
      "75",
      "80",
      "85",
      "90",
      "95",
      "100",
      "105"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 95,
          "to": 105,
          "color": "rgba(200, 50, 50, .75)"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw();

// Create Humidity Gauge
var gaugeHumOut = new RadialGauge({
  renderTo: 'gauge-humidity-outdoor',
  width: 200,
  height: 250,
  units: "Outdoor Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 0,  
  valueInt: 2,
  majorTicks: [
      "0",
      "20",
      "40",
      "60",
      "80",
      "100"

  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#03C0C1"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var tempIn = myObj.tempIndoor;
      var humIn = myObj.humIndoor;
      var tempOut = myObj.tempOutdoor;
      var humOut = myObj.humOutdoor;
      gaugeTempIn.value = tempIn;
      gaugeHumIn.value = humIn;
      gaugeTempOut.value = tempOut;
      gaugeHumOut.value =  humOut;
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  
  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);
  
  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);
  
  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    var tempIn = myObj.tempIndoor;
    var humIn = myObj.humIndoor;
    var tempOut = myObj.tempOutdoor;
    var humOut = myObj.humOutdoor;
    gaugeTempIn.value = tempIn;
    gaugeHumIn.value = humIn;
    gaugeTempOut.value = tempOut;
    gaugeHumOut.value =  humOut;
    document.getElementById("outdoorT").style.display = "block";
    document.getElementById("outdoorH").style.display = "block";
    if (tempOut == 0){document.getElementById("outdoorT").style.display = "none";}
    if (humOut == 0){document.getElementById("outdoorH").style.display = "none";}
    
  }, false);
  
  

}



