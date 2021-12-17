#include <Adafruit_ADS1X15.h>
#include <DFRobot_ESP_EC.h>
#include <U8g2lib.h>

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;
U8G2_SH1106_128X64_NONAME_1_HW_I2C _display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void initializeOLEDDisplay();
void showScreen(float ph, float ec);
void showScreen(float ph, short ppm);
float getPh();
float getEC();
float enableECCalibration();
short getPPM();
int getMedianNum(int bArray[], int iFilterLen);
float getTemperature();

void setup()
{
    initializeOLEDDisplay();
    ec.begin();
    ads.setGain(GAIN_TWO);
    ads.begin();
    // Serial.begin();
}

void loop()
{
    showScreen(getPh(), getPPM());
    //    showScreen(getPh(), getEC());
    // enableECCalibration();
}

void initializeOLEDDisplay()
{
    _display.begin();
    _display.setPowerSave(0);
    _display.setFontMode(1);
    _display.setFontDirection(0);
    _display.setDrawColor(2);
    _display.enableUTF8Print();
}

void showScreen(float ph, float ec)
{
    _display.firstPage();
    do
    {
        _display.setFont(u8g2_font_ncenB14_tf);
        _display.setCursor(0, 30);
        _display.print(F("ph"));

        _display.setCursor(25, 30);
        _display.setFont(u8g2_font_ncenB24_tf);
        _display.print(ph);

        _display.setFont(u8g2_font_ncenB14_tf);
        _display.setCursor(0, 60);
        _display.print(F("ec"));

        _display.setCursor(25, 60);
        _display.setFont(u8g2_font_ncenB24_tf);
        _display.print(ec);
        _display.setFont(u8g2_font_ncenB08_tf);
        _display.print(F("ms/cm"));
    } while (_display.nextPage());
}

void showScreen(float ph, short ppm)
{
    _display.firstPage();
    do
    {
        _display.setFont(u8g2_font_ncenB14_tf);
        _display.setCursor(0, 30);
        _display.print(F("ph"));

        _display.setCursor(25, 30);
        _display.setFont(u8g2_font_ncenB24_tf);
        _display.print(ph);

        _display.setFont(u8g2_font_ncenB14_tf);
        _display.setCursor(0, 60);
        _display.print(F("ppm"));

        _display.setCursor(50, 60);
        _display.setFont(u8g2_font_ncenB24_tf);
        _display.print(ppm);
    } while (_display.nextPage());
}

float getPh()
{
    int pHSensorPin = 35;
    int samplesPhMeasures = 20;
    float espADCResolution = 4096.0;
    float maximumVoltage = 3.3;
    float mediumVoltage = maximumVoltage / 2;

    int measurings = 0;

    for (int i = 0; i < samplesPhMeasures; i++)
    {
        measurings += analogRead(pHSensorPin);
        delay(10);
    }

    float voltage = maximumVoltage / espADCResolution * measurings / samplesPhMeasures;
    return 7 + ((mediumVoltage - voltage) / 0.18);
}

float getEC()
{
    float voltage = ads.readADC_SingleEnded(0) / 10;
    float ecValue = ec.readEC(voltage, getTemperature()); // convert voltage to EC with temperature compensation

    return ecValue;
}

float enableECCalibration()
{
    float voltage, ecValue;

    static unsigned long timepoint = millis();
    if (millis() - timepoint > 1000U) //time interval: 1s
    {

        timepoint = millis();
        voltage = ads.readADC_SingleEnded(0) / 10;
        Serial.print("voltage:");
        Serial.println(voltage, 4);

        Serial.print("temperature:");
        Serial.print(getTemperature(), 1);
        Serial.println("^C");

        ecValue = ec.readEC(voltage, getTemperature()); // convert voltage to EC with temperature compensation
        Serial.print("EC:");
        Serial.print(ecValue, 4);
        Serial.println("ms/cm");

        Serial.println();
    }
    ec.calibration(voltage, getTemperature()); // calibration process by Serail CMD
}

short getPPM()
{
    // byte TdsSensorPin = 34;
    // pinMode(TdsSensorPin, INPUT);
    float VREF = 3.3; // analog reference voltage(Volt) of the ADC
    byte SCOUNT = 30; // sum of sample point
    short ADCResolution = 4096;

    int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC

    for (byte i = 0; i < SCOUNT; i++)
    {
        // analogBuffer[i] = analogRead(TdsSensorPin);
        analogBuffer[i] = ads.readADC_SingleEnded(0) / 10;
        delay(10);
    }

    float averageVoltage = getMedianNum(analogBuffer, SCOUNT) * VREF / (float)ADCResolution; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (getTemperature() - 25.0);                       //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVoltage = averageVoltage / compensationCoefficient;                    //temperature compensation

    //convert voltage value to tds value
    float tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage) * 0.5;

    return tdsValue;
}

int getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];

    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }

    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;

    return bTemp;
}

float getTemperature()
{
    return 25;
}