#include "tcp_client.h"
int CONN_COUNTER = 0 ;
Tcp_Client::Tcp_Client(){};
int Tcp_Client::initialize(char hostname[],  int port)
{
   // if(CONN_COUNTER == 8){
   // 	return 0;
   // }
   // CONN_COUNTER++;
   // gsm.SimpleWrite("AT+CIPSTART=");
   // gsm.SimpleWrite(CONN_COUNTER);
   // gsm.SimpleWrite(",\"TCP\",\"");
   // gsm.SimpleWrite(hostname);
   // gsm.SimpleWrite("\",");
   // gsm.SimpleWriteln(port);
   // SendData();
   //  switch(gsm.WaitResp(1000, 200, "OK")){
   //  case RX_TMOUT_ERR:
   //  	CONN_COUNTER--;
   //  	return 0;
   //  break;
   //  case RX_FINISHED_STR_NOT_RECV: 
   //  	CONN_COUNTER--;
   //  	return 0; 
   //  break;
   //  }

   //  delay(1000);
   //  sock_id = CONN_COUNTER;
    return 1;
}
int Tcp_Client::SendData(){
  gsm.SimpleWrite("AT+CIPSEND=");
  gsm.SimpleWriteln(sock_id);
  switch(gsm.WaitResp(5000, 200, ">")){
  	case RX_TMOUT_ERR: 
  		return 0;
  	break;
  	case RX_FINISHED_STR_NOT_RECV: 
  		return 0; 
  	break;
    return 1;
  }
}
int Tcp_Client::SendData(String data){
  gsm.SimpleWrite("AT+CIPSEND=");
  gsm.SimpleWrite(sock_id);
  gsm.SimpleWrite(",");
  gsm.SimpleWrite((char)0);
  gsm.SimpleWrite(data);
  gsm.SimpleWriteln((char)255);
  switch(gsm.WaitResp(5000, 200, ">")){
    case RX_TMOUT_ERR: 
      return 0;
    break;
    case RX_FINISHED_STR_NOT_RECV: 
      return 0; 
    break;
    return 1;
  }
}
int Tcp_Client::SendDataln(String data){
  gsm.SimpleWrite("AT+CIPSEND=");
  gsm.SimpleWrite(sock_id);
  gsm.SimpleWrite(",");
  // gsm.SimpleWrite((char)0);
  gsm.SimpleWriteln(data);
  // gsm.SimpleWriteln((char)255);
  switch(gsm.WaitResp(5000, 200, ">")){
    case RX_TMOUT_ERR: 
      return 0;
    break;
    case RX_FINISHED_STR_NOT_RECV: 
      return 0; 
    break;
    return 1;
  }
}

uint8_t Tcp_Client::read(){
 return gsm.read();
}
bool Tcp_Client::connected(){
  if(sock_id > 0)
    return true;
  return false;
}
int Tcp_Client::stop(){
  gsm.SimpleWrite("AT+CIPCLOSE=");
  gsm.SimpleWriteln(sock_id);
  gsm.WaitResp(5000, 100, "CLOSE OK");
  if(!gsm.IsStringReceived("CLOSE OK"))
    return 0;
  else{
    return 1;
  }
}
bool Tcp_Client::available(){
  gsm.SimpleWrite("AT+CIPSEND=");
  gsm.SimpleWriteln(sock_id);
  gsm.WaitResp(5000, 100, "OK");
  if(!gsm.IsStringReceived("OK"))
    return false;
  else{
    return true;
  }
}
