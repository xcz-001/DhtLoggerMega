  #include <Arduino.h>
  #include <DHT.h>
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <RTClib.h>
  #include <SPI.h>
  #include <SD.h>
  #include <Bounce2.h>

  #define DHTTYPE DHT22
  #define DHT1_PIN 3
  #define DHT2_PIN 4
  #define DHT3_PIN 5

  const int chipSelect = 53; // CS pin

  // ==================CHANGABLE VARIABLE===================
  int DataLogIntervalMinutes = 10;     // update interval. Change this for testing

  DHT dht1(DHT1_PIN, DHTTYPE);
  DHT dht2(DHT2_PIN, DHTTYPE);
  DHT dht3(DHT3_PIN, DHTTYPE);

  float temperature1,temperature2,temperature3;
  float humidity1,humidity2,humidity3;

  int lastMinute = -1;

  LiquidCrystal_I2C lcd(0x27, 20, 4);

  RTC_DS3231 rtc;

  File dataFile;
  #define LASTLOG_FILE "LASTLOG.TXT"
  char currentLogFile[13] = "DATA000.CSV"; // 8.3 + null

  Bounce button;

  bool tempLineInitiated = false;

  //===== helper functions ======
  String fOrErr(float v, uint8_t dec = 1) {
    if (isnan(v)) return "ERR";
    return String(v, dec);
  }

  //===============DHT Functions=================

  void dhtSetup() {
    dht1.begin();
    Serial.println("DHT 1 ready!");
    dht2.begin();
    Serial.println("DHT 2 ready!");
    dht3.begin();
    Serial.println("DHT 3 ready!");
  }

  void dhtReadAll() {

    temperature1 = dht1.readTemperature();
    humidity1 = dht1.readHumidity();

    temperature2 = dht2.readTemperature();
    humidity2 = dht2.readHumidity();

    temperature3 = dht3.readTemperature();
    humidity3 = dht3.readHumidity();
  }

  //===============LCD Setup=================
  void lcdSetup() {
    lcd.init();
    lcd.backlight();
    lcd.noBacklight();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("   DHT DATA LOGGER   ");
    Serial.println("LCD ready!");
    delay(2000);
    //lcd.clear();
  }

  void lcdTempLayoutInit() {
    if (tempLineInitiated) return;

    lcd.setCursor(0,1);
    lcd.print("T1 T:    ");
    lcd.print((char)223);
    lcd.print("C H:    %");

    lcd.setCursor(0,2);
    lcd.print("T2 T:    ");
    lcd.print((char)223);
    lcd.print("C H:    %");

    lcd.setCursor(0,3);
    lcd.print("T3 T:    ");
    lcd.print((char)223);
    lcd.print("C H:    %");

    tempLineInitiated = true;
  }

  void lcdTempValuesUpdate() {

    lcd.setCursor(5,1);   // T1 temp
    lcd.print(fOrErr(temperature1));

    lcd.setCursor(14,1);  // T1 hum
    lcd.print(fOrErr(humidity1));

    lcd.setCursor(5,2);
    lcd.print(fOrErr(temperature2));

    lcd.setCursor(14,2);
    lcd.print(fOrErr(humidity2));

    lcd.setCursor(5,3);
    lcd.print(fOrErr(temperature3));

    lcd.setCursor(14,3);
    lcd.print(fOrErr(humidity3));
  }


  //===============RTC=================
  void rtcSetup() {
    if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    DateTime now = rtc.now();

    Serial.print("Time: ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    Serial.print("Date: ");
    Serial.print(now.day());
    Serial.print("/");
    Serial.print(now.month());
    Serial.print("/");
    Serial.println(now.year());

  }

  void PrintDateTimeToLcd() {
    DateTime now = rtc.now();

    //date
    lcd.setCursor(0,0);
    if (now.month()<10) lcd.print("0");
    lcd.print(now.month());
    lcd.print("/");
    if (now.day()<10) lcd.print("0");
    lcd.print(now.day());
    lcd.print("/");
    lcd.print(now.year());

    //time
    lcd.setCursor(12,0);
    if (now.hour()<10) lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute()<10) lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second()<10) lcd.print("0");
    lcd.print(now.second());
  }

  //===============SD=================

  bool readLastLogged() {
    File file = SD.open(LASTLOG_FILE);
    if (!file) return false;

    int index = 0;
    while (file.available() && index < 12) {
      char c = file.read();
      if (c == '\n' || c == '\r') break;
      currentLogFile[index++] = c;
    }
    currentLogFile[index] = '\0';

    file.close();
    return true;
  }

  void ensureLastLoggedExists() {
    if (!SD.exists(LASTLOG_FILE)) {
      Serial.println("LASTLOG NOT FOUND. Creating LASTLOG.TXT");
      File file = SD.open(LASTLOG_FILE, FILE_WRITE);
      if (file) {
        file.println("DATA000.CSV");
        file.close();
        strcpy(currentLogFile, "DATA000.CSV");
      }
    } else {
      Serial.println("LASTLOG FOUND. Reading last logged file.");
      readLastLogged();
    }
  }

  void sdSetup() {
    // pinMode(10, OUTPUT);  // must be OUTPUT before SD.begin()
    if (!SD.begin(chipSelect)) {
      Serial.println("SD initialization failed!");
      while (1); // STOP if SD fails (important)
    }

    Serial.println("SD ready.");
    delay(600); //wait for SD to stabilize
    ensureLastLoggedExists();
  }


  void updateLastLogged(const char* filename) {
    SD.remove(LASTLOG_FILE);              // delete old
    File file = SD.open(LASTLOG_FILE, FILE_WRITE);
    if (file) {
      file.println(filename);
      file.close();
      strcpy(currentLogFile, filename);
    }
  }


  bool createFile(const char* filename) {
    dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile) return false;

    dataFile.println("Timestamp,DHT1_Temp,DHT1_Hum,DHT2_Temp,DHT2_Hum,DHT3_Temp,DHT3_Hum");
    dataFile.close();

    Serial.print("Created: ");
    Serial.println(filename);
    return true;
  }

  String buildDataLine() {

    DateTime now = rtc.now(); // get current time

    // Build timestamp: YYYY-MM-DD HH:MM:SS
    String timestamp = String(now.year()) + "-" +
                      (now.month() < 10 ? "0" : "") + String(now.month()) + "-" +
                      (now.day() < 10 ? "0" : "") + String(now.day()) + " " +
                      (now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" +
                      (now.minute() < 10 ? "0" : "") + String(now.minute()) + ":" +
                      (now.second() < 10 ? "0" : "") + String(now.second());

    // Build CSV line
    String line = timestamp + "," +
                fOrErr(temperature1) + "," + fOrErr(humidity1) + "," +
                fOrErr(temperature2) + "," + fOrErr(humidity2) + "," +
                fOrErr(temperature3) + "," + fOrErr(humidity3);
    return line;
  }

  bool writeToFile(const char* filename, const String &data) {
    Serial.print("Writing to file: ");
    Serial.println(filename);
    Serial.print("Data: ");
    Serial.println(data);
    dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile) return false;

    dataFile.println(data);
    dataFile.close();
    return true;
  }

  void createAnotherFile(){ //for the button at d7
    Serial.println("Creating new data file...");
    int num = atoi(&currentLogFile[4]); // extract number from DATA###.CSV
    num++;

    char newFile[13];
    sprintf(newFile, "DATA%03d.CSV", num);

    if (createFile(newFile)) {
      updateLastLogged(newFile);
    }
  }

  void SaveDataToSdCard() {
    writeToFile(currentLogFile, buildDataLine());
  }

  // Erase all files on SD
  // -----------------------
  // bool eraseSDCard() {
  //   File root = SD.open("/");
  //   if (!root) {
  //     Serial.println("Failed to open root directory");
  //     return false;
  //   }

  //   File entry = root.openNextFile();
  //   while (entry) {
  //     String name = entry.name();
  //     entry.close();              // must close before delete

  //     if (!SD.remove(name)) {
  //       Serial.print("Failed to delete: ");
  //       Serial.println(name);
  //     } else {
  //       Serial.print("Deleted: ");
  //       Serial.println(name);
  //     }

  //     entry = root.openNextFile();
  //   }

  //   root.close();
  //   Serial.println("SD card erased.");
  //   return true;
  // }

  //================Button=================
  void buttonSetup() {
    pinMode(2, INPUT);          // external pulldown
    button.attach(2);
    button.interval(150); //debounce interval
  }

  void buttonController(){

    button.update();

    if (button.fell()) {
      createAnotherFile();
    }
  }
  void lcdPrintController() {
    static unsigned long lastRun = 0;
    unsigned long now = millis();

    if (now - lastRun < 1000) return; // 1s rate limit
    lastRun = now;

    PrintDateTimeToLcd();
    lcdTempValuesUpdate();
  }
  void dataLoggingController(const DateTime& now) {
    static int lastMinute = -1;

    if (now.minute() % DataLogIntervalMinutes == 0 &&
        lastMinute != now.minute()) {

      lastMinute = now.minute();
      SaveDataToSdCard();
    }
  }

  void sensorReadController(){
    static unsigned long lastRun = 0;
    unsigned long now = millis();

    if (now - lastRun < 2000) return; // 1s rate limit
    lastRun = now;

    dhtReadAll();
  }

  //===============Arduino Setup & Loop=================

  void setup() {
    Serial.begin(9600);
    Serial.println("DHT Data Logger Starting...");
    dhtSetup();
    lcdSetup();
    rtcSetup();
    sdSetup();
    buttonSetup();
    dhtReadAll();
    lcdTempLayoutInit();
    //eraseSDCard();
  }

  void loop() {

    DateTime now = rtc.now();

    sensorReadController();
    buttonController();
    lcdPrintController();

    dataLoggingController(now);
  }
