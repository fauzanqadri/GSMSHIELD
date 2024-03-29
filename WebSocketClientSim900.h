/*
 WebsocketClient, a websocket client for Arduino
 Copyright 2011 Kevin Rohling
 http://kevinrohling.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef WEBSOCKETCLIENTSIM900_H
#define WEBSOCKETCLIENTSIM900_H_

#include <string.h>
#include <stdlib.h>
#include <WString.h>
#include "tcp_client.h"
#include "Arduino.h"

//Uncomment this to use WIFLY Client
// #define WIFLY true

class WebSocketClientSim900 {
	public:
		typedef void (*DataArrivedDelegate)(WebSocketClientSim900 client, String data);
		bool connect(char hostname[], char path[] = "/", int port = 80);
        bool connected();
        void disconnect();
		void monitor();
		void setDataArrivedDelegate(DataArrivedDelegate dataArrivedDelegate);
		void send(String data);
	private:
        String getStringTableItem(int index);
        void sendHandshake(char hostname[], char path[]);
        Tcp_Client _client;
        DataArrivedDelegate _dataArrivedDelegate;
        bool readHandshake();
        String readLine();
        char *sim900EOF();
};


#endif
