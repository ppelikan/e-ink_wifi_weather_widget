#include "global.h"

#include <GxEPD.h>
#include <GxGDEW027C44/GxGDEW027C44.h> // 2.7" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include <U8g2_for_Adafruit_GFX.h>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//#include <time.h> //TODO

#include "plot.h"
#include "label.h"

// ******** YOUR SETUP ********

// WiFi network name and password
static const char *ssid = "XXX";
static const char *pass = "XXX";

// OpenWeatherMap API server name
static const String server = "http://api.openweathermap.org";

// replace the next line to match your city and 2 letter country code
static const String nameOfCity = "Wroclaw,PL";

// replace the next line with your API Key
static const String apiKey = "XXX";

//E-Ink display connection lines (default is for ESP8266 E-paper Driver Board)
#define EINK_CS        15
#define EINK_DC        4
#define EINK_RST       2
#define EINK_BUSY      5
#define ONEWIRE_SENSOR 2 //GPIO2 = port D2 on ESP8266 E-Paper PCB

// ******** ******** ********


#define FONT_8 u8g2_font_helvB08_te
#define FONT_11 u8g2_font_helvB10_te // select u8g2 fonts from here: https://github.com/olikraus/u8g2/wiki/fntlistall
#define FONT_23 u8g2_font_helvB24_te

bool isOneWirePresent = false;

OneWire oneWire(ONEWIRE_SENSOR); 
DallasTemperature sensors(&oneWire);

GxIO_Class io(SPI, EINK_CS, EINK_DC, EINK_RST);  //CS, DC, RST
GxEPD_Class display(io, EINK_RST, EINK_BUSY); //RST, BUSY
U8G2_FOR_ADAFRUIT_GFX u8g2;

void printWiFiStatus(bool onlcd = false)
{
    // print the SSID of the network:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void connect_to_WiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("connecting");
    while (WiFi.status() != WL_CONNECTED)  //TODO: add timeout and error displayed on eink
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi Connected");
}

void draw(int16_t x, int16_t y, tImage img, int16_t color = GxEPD_BLACK)
{
    display.drawBitmap(x, y, img.data, img.width, img.height, color);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup ==========");

    display.setRotation(1);
    display.init(115200);
    u8g2.begin(display); // connect u8g2 procedures to Adafruit GFX

    u8g2.setFontMode(1);
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);

    // Start the DS18B20 sensor
    sensors.begin();
    if (sensors.getDeviceCount()) //check whether sensor is connected
        isOneWirePresent = true;

    connect_to_WiFi();

    Serial.println("End Setup ==========");

    HTTPreq();
    updateDisplay();
    oneWire.depower();
    display.powerDown();
    ESP.deepSleep(15 * 60e6); //wake up every 15 minutes (requires an GPIO16 to be shorted to RST pin)
}

void loop()
{
}

void HTTPreq()
{
    WiFiClient client;
    HTTPClient http;
    String addr = server + "/data/2.5/forecast?q=" + nameOfCity + "&APPID=" + apiKey + "&mode=json&units=metric&cnt=8&lang="+TR_CCODE;
    Serial.println(addr);
    http.begin(addr);
    int httpCode = http.GET();
    if (httpCode <= 0)
    {
        Serial.println("http error, httpCode: " + String(httpCode) + ", " + http.errorToString(httpCode));
    }
    String payload = http.getString();
    http.end();
    Serial.println("got data! =================================");
    Serial.println(payload);
    Serial.println("end got data! =============================");
    parseJson(&payload);
}

const tImage *getIcon(String id)
{
    static const String ico_luts[] = {"01d", "02d", "03d", "04d", "09d", "10d", "11d", "13d", "50d",
                                      "01n", "02n", "03n", "04n", "09n", "10n", "11n", "13n", "50n"};
    static const tImage *ico_lut[] = {&icon_sun, &icon_suncloud, &icon_cloud, &icon_clouds, &icon_rain, &icon_smallrain, &icon_thunder, &icon_snow, &icon_fogg,
                                      &icon_moonstars, &icon_mooncloud, &icon_cloud, &icon_clouds, &icon_rain, &icon_smallrainmoon, &icon_thunder, &icon_snow, &icon_fogg};
    int i = 0;
    for (i = 0; (i < sizeof(ico_luts)); i++)
        if (id == ico_luts[i])
            return ico_lut[i];
    return nullptr;
}

String limit_past_dot(String str, byte decimal_places = 1, String dotstr = ".")
{
    int dot = str.indexOf(dotstr);
    if (dot)
    {
        if (decimal_places)
            return str.substring(0, dot + 1 + decimal_places);
        else
            return str.substring(0, dot);
    }
    return str;
}

uint8_t getDoW(unsigned long t)
{
    return ((t / 86400) + 4) % 7;
}

String getDoWstr(char num)
{
    static const String dow_lut[] = {TR_DOW_1, TR_DOW_2, TR_DOW_3, TR_DOW_4, TR_DOW_5, TR_DOW_6, TR_DOW_7};
    return dow_lut[num];
}

typedef struct
{
    const tImage *Icon; //pointer to const tImage
} tDisplayData;

tDisplayData DisplayData;

Label TempeLabel(64, 0, 100, 29);
Label Tempe2Label(64 + 101, 30 + 8, 264 - (65 + 101), 15);
Label WeekDayLabel(64 + 1 + 100 + 1, 0, 264 - (65 + 101), 15);
Label DateLabel(65 + 101, 16, 264 - (65 + 101), 15);
Label PressureLabel(65, 30, (264 - 65) / 2, 15);
Label WindLabel(65, 30 + 16, (264 - 65) / 2, 15);
Label WeatherDesrLabel(0, 62, 264, 15);
Plot TempPlot(6, 63 + 16, 250, 50);

Label Wtf(0, 176-25, 264, 25);

//to parse json data recieved from OWM
void parseJson(String *jsonString)
{
    DynamicJsonDocument jsonDoc(10000);

    DeserializationError error = deserializeJson(jsonDoc, jsonString->c_str());
    if (error)
    {
        Serial.println("deserializeJson failed, error: " + String(error.c_str()));
        return;
    }

    char buf[50];

    DisplayData.Icon = getIcon(jsonDoc["list"][0]["weather"][0]["icon"]);
    TempeLabel.setText(limit_past_dot(jsonDoc["list"][0]["main"]["temp"]) + "°C"); //TODO: get rid of all used limit_past_dot(), instead use round() function
    PressureLabel.setText(limit_past_dot(jsonDoc["list"][0]["main"]["pressure"]) + "hPa");
    WeatherDesrLabel.setText(/*(const char *)*/ jsonDoc["list"][0]["weather"][0]["description"]);
    WindLabel.setText(limit_past_dot(jsonDoc["list"][0]["wind"]["speed"], 0) + "km/h");
    DateLabel.setText(limit_past_dot(jsonDoc["list"][0]["dt_txt"], 0, " "));
    WeekDayLabel.setText(getDoWstr(getDoW((unsigned long)jsonDoc["list"][0]["dt"]))); // TODO: take timezones into account
    //timestamp = (unsigned long)jsonDoc["list"][0]["dt"] + jsonDoc["city"]["timezone"]; //TODO: timestamp corrected by timezone (city.timezone - Shift in seconds from UTC)
    if (isOneWirePresent)
    {
        sensors.requestTemperatures();
        float sensorTemp = sensors.getTempCByIndex(0);
        sprintf(buf, "%.1f°C", sensorTemp);
        Tempe2Label.setText(buf);
    }
    else
    {
        Tempe2Label.setText("---");
    }

    TempPlot.clear(0);
    for (int i = 0; i < 8; i++)
    {
        String tempstr = jsonDoc["list"][i]["main"]["temp"];
        String dtstr = limit_past_dot(jsonDoc["list"][i]["dt_txt"], 2, ":").substring(11);
        if (i == 0)
            dtstr = TR_NOW;
        TempPlot.addPoint(i, tempstr.toFloat(), dtstr, String(limit_past_dot(tempstr, 0) + "°C"));
    }
}

void displayGraph()
{
    TempPlot.setAxisLabelsFont(FONT_8);
    TempPlot.FitZero = false;
    TempPlot.ShowBorder = TempPlot.ShowXAxis = false;
    TempPlot.borderY = 5;
    TempPlot.redraw();
}

void updateDisplay()
{
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);
    display.fillScreen(GxEPD_WHITE);
    draw(0, 0, *DisplayData.Icon);

    displayGraph();
    TempeLabel.setTextPos(jLeftCenter).setFont(FONT_23).redraw();
    WeekDayLabel.setTextPos(jCenter).setFont(FONT_11).redraw();
    DateLabel.setTextPos(jCenter).setFont(FONT_11).redraw();
    PressureLabel.setTextPos(jLeftCenter).setFont(FONT_11).redraw();
    WindLabel.setTextPos(jLeftCenter).setFont(FONT_11).redraw();
    WeatherDesrLabel.setTextPos(jLeftCenter).setFont(FONT_11).redraw();
    Tempe2Label.setTextPos(jCenter).setFont(FONT_11).redraw();
    Wtf.fGcolor = GxEPD_WHITE;
    Wtf.bGcolor = GxEPD_RED;
    Wtf.setText("TODO, put something here...");
    Wtf.setTextPos(jCenter).setFont(FONT_11).redraw();

    display.update();
}

//TO DOS: ?
//http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html

