/*
 * This module uses the esp8266 module
 * to connect to WiFi and upload the data
 */
#include <SoftwareSerial.h>

class WiFi {

public:
	WiFi(int rx, int tx);

	void connect(String ap, String password);

	void sendCommand(String command, int maxTime, char readReplay[]);

	void sendData(String host, String port, String data);

	bool connected;

private:
	SoftwareSerial esp8266;
	int countTrueCommand;
	int countTimeCommand; 
	bool found; 
	int valSensor;
};
