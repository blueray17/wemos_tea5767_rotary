#include <TEA5767.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 0  // GPIO0
#define ENCODER_A D6
#define ENCODER_B D5
#define BUTTON D7

Adafruit_SSD1306 display(OLED_RESET);
TEA5767 radio = TEA5767();
long lmillis = 0;
int encoderPos, jmlst,  i = 14 , lastEncoderPos = 1;
short modee = 1;
float stepp;
String judul = "Okeeee";

const char * const st[][2] = {
  "87.70", "HardRock",
  "88.10", "SouthEast",
  "88.50", "MoraFM",
  "88.90", "Auto",
  "89.30", "ElShinta",
  "89.70", "Global",
  "90.10", "Urban Bdg",
  "90.50", "Cakra",
  "90.90", "X-Channel",
  "91.30", "Trijaya", //1
  "91.70", "Voks",
  "92.10", "SuaraIndh",
  "92.50", "Maestro",
  "93.30", "Sonora",
  "93.70", "Paramuda",
  "94.40", "DeltaFM",
  "94.80", "FitFM",
  "95.20", "Bandung",
  "95.60", "B-Radio",
  "96.40", "Bobotoh", //2
  "96.80", "Eight",
  //"97.20","NewShinta",
  "97.60", "RRI Pro1",
  "98.00", "MayaFM",
  "98.40", "Prambors",
  "98.80", "Raka",
  //"99.20","Mom&Kids",
  "100.00", "99ers",
  "100.40", "KLCBS",
  "100.80", "Kandaga", //3
  "101.10", "MGT",
  "101.50", "Dahlia",
  //"101.90","Cosmo",
  "102.30", "RaseFM",
  "102.70", "MQFM",
  "103.10", "OzRadio",
  "103.40", "Gak Tau",
  "103.90", "UNIKOM",
  "104.30", "RodjaFM",
  "104.70", "RamaFM", //4
  "105.10", "I-Radio",
  "105.50", "Garuda",
  "105.90", "Ardan",
  "106.30", "UrbanFM",
  "106.70", "Mara FM",
  "107.10", "K-lite",
  "107.50", "PRFM",
  "108.00", "Arduino"
};

float frek = atof(st[i][0]);

void setup()   {
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  jmlst = sizeof(st) / sizeof(st[0]);
  delay(100);
  radio.setFrequency(atof(st[i][0]));
  delay(100);
  setDisplay();
  stepp = 0.1;
}


void loop() {
  /*if(lmillis <= millis()){
    lmillis = millis() + 2000; //Read current time and add 1 second
    setDisplay();
    }*/
  encoderPos = digitalRead(ENCODER_A);
  //delay(10);
  if (encoderPos != lastEncoderPos) {
    if (digitalRead(ENCODER_B) != encoderPos  && encoderPos == 0) {
      if (modee == 1) {
        i++;
        if (i >= jmlst) {
          i = 0;
        }
        frek = atof(st[i][0]);
      } else {
        frek += stepp;
      }
    } else if (digitalRead(ENCODER_B) == encoderPos  && encoderPos == 0) {
      if (modee == 1) {
        i--;
        if (i <= -1) {
          i = jmlst - 1;
        }
        frek = atof(st[i][0]);
      } else {
        frek -= stepp;
      }
    }    
    radio.setFrequency(frek);
    setDisplay();
  }
  
  lastEncoderPos = encoderPos;

  if (digitalRead(BUTTON) == LOW) {
    modee++;
    if (modee == 1) {

    } else if (modee == 2) {
      stepp = 0.1;
      judul = "Manual 0.1";
    } else if (modee == 3) {
      stepp = 1;
      judul = "Manual 1";
    } else if (modee == 4) {
      stepp = 5;
      judul = "Manual 5";
    } else if (modee == 5) {
      modee = 1;
    }
    setDisplay();
    delay(500);
  }

}

void setDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Radio FM");
  display.setCursor(0, 7);
  display.println("----------");
  display.setCursor(0, 16);
  if (modee == 1) {
    display.print(st[i][1]);
  } else {
    display.print(judul);
  }
  display.setCursor(0, 28);
  display.print("Freq:");
  display.println(frek, 1);
  display.display();
  display.setCursor(0, 39);
  display.print("Signal:");
  display.print(floor(radio.getSignalLevel() * 6.66), 0);
  display.print("%");
  display.display();
}

void printStereo() {
  bool stereo = radio.isStereo(); //Get Stereo
  Serial.print("Stereo: ");
  Serial.println(stereo);
}
