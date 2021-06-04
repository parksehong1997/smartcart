#include <usbhid.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

#define ITEM1  "8801116007172"
#define ITEM2 "5060214370172"
#define ITEM3 "88021331"
#define ITEM4 "8809180747307"
#define ITEM5 "8801402610239"

uint8_t index = 0;
char scannedValue[16] = {0,};

class MyParser : public HIDReportParser {
  public:
    MyParser();
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
  protected:
    uint8_t KeyToAscii(bool upper, uint8_t mod, uint8_t key);
    virtual void OnKeyScanned(bool upper, uint8_t mod, uint8_t key);
    virtual void OnScanFinished();
};

MyParser::MyParser() {}

void MyParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  // If error or empty, return
  if (buf[2] == 1 || buf[2] == 0) return;

  for (uint8_t i = 7; i >= 2; i--) {
    // If empty, skip
    if (buf[i] == 0) continue;

    // If enter signal emitted, scan finished
    if (buf[i] == UHS_HID_BOOT_KEY_ENTER) {
      OnScanFinished();
    }

    // If not, continue normally
    else {
      // If bit position not in 2, it's uppercase words
      OnKeyScanned(i > 2, buf, buf[i]);
    }

    return;
  }
}

uint8_t MyParser::KeyToAscii(bool upper, uint8_t mod, uint8_t key) {
  // Letters
  if (VALUE_WITHIN(key, 0x04, 0x1d)) {
    if (upper) return (key - 4 + 'A');
    else return (key - 4 + 'a');
  }

  // Numbers
  else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
    return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
  }

  return 0;
}

void MyParser::OnKeyScanned(bool upper, uint8_t mod, uint8_t key) {
  uint8_t ascii = KeyToAscii(upper, mod, key);
  Serial.print((char)ascii);

  scannedValue[index++] = (char)ascii;
}

void MyParser::OnScanFinished() {
  
  if (strcmp(scannedValue, ITEM1) == 0) {
  Serial.print(" 담배 ");-
  Serial.println("5000");
  } 
  
  else if (strcmp(scannedValue, ITEM2) == 0) {
    Serial.print(ITEM2);
  Serial.print(" 라면 ");
  Serial.println("1100");
  }


else if (strcmp(scannedValue, ITEM3) == 0)
  {
 
  Serial.print(" 과자 ");
  Serial.println("1300");
    
    }

    else if (strcmp(scannedValue, ITEM4) == 0)
  {
 
  Serial.print(" 물티슈 ");
  Serial.println("1000");
    
    }

        else if (strcmp(scannedValue, ITEM5) == 0)
  {
 
  Serial.print(" LAN ");
  Serial.println("9500");
    
    }

  
  for (int i = 0; i < 16; i++) {
  scannedValue[i] = 0;
  }
  
  index = 0;
}

USB          Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);
MyParser     Parser;

void setup() {
  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }

  delay( 200 );

  Hid.SetReportParser(0, &Parser);

  // For debugging
  /*
  for (int i = 0; i < 13; i++) {
    scannedValue[i] = ITEM1[i];
  }
  if (strcmp(scannedValue, ITEM1) == 0) {
    Serial.print(ITEM1);
    Serial.print(" 담배 ");
    Serial.println("5000");
  }
  */
}

void loop() {
  Usb.Task();
}
