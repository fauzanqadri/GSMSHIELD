
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H_
#include "SIM900.h"

class Tcp_Client
{
private:
	int sock_id;
public:
	Tcp_Client();
	int initialize(char hostname[],  int port);
	int SendData();
	int SendData(String data);
	int SendDataln(String data);
	uint8_t read();
	int stop();
	bool available();
	bool connected();
};
#endif