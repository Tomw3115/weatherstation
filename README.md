# weatherstation
modification to previously published code for esp32 that displays indoor and outdoor readings

The original code was published by Rui Santos at 
https://RandomNerdTutorials.com/esp32-web-server-gauges/

This version uses a second esp32 and sensor placed outside to send temperature and humidity information back to the server, which then sends the information to http clients 
using its webserver capabilities as in the original code.
  

Modifications to the original code:
1.  Modified to use DHT11 sensors for temperature and humidity.
2.  Modified to display temperature in degrees F 
3.  Added static IP address assignment
4.  Added outdoor sensor to read outdoor weather.  The outdoor sensor periodically reads the temperature and humidity and sends the values to the primary server in an HTTP GET   message.  The primary server then responds to the outdoor device and reads the outdoor temperature and humidity from the GET message.  It thens relays these values to the connected clients in the event update  message along with the indoor values.  The Javascript and index.html are modified to display 4 canvas gauges and update the values from the event update message.  If the server has not received an update from the outdoor sensors for an extended period of time, it resets the outdoor temp and humidity to 0 and the javascript sets display style to "none" on the outdoor gauges.  When outdoor readings are later available the display shows all 4 readings again.

5.  An example screen shot is in file CaptureWeatherGauge.PNG

  
