# HeizungESP8266

Our present time demands for more interaction of all household components.
Many older components do not provide a connection to other smart home components.
Thus expensive components need to be extended with an interface to communicate with the rest of the smart home.

Here i will extend our Vaillant VKO 246 heater with an VRC 420 control with a Wifi Chip. 
The ESP8266 which will be used here, has become quite popular on makers.
Now this microcontroller will request parameters of the heaters and publish the parameters via MQTT, 
which can be easily integrated in any smart home software.

The Heater features the Vaillant X6 serial interface.
Unfortunately Vaillant did not want to provide me any matirial for theirs X6 interface, so i needed to reverse engineer it.

Preliminary work was done by the Ethersex Project: http://old.ethersex.de/index.php/Vaillant_X6_Schnittstelle
They decoded the Protocol and analyzed their own heater parameters.
Unfortunately mine heater isnt using the same parameters but at least the standard X6 protocol.
To figure out the missing Parameters i recorded signals of my heater, 
and on this basis developed a software model of the heater (which will not be presented here).
This software model of the heater allows me to directly test the ESP8266 Software 
and to figure out unknown Parameters with the Vaillant vrDialog Software.
