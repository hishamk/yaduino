#include <SoftwareSerial.h>

#include <LiquidCrystal.h>

#include <EncoderButton.h>

#include <WiFiEspAT.h>

#define BIT(n, i)(n >> i & 1)
#define esp8266Speed 9600

SoftwareSerial esp8266(11, 10); // rx, tx

int lastPosition;

WiFiClient client;

const int MAX_CLIENTS = 3;
const int CLIENT_CONN_TIMEOUT = 3600; // seconds. 1 hour
WiFiServerPrint server(2323);

LiquidCrystal lcd(0, 1, 5, 6, 7, 8); // 0 = rxPin, 1 = txPin Parameters: (rs, enable, d4, d5, d6, d7)
EncoderButton eb1(2, 3, 4); // INT0 and INT1 hardware interrupt pins are 2 and 3. We use them so that we can read the encoder using interrupts rather than polling on every cycle which would be laggy/unreliable!

String s;

byte yaduinoLogo[] = {
  B10100,
  B10100,
  B11100,
  B00110,
  B11101,
  B00101,
  B00101,
  B00110
};

byte wifiLogo[] = {
  B10101,
  B10101,
  B11101,
  B11101,
  B00000,
  B11101,
  B11001,
  B10001
};

byte ok[] = {
  B11110,
  B10010,
  B10010,
  B11110,
  B00101,
  B00110,
  B00110,
  B00101
};

byte no[] = {
  B10010,
  B11010,
  B10110,
  B10010,
  B01111,
  B01001,
  B01001,
  B01111
};

byte smiley[] = {
  B11011,
  B11011,
  B00100,
  B00100,
  B10001,
  B10001,
  B01110,
  B00000
};

void setup() {
  // set up the LCD's number of columns and rows:
 
  lcd.begin(16, 2);
  lcd.createChar(0,yaduinoLogo);
  lcd.createChar(1,wifiLogo);

  //Link the event(s) to your function
  eb1.setClickHandler(onEb1Clicked);
  eb1.setEncoderHandler(onEb1Encoder);

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);

  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);

  for (int source = 0; source < 4; source++) {
    delay(300);
    selectSource(source);
  }

  delay(300);
  selectSource(0);

  esp8266.begin(esp8266Speed);
  WiFi.init(esp8266);
  WiFi.hostname("yaduino-yellow");

  printWelcome();
  delay(2000);
  lcdReset();

  if (WiFi.status() == WL_NO_MODULE) {
    lcd.print("WiFi error!");
    // don't continue
    while (true);
  }

  // waiting for connection to Wifi network set with the SetupWiFiConnection sketch
  lcd.write(byte(1));
  lcd.print(" Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.print('.');
  }

  server.begin(MAX_CLIENTS, CLIENT_CONN_TIMEOUT);
  printNetInfo();
}

void lcdReset() {
  lcd.clear();
//  lcd.blink();
  lcd.setCursor(0, 0);
}

void printNetInfo() {
  lcdReset();
  IPAddress ip = WiFi.localIP();
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.print(WiFi.SSID());
  lcd.setCursor(0, 1);
  lcd.print(ip);
}

void printLove() {
  lcdReset();
  lcd.print("I love you.");
  lcd.setCursor(0, 1);
  lcd.print(":)");
}

void printWelcome() {
  lcdReset();
  lcd.print("   Welcome to");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(" Yaduino Mk I");
}

void loop() {
  eb1.update();

  WiFiClient client = server.available(); // returns first client which has data to read or a 'false' client
  if (client) { // client is true only if it is connected and has data to read
    s = client.readStringUntil('\n'); // read the message incoming from one of the clients
    s.trim(); // trim eventual \r
    updateMessage(s);
    client.print("echo: "); // this is only for the sending client
    server.println(s); // send the message to all connected clients
    server.flush(); // flush the buffers
  }
}

void updateMessage(String msg) {
  lcdReset();
  lcd.print(msg);
  if (s.length() > 16) {
    delay(1000);
  }
}

void onEb1Clicked(EncoderButton & eb) {
  tone(9, 100, 10);
  //  lcd.clear();
  lcd.setCursor(0, 0);
  // Print a message to the LCD.
  lcd.print("Button click   ");
  lcd.setCursor(13, 0);
  lcd.print(eb.clickCount());
  lcd.setCursor(0, 1);
  lcd.print("                ");

  if (eb.clickCount() == 2) {
    printNetInfo();
  } else if (eb.clickCount() == 3) {
    printLove();
  }
}


void levelSet(int d) {
  static int level=-1;
  lcd.setCursor(0,0);
  lcd.print("VOLUME          ");
  level += d;

  lcd.setCursor(0,1);
  for (int x = 0; x<= level; x++) {
    lcd.print((char)0xFF);
  }
  int blank = 16 - level;
  if (blank>0) {
    lcd.print(" ");
  }
}

void onEb1Encoder(EncoderButton & eb) {
  if (eb.position() >= 0 && eb.position() <= 16) {
    if (eb.position() >= 0 && eb.position() <= 3) {
      selectSource(eb.position());
    }
    tone(9, 1000, 25);
  if (eb.increment()==1) {
    levelSet(1);
  } else if (eb.increment()==-1) {
    levelSet(-1);
   } 
  
  } else {
  eb.resetPosition(lastPosition);
    tone(9, 20, 150);
  }

  lastPosition = eb.position();
}

void selectSource(int x) {
  int p14 = BIT(x, 0);
  int p15 = BIT(x, 1);
  int p16 = BIT(x, 2);

  digitalWrite(14, p14);
  digitalWrite(15, p15);
  digitalWrite(16, p16);
}

void loadSymbols() {
  lcd.createChar(0, yaduinoLogo);
  lcd.createChar(1, wifiLogo);
  lcd.createChar(2, ok);
  lcd.createChar(3, no);
  lcd.createChar(4, smiley);
}
