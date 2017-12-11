#include "HIH9130.h"
#include "stdio.h"

bool HIH9130Wireless::parseSensorData(byte* data, int len){
    int sensorType = getSensorType(data[sensorTypeMSBIndex], data[sensorTypeLSBIndex]);
    if(data[0] != headerByte || sensorType != HIH9130SensorType){
        return false;
    }
    
    char publishData[100];
    sprintf(publishData, "NodeID:%i|firmware:%i|battery voltage:%0.2f|transmissionID:%i|sensorType:%i|humidity:%0.2f|temperature:%0.2f", data[nodeIDIndex], data[firmwareVersionIndex], getBatteryVoltage(data[batteryVotageMSBIndex], data[batteryVoltageLSBIndex]), data[transmitCounterIndex], getSensorType(data[sensorTypeMSBIndex], data[sensorTypeLSBIndex]), getHumidity(data[humidityMSBIndex], data[humidityLSBIndex]), getTemperature(data[temperatureMSBIndex],data[temperatureLSBIndex]));
    Serial.println(publishData);
    return Particle.publish(String(data[nodeIDIndex]), String(publishData));
}

float HIH9130Wireless::getBatteryVoltage(byte msb, byte lsb){
    float battery = ((msb * 256) + lsb);
    float voltage = 0.00322 * battery;
    return voltage;
}
float HIH9130Wireless::getTemperature(byte msb, byte lsb){
    float temp = ((msb * 256) + (lsb & 0xFC)) / 4;
    float cTemp = (temp / 16384.0) * 165.0 - 40.0;
    return cTemp * 1.8 + 32;
}
float HIH9130Wireless::getHumidity(byte msb, byte lsb){
    return ((((msb & 0x3F) * 256) + lsb) * 100.0) / 16383.0;
}
int HIH9130Wireless::getSensorType(byte msb, byte lsb){
    return((msb * 265)+lsb);
}
