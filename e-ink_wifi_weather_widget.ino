#include "global.h"

#include <GxEPD.h>
#include <GxGDEW027C44/GxGDEW027C44.h> // 2.7" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include <U8g2_for_Adafruit_GFX.h>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "plot.h"
#include "label.h"

static const char *ssid = "XXX";
static const char *pass = "XXX";

static const String server = "http://api.openweathermap.org";

static const String nameOfCity = "Wroclaw,PL";

static const String apiKey = "XXX"; 

GxIO_Class io(SPI, 15, 4, 2);  //CS, DC, RST
GxEPD_Class display(io, 2, 5); //RST, BUSY

unsigned long lastConnectionTime = 10 * 60 * 1000;        // last time you connected to the server, in milliseconds
static const unsigned long postInterval = 10 * 60 * 1000; // posting interval of 10 minutes  (10L * 1000L; 10 seconds delay for testing)

#define FONT_11 u8g2_font_helvB10_te //u8g2_font_t0_17b_te
#define FONT_23 u8g2_font_helvB24_te //u8g2_font_maniac_te
#define FONT_TEST u8g2_font_helvB12_te

U8G2_FOR_ADAFRUIT_GFX u8g2;

// print Wifi status
void printWiFiStatus(bool onlcd = false)
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    if (onlcd)
    {
        u8g2.print(F("Sieć: "));
        u8g2.println(WiFi.SSID());
    }

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    if (onlcd)
    {
        u8g2.print(F("Adres IP: "));
        u8g2.println(ip);
    }

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    if (onlcd)
    {
        u8g2.print(F("Poziom sygnału (RSSI): "));
        u8g2.print(rssi);
        u8g2.println(F("dBm"));
    }
}

void connect_to_WiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("connecting");
    while (WiFi.status() != WL_CONNECTED)
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

    display.fillScreen(GxEPD_WHITE);

    u8g2.setFontMode(1);
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);
    //u8g2.setFontMode(1);

    //u8g2.setFont(u8g2_font_ncenB18_te); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.setFont(FONT_23);
    //u8g2.setForegroundColor(GxEPD_RED);
    // u8g2.setCursor(4, 7 + 23);
    // u8g2.print(F("Widget pogodowy"));

    // u8g2.setCursor(4, 7+23);
    // u8g2.print(F("Widget pogodowy"));

    u8g2.setCursor(2, 5 + 23);
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.print(F("Pogodny widget"));

    u8g2.setCursor(5, 7 + 23 + 25);
    u8g2.setFont(FONT_11);
    u8g2.println(F("Zrobione przez Pawła"));

    draw(5 + 0, 110, icon_thunder);
    draw(5 + 64, 110, icon_clouds);
    draw(5 + 128, 110, icon_smallrain);
    draw(5 + 192, 110, icon_sun);

    u8g2.setCursor(35, 90);
    u8g2.print(F("Trwa łączenie przez WiFi..."));

    display.update();

    connect_to_WiFi();
    delay(5000);
    display.fillScreen(GxEPD_WHITE);
    u8g2.setCursor(5, 5);
    u8g2.println();
    u8g2.println(F("Połączono z siecią"));
    u8g2.println();
    printWiFiStatus(true);
    u8g2.println();
    u8g2.println(F("Pobieranie danych..."));
    //display.update();

    //text.reserve(JSON_BUFF_DIMENSION);

    Serial.println("End Setup ==========");

    lastConnectionTime = millis();
    //makehttpRequest();
    HTTPreq();
    updateDisplay();
}

void loop()
{
    if (millis() - lastConnectionTime > postInterval)
    {
        lastConnectionTime = millis();
        HTTPreq();
        updateDisplay();
    }
}

void HTTPreq()
{
    WiFiClient client;
    HTTPClient http;
    String addr = server + "/data/2.5/forecast?q=" + nameOfCity + "&APPID=" + apiKey + "&mode=json&units=metric&cnt=8&lang=pl";
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
    static const String dow_lut[] = {"Niedziela", "Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota"};
    return dow_lut[num];
}

typedef struct
{
    String Temperature;
    String Pressure;
    String WindSpeed;
    String humidity;
    String WeatherDescription;
    String Date;
    String DayOfWeek;
    const tImage *Icon; //pointer to const tImage
} tDisplayData;

tDisplayData DisplayData;

//to parse json data recieved from OWM
void parseJson(String *jsonString)
{
    DynamicJsonDocument jsonDoc(10000);

    // FIND FIELDS IN JSON TREE
    DeserializationError error = deserializeJson(jsonDoc, jsonString->c_str());
    if (error)
    {
        Serial.println("parseObject() failed, error: " + String(error.c_str()));
        return;
    }

    auto list = jsonDoc["list"];

    DisplayData.Icon = getIcon(list[1]["weather"][0]["icon"]);
    DisplayData.Temperature = limit_past_dot(list[1]["main"]["temp"]) + "°C";
    DisplayData.Pressure = limit_past_dot(list[1]["main"]["pressure"]) + "hPa";
    DisplayData.WeatherDescription = (const char *)list[1]["weather"][0]["description"];
    DisplayData.WindSpeed = limit_past_dot(list[1]["wind"]["speed"], 0) + "km/h";
    DisplayData.Date = limit_past_dot(list[0]["dt_txt"], 0, " ");
    DisplayData.DayOfWeek = getDoWstr(getDoW((unsigned long)list[0]["dt"]));
}

void displayGraph()
{
    Plot TempPlot(5, 64, 254, 50);
    TempPlot.FitZero = false;
    TempPlot.ShowBorder = TempPlot.ShowXAxis = TempPlot.ShowYAxis = false;
    TempPlot.addPoint(0, 21);
    TempPlot.addPoint(1, 23);
    TempPlot.addPoint(2, 21);
    TempPlot.addPoint(3, 20);
    TempPlot.addPoint(4, 12);
    TempPlot.addPoint(5, 11);
    TempPlot.addPoint(6, 14);
    TempPlot.addPoint(7, 19);
    TempPlot.redraw();
}



Label TempeLabel(64+1, 0, 100, 29);
Label WeekDayLabel(64+1+100+1, 0, 264 - (65+101), 15);
Label DateLabel(65+101, 16, 264 - (65+101), 15);
Label PressureLabel(65,  64-16, (264 - 65) / 2, 15);
Label WindLabel(65 + (264 - 65) / 2, 64-16, (264 - 65) / 2, 15);
Label WeatherDesrLabel(65,  64-16*2, (264 - 65), 15);

void updateDisplay()
{
    display.fillScreen(GxEPD_WHITE);
    draw(0, 0, *DisplayData.Icon);

    // u8g2.setFont(FONT_23);
    // // u8g2.setCursor(65 + 2, 3 + 23 + 2);
    // // u8g2.print(DisplayData.Temperature);
    // //u8g2.setCursor(65+1, 3 + 23+1);
    // // u8g2.setForegroundColor(GxEPD_RED);
    // // u8g2.print(DisplayData.Temperature);
    // u8g2.setCursor(65, 4 + 23);
    // // u8g2.setForegroundColor(GxEPD_BLACK);
    // u8g2.print(DisplayData.Temperature);

    TempeLabel.setTextPos(jLeftCenter).setFont(FONT_23).redraw(DisplayData.Temperature);
    WeekDayLabel.setTextPos(jCenter).setFont(FONT_11).redraw(DisplayData.DayOfWeek);
    DateLabel.setTextPos(jCenter).setFont(FONT_11).redraw(DisplayData.Date);
    PressureLabel.setTextPos(jCenter).setFont(FONT_11).redraw(DisplayData.Pressure);
    WindLabel.setTextPos(jCenter).setFont(FONT_11).redraw(DisplayData.WindSpeed);
    WeatherDesrLabel.setTextPos(jCenter).setFont(FONT_TEST).redraw(DisplayData.WeatherDescription);

    // u8g2.setFont(FONT_11);
    // u8g2.setCursor(185, 3 + 11);
    // u8g2.print(DisplayData.DayOfWeek);
    // u8g2.setCursor(180, 5 + 11 + 11);
    // u8g2.print(DisplayData.Date);
    // u8g2.setCursor(65, 5 + 23 + 14);
    // u8g2.print(DisplayData.Pressure + "   " + DisplayData.WindSpeed);
    // u8g2.setCursor(65, 5 + 23 + 14 * 2);
    // u8g2.print(DisplayData.WeatherDescription);
    displayGraph();
    display.update();
}

// Label TempeLabel(64+1, 0, 100, 25);
// Label WeekDayLabel(64+1+100+1, 0, 264 - (65+101), 13);
// Label DateLabel(65+101, 14, 264 - (65+101), 13);
// Label PressureLabel(65, 64-15*2, (264 - 65) / 2, 13);
// Label WindLabel(65 + (264 - 65) / 2, 5 + 23 + 14, (264 - 65) / 2, 13);
// Label WeatherDesrLabel(65, 64-15, (264 - 65), 13);

// Label TempeLabel(64+1, 0, 100, 28);
// Label WeekDayLabel(64+1+100+1, 0, 264 - (65+101), 15);
// Label DateLabel(65+101, 16, 264 - (65+101), 15);
// Label PressureLabel(65, 64-16*2, (264 - 65) / 2, 15);
// Label WindLabel(65 + (264 - 65) / 2, 64-16*2 , (264 - 65) / 2, 15);
// Label WeatherDesrLabel(65, 64-16, (264 - 65), 15);
