#include "spark_wiring_constants.h"
#include "spark_wiring_string.h"
#include "spark_wiring_cloud.h"
#include "spark_wiring_print.h"
#include "spark_wiring_usbserial.h"


class HIH9130Wireless{
public:
    bool parseSensorData(byte* data, int len);
    
private:
    float getBatteryVoltage(byte msb, byte lsb);
    float getTemperature(byte msb, byte lsb);
    float getHumidity(byte msb, byte lsb);
    int getSensorType(byte msb, byte lsb);
    int sensorType;
    float batteryVoltage;
    float temperature;
    float humidity;
    byte headerByte = 0x7F;
    uint8_t nodeIDIndex = 1;
    uint8_t firmwareVersionIndex = 2;
    uint8_t batteryVotageMSBIndex = 3;
    uint8_t batteryVoltageLSBIndex = 4;
    uint8_t transmitCounterIndex = 5;
    uint8_t sensorTypeMSBIndex = 6;
    uint8_t sensorTypeLSBIndex = 7;
    uint8_t humidityMSBIndex = 9;
    uint8_t humidityLSBIndex = 10;
    uint8_t temperatureMSBIndex = 11;
    uint8_t temperatureLSBIndex = 12;
    uint8_t HIH9130SensorType = 0x01;
    
    
};
