<h2><strong>ESP8266 Triple Server</strong></h2>

This project provides a Web Server Framework that supports 3 simultaneous protocols:

1. http
2. MQTT
3. CoAP

Setup:

1. Copy the http_coap_mqtt_server folder to your Arduino sketch folder.
2. Copy the UtilityFunctions folder to your Arduino libraries folder.
3. Copy the coap folder to your Arduino libraries folder.
4. Copy the webserver folder to your Arduino libraries folder.
5. Change the following in the http_coap_mqtt_server.ino file to match your network settings:

const char* ssid = "YOURWIFISSID";
const char* password = "YOURWIFIPASSWORD";
const IPAddress ipadd(192,168,0,132);     
const IPAddress ipgat(192,168,0,1); 

define SVRPORT 9701

6.0 Server Setting

6.1 To use the standard Arduino Web Server library, which polls for connections, use this define in the sketch:

define SVR_TYPE SVR_HTTP_LIB

6.2 To use the EspressIf SDK Web Server API, which uses event callbacks, use this define in the sketch.h file:

define SVR_TYPE SVR_HTTP_SDK

Operation:

While not necessary, the code assumes an LED is connected to GPIO16. This LED is ON upon 
power-up and is turned OFF once initialization completes.


Server test:

Note: In order to test the CoAP server, Mozilla Firefox must be installed with the 
Copper (Cu) add-on user-agent installed.

To install Copper:<br>
   a. Open the Mozilla Firefox browser<br>
   b. Enter the URL:<br>
      https://addons.mozilla.org/en-US/firefox/addon/copper-270430/<br>
   c. Click on the "Add to Firefox" button<br>

Here is the test...

First, compile and load the sketch to the ESP8266. With the sketch running, follow
the following 3 server-specific steps:

1. http server:<br>
   a. Open the html file mqtt_server.html in a web browser<br>
   b. Click the "Request via HTTP" button<br>

2. mqtt server:<br>
   a. pen the html file mqtt_server.html in a web browser<br>
   b. Click the "Request via MQTT" button<br>

3. coap server:<br>
   a. open the Mozilla Firefox web browser and enter the URL:<br>
      coap://192.168.0.132:5683<br>
   b. Click the "Discover" button<br>
   c. Click the "request" service (left part of web browser window)<br>
   d. Enter "/?request=GetSensors" in the Outgoing Payload tab (center of web browser window)<br>
   e. Click the "Put" button

For each of the 3 servers:

A JSON string will be returned with the sensor values in this format:

{<br>
"Ain0":"316.00",<br>
"Ain1":"326.00",<br>
"Ain2":"325.00",<br>
"Ain3":"314.00",<br>
"Ain4":"316.00",<br>
"Ain5":"163.00",<br>
"Ain6":"208.00",<br>
"Ain7":"333.00",<br>
"SYS_Heap":"25408",<br>
"SYS_Time":"26"<br>
}<br>

