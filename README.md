# External Pihole WiFi-Display
External Nextion Display in combination with an ESP8266 NodeMCU V3 to display PiHole Stats on a Nextion 7 inch display.

In this reporisitory i used a Nextion NX8048T070 display with 7 inch. Sure, anyone can use cheaper free solutions with any
free android or iOS applikations to show their friends the imposing results of your lokal PiHole installation. But i don't
want every time to grab my smartphone and open the needed applikation or the PiHole Webinterface to see, if PiHole running
with the current statistics. So i can placed the display on the wall in addition to my other smart home components and can
see immediately, if all ok.

I looked for a suitable solution for myself on the Internet, but unfortunately I didn't find the right one for me. So I took
a 3 year old project which found on Github (https://github.com/arejaycodes/piholenextion) and built it from scratch new and
rebuild this from 3,5 inch to 7 inch, while his files doesn't work for an 7 inch display.

The first start and finish the planed project.
![Alt text](project_pictures/nextion_NX8048T070_7_inch_in_action_update.png?raw=true "Pihole Nestion WiFi Display cin action")

As next step i fixed the NodeMCU V3 with glow behind the display:
![Alt text](/project_pictures/nextion_NX8048T070_7_inch_build_rearside.png?raw=true "Pihole Nestion WiFi Display build rearside")

A friend from me builds the case with his nice professional 3D Printer setup for me. The STL files can be find in this
repository to, when every one this needed.
![Alt text](project_pictures/nextion_NX8048T070_7_inch_case.png?raw=true "Pihole Nestion WiFi Display case")

Build in the Nextion display in the case ...
![Alt text](project_pictures/nextion_NX8048T070_7_inch_build_in.png?raw=true "Pihole Nestion WiFi Display built in")

![Alt text](project_pictures/nextion_NX8048T070_7_inch_build_in_complete.png?raw=true "Pihole Nestion WiFi Display build in complete")

NOTICE: Please read the instructions in the Arduino project file before you run this project!

Copy the Nextion Display TFT File on a blank micro sd-card and palce them in the micro sd cardholder on the nextion display.
Power on the display and you can see the update routine. If the update ready, switch off the display and remove the micro
sd card from the cardholder.

As next step load the .ino file in your Arduino editor, make the needed wifi and pihole/ntp (pihole and ntp must run on the same
maschine) changes and install the needed libarys, compile and upload it to your esp8266 nodemcu v3. 

Future request's are welcome ...
