#include "WiFi.h"
#include <SoftwareSerial.h>

WiFi::WiFi(int rx, int tx):
esp8266(rx, tx),
found(true),
valSensor(1),
connected(false) {
	esp8266.begin(115200);
}

void WiFi::connect(String ap, String password) {
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ ap +"\",\""+ password +"\"",20,"OK");
}

void WiFi::sendCommand(String command, int maxTime, char readReplay[]) {

	while(countTimeCommand < (maxTime*1)) {
		esp8266.println(command);
		if(esp8266.find(readReplay)) {
			found = true;
			break;
		}
		countTimeCommand++;
	}
	
	if(found == true) {		
		countTrueCommand++;
	} else {
		countTrueCommand = 0;
	}
	countTimeCommand = 0;
	found = false;
}
