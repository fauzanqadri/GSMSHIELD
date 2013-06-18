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

#include <WebSocketClientSim900.h>
#include <WString.h>
#include <string.h>
#include <stdlib.h>

prog_char stringVar[] PROGMEM = "{0}";
prog_char clientHandshakeLine1[] PROGMEM = "GET {0} HTTP/1.1";
prog_char clientHandshakeLine2[] PROGMEM = "Upgrade: WebSocket";
prog_char clientHandshakeLine3[] PROGMEM = "Connection: Upgrade";
prog_char clientHandshakeLine4[] PROGMEM = "Host: {0}";
prog_char clientHandshakeLine5[] PROGMEM = "Origin: ArduinoWebSocketClient";
prog_char serverHandshake[] PROGMEM = "HTTP/1.1 101";


PROGMEM const char *WebSocketClientStringTable[] =
{   
    stringVar,
    clientHandshakeLine1,
    clientHandshakeLine2,
    clientHandshakeLine3,
    clientHandshakeLine4,
    clientHandshakeLine5,
    serverHandshake
};

String WebSocketClientSim900::getStringTableItem(int index) {
    char buffer[35];
    strcpy_P(buffer, (char*)pgm_read_word(&(WebSocketClientStringTable[index])));
    return String(buffer);
}

bool WebSocketClientSim900::connect(char hostname[], char path[], int port) {
    bool result = false;

    if (_client.initialize(hostname, port)) {
        sendHandshake(hostname, path);
        result = readHandshake();
    }
    
	return result;
}


bool WebSocketClientSim900::connected() {
    return _client.connected();
}

void WebSocketClientSim900::disconnect() {
    _client.stop();
}

void WebSocketClientSim900::monitor () {
    char character;
    
	if (_client.available() > 0 && (character = _client.read()) == 0) {
        String data = "";
        bool endReached = false;
        while (!endReached) {
            character = _client.read();
            endReached = character == -1;

            if (!endReached) {
                data += character;
            }
        }
        
        if (_dataArrivedDelegate != NULL) {
            _dataArrivedDelegate(*this, data);
        }
    }
}

void WebSocketClientSim900::setDataArrivedDelegate(DataArrivedDelegate dataArrivedDelegate) {
	  _dataArrivedDelegate = dataArrivedDelegate;
}


void WebSocketClientSim900::sendHandshake(char hostname[], char path[]) {
    String stringVar = getStringTableItem(0);
    String line1 = getStringTableItem(1);
    String line2 = getStringTableItem(2);
    String line3 = getStringTableItem(3);
    String line4 = getStringTableItem(4);
    String line5 = getStringTableItem(5);
    
    line1.replace(stringVar, path);
    line4.replace(stringVar, hostname);
    String data = line1 + "\n" + line2 + "\n" + line3 + "\n" + line4 + "\n" + line5 + "\n" + sim900EOF();
    _client.SendDataln(data);
    
    // gsm.SimpleWrite(line1);
    // gsm.SimpleWrite("\n");
    // gsm.SimpleWrite(line2);
    // gsm.SimpleWrite("\n");
    // gsm.SimpleWrite(line3);
    // gsm.SimpleWrite("\n");
    // gsm.SimpleWrite(line4);
    // gsm.SimpleWrite("\n");
    // gsm.SimpleWrite(line5);
    // gsm.SimpleWrite("\n");
    // gsm.SimpleWrite(sim900EOF());

}



bool WebSocketClientSim900::readHandshake() {
    gsm.WaitResp(5000, 100, "OK");
    if(!gsm.IsStringReceived("SEND OK"))
        return false;

    bool result = false;
    char character;
    String handshake = "", line;
    int maxAttempts = 300, attempts = 0;
    
    while(_client.available() == 0 && attempts < maxAttempts) 
    { 
        delay(100); 
        attempts++;
    }
    
    while((line = readLine()) != "") {
        handshake += line + '\n';
    }
    
    String response = getStringTableItem(6);
    result = handshake.indexOf(response) != -1;
    
    if(!result) {
        _client.stop();
    }
    
    return result;
}

String WebSocketClientSim900::readLine() {
    String line = "";
    char character;
    
    while(_client.available() > 0 && (character = _client.read()) != '\n') {
        if (character != '\r' && character != -1) {
            line += character;
        }
    }
    
    return line;
}

void WebSocketClientSim900::send (String data) {
    gsm.SimpleWrite((char)0);
	gsm.SimpleWrite(data);
    gsm.SimpleWrite((char)255);
    gsm.WaitResp(5000, 100, "OK");
    // if(!gsm.IsStringReceived("SEND OK"))
    //     return false;    
}
char *WebSocketClientSim900::sim900EOF(){
  char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';
  return end_c;
}
