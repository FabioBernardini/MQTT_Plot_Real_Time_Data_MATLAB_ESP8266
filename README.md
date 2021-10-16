# MQTT_Plot_Real_Time_Data_MATLAB_ESP8266
How to plot real-time data using MQTT? 

In this repository, you can find the script codes to implement a MQTT communication between an ESP8266 and a MATLAB editor. In particular, data sensor coming from a DHT11 sensor linked to an ESP8266 12-E will be represent in real time on MATLAB through a MQTT communication.

In this case, the ESP8266 will be the publisher, while the MATLAB will be the subscriber and it will represent the sensor data.

The script .m holds the code using to subscribe to the particular Topic and then to represent the sensor data.

The script .ino holds the code to compile and to load on the ESP8266. This code allows to receive the data from the sensor and then to publish them on the topic.
