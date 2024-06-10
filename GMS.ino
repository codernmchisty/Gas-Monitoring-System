//Section 1
#include <SoftwareSerial.h>
#define RX 2
#define TX 3
String SSID = "ISTT-2 (ACC.ADMIN.ECE)";          //WiFi Name
String PASS = "20402040";         //WiFi PASSWORD
String API = "8F5IFEXQ1Y7EM55G";   //ThingSpeak Key
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
SoftwareSerial esp8266(RX,TX);


//Section 2

//MQ136
#define mq136Pin A0
short mq136Val;

//MQ135
#define mq135Pin A1
short mq135Val;

//MQ4
#define mq4Pin A2
short mq4Val;

 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ SSID +"\",\""+ PASS +"\"",20,"OK");
  
  pinMode(mq136Pin, INPUT);
  pinMode(mq135Pin, INPUT);
  pinMode(mq4Pin, INPUT);

}

void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getmq136Value()+"&field2="+getmq135Value()+"&field3="+getmq4Value();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);
 delay(500); 
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
  
}


String getmq136Value(){
  mq136Val= analogRead(mq136Pin);

  Serial.print(mq136Val);
  Serial.print("  ");  
  
  return String(mq136Val);
  }

String getmq135Value(){
  mq135Val= analogRead(mq135Pin);

  Serial.print(mq135Val);
  Serial.print("  ");
  
  return String(mq135Val);
  }

  String getmq4Value(){
  mq4Val= analogRead(mq4Pin);

  Serial.print(mq4Val);
  Serial.print("  ");
  
  return String(mq4Val);
  }


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);         //at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Ok");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }


 
