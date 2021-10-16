clc;
clear all;
close all;

% Author: Fabio Bernardini
% 
% In this script there is the subscription to the MQTT topic to receive the sensor data
% coming from the ESP8266. The data are plotted to see their values in real-time
%  

%Creation of the MQTT communication
myMQTT=mqtt('tcp://broker.emqx.io','Port',1883);
 
% MATLAB is the publisher, ESP8266 is the subscriber: if Message is 0 the LED on the ESP
% is off, if Message is 1 the LED is ON
Topic = 'Led/status';
Message = '0'; 
publish(myMQTT,Topic,Message);
 
%MATLAB is the susbcriber, ESP8266 s the publisher: ESP sends the sensor and MATLAB receives them 
Topic2 = "ESP8266/dataSensor";
Data = subscribe(myMQTT,Topic2);
 
pause(6);
 
x=0;
y=0;

% Representation of the data
while true
   values=Data.MessageCount;
   if(values>0)
      op = jsondecode(read(Data));
      temp = op.parameters.temp;
      hum = op.parameters.humidity;
       
      x = [x,temp];
      y = [y,hum];
       
      subplot(2,1,1);
      plot(x,'color','r','LineWidth',2,'marker','o','MarkerFacecolor','r','Markerdgecolor','r','MarkerSize',8);
      xlabel('Data, n')
      ylabel('Temp. (°C)')
      ylim([0,100]);
      grid ON;
      title("Temperature");
       
      subplot(2,1,2);
      plot(y,'color','r','LineWidth',2,'marker','o','MarkerFacecolor','b','Markerdgecolor','b','MarkerSize',8);
      xlabel('Data, n')
      ylabel('Hum. (%)')
      ylim([0,100]);
      grid ON;
      title("Humidity");
       
      drawnow
      pause(3)      
   end  
end