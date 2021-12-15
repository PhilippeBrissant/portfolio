#include <Adafruit_ADS1X15.h>
#include <DFRobot_ESP_EC.h>
#include <U8g2lib.h>

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;
U8G2_SH1106_128X64_NONAME_1_HW_I2C _display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void initializeOLEDDisplay();
void showScreen(float ph, float ec);
float getPh();
float getEC();
float enableECCalibration();

void setup()
{
    initializeOLEDDisplay();
    ec.begin();
    ads.setGain(GAIN_ONE);
    ads.begin();
    // Serial.begin();
}

void loop()
{
    showScreen(getPh(), getEC());
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
    float temperature = 25;
    float voltage = ads.readADC_SingleEnded(0) / 10;
    float ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation

    return ecValue;
}

float enableECCalibration()
{
    float voltage, ecValue, temperature = 25;

    static unsigned long timepoint = millis();
    if (millis() - timepoint > 1000U) //time interval: 1s
    {

        timepoint = millis();
        voltage = ads.readADC_SingleEnded(0) / 10;
        Serial.print("voltage:");
        Serial.println(voltage, 4);

        Serial.print("temperature:");
        Serial.print(temperature, 1);
        Serial.println("^C");

        ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
        Serial.print("EC:");
        Serial.print(ecValue, 4);
        Serial.println("ms/cm");

        Serial.println();
    }
    ec.calibration(voltage, temperature); // calibration process by Serail CMD
}
