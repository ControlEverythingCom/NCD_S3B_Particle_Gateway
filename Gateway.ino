// This #include statement was automatically added by the Particle IDE.
#include "HIH9130.h"

// This #include statement was automatically added by the Particle IDE.
#include "S3B.h"

HIH9130Wireless HIH9130sensor;



//Global variables
unsigned long serialTimeout = 500;
byte receiveBuffer[256];
unsigned long tOut = 3000;

unsigned long interval = 500;
unsigned long lastSend = 0;

int LED = D7;

//Global objects
S3B module;

void setup() {
    Serial1.begin(9600);
    module.init();
    pinMode(LED, OUTPUT);
}

void loop() {
    //Check for transmission from Master
    if(Serial1.available() != 0){
        parseReceivedData();
    }
}

void parseReceivedData(){
    // Serial.println("Got a command");
    byte startDelimiter = Serial1.read();
    if(startDelimiter == 0x7E){
        unsigned long startTime = millis();
        while(Serial1.available() < 2 && millis() <= tOut+startTime);
        if(Serial1.available() < 2){
            Serial.println("Timeout");
            return;
        }
        byte lenMSB = Serial1.read();
        byte lenLSB = Serial1.read();
        int newDataLength = (lenMSB*256)+lenLSB;
        
        int count = 0;
        while(count != newDataLength+1 && millis() <= tOut+startTime){
            if(Serial1.available() != 0){
                receiveBuffer[count] = Serial1.read();
                count++;
            }
        }
        if(count < newDataLength+1){
            Serial.println("Timeout2");
            Serial.printf("Received Bytes: %i, expected %i \n", count, newDataLength+1);
            return;
        }
        byte newData[newDataLength+4];
        newData[0] = startDelimiter;
        newData[1] = lenMSB;
        newData[2] = lenLSB;
        for(int i = 3; i < newDataLength+4; i++){
            newData[i] = receiveBuffer[i-3];
        }
        if(!module.validateReceivedData(newData, newDataLength+4)){
            Serial.println("Invalid Data");
            return;
        }
        //get length of new data
        int receivedDataLength = module.getReceiveDataLength(newData);
        byte receivedData[receivedDataLength];
        int validDataCount = module.parseReceive(newData, receivedData, newDataLength+4);
        if(validDataCount == receivedDataLength){

        }
        Serial.print("Received: ");
        for(int i = 0; i < receivedDataLength; i++){
            Serial.printf("%02X, ", receivedData[i]);
        }
        if(receivedData[0] != 0x7F){
            return;
        }
        switch(sensorType(receivedData[6], receivedData[7])){
            case 1:
            HIH9130sensor.parseSensorData(receivedData, receivedDataLength);
            break;
        }
        
        
    }else{
        Serial.printf("First byte not valid, it was: 0x%x \n", startDelimiter);
    }
}

int sensorType(byte msb, byte lsb){
    return((msb * 265)+lsb);
}
