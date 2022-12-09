
#include <string.h>

//#define RELEASE
#ifndef RELEASE
#define DEBUG
#endif

#define MESH_PREFIX "LOCAL_MESH"
#define MESH_PASSWORD "LOCALMESHMOTHERFUCKER"
#define MESH_PORT 2137

//variable definitions
#define CMD_ENTER_CONFIG "CEDIT"
#define USB_BAUDRATE 115200
#define FOO_OK 1
#define FOO_ERROR 0
#define EZ_COUNT 6
#define REL_COUNT 1

//pins definitions
#define PIN_EZ1 5
#define PIN_EZ2 16
#define PIN_EZ3 14
#define PIN_EZ4 12
#define PIN_EZ5 13
#define PIN_EZ6 15
#define PIN_REL 4
#define PIN_LED 2

//typedef
struct ez {

} typedef EZ;

struct rel {

} typedef REL;

  //file scope variables
  static const uint8_t EZ_Pins[] = { PIN_EZ1, PIN_EZ2, PIN_EZ3, PIN_EZ4, PIN_EZ5, PIN_EZ6 };

  //function prototypes
  uint8_t doAction(uint8_t, uint8_t);
uint8_t activateEZ(uint8_t);
uint8_t deactivateEZ(uint8_t);
void activateREL(void);
void deactivateREL(void);
void turnOnboardLedOn(void);
void turnOnboardLedOff(void);
void initalizeGPIO(void);
void checkForUserInput(void);
void checkForPilotMessage(void);

void setup() {
  initalizeGPIO();
  Serial.begin(USB_BAUDRATE);
#ifdef DEBUG
  Serial.println("I`m alive");
  Serial.println("I`m alive");
  Serial.println("I`m alive\n");
#endif
}

void loop() {
  checkForUserInput();
  checkForPilotMessage();
}

uint8_t doAction(uint8_t action_type, uint8_t ez_output_number) {
  uint8_t ret = FOO_OK;
  switch (action_type) {
    case 1:
      activateREL();
      if (activateEZ(ez_output_number) != FOO_OK) ret = FOO_ERROR;
      break;
    case 2:
      if (activateEZ(ez_output_number) != FOO_OK) ret = FOO_ERROR;
      if (activateEZ(6) != FOO_OK) ret = FOO_ERROR;
      break;
    default:
      ret = FOO_ERROR;
      break;
  }
#ifdef DEBUG
  Serial.print("Action:");
  Serial.print(action_type);
  Serial.print(", ez_output_number:");
  Serial.println(ez_output_number);
  Serial.print("Function return: ");
  if (ret == FOO_OK) Serial.println("OK\n");
  else Serial.println("ERROR\n");
#endif
  return ret;
}

void initalizeGPIO(void) {
  pinMode(PIN_REL, OUTPUT);
  deactivateREL();
  for (uint8_t i = 0; i < EZ_COUNT; i++) {
    pinMode(EZ_Pins[i], OUTPUT);
    digitalWrite(EZ_Pins[i], LOW);
  }
  pinMode(PIN_LED, OUTPUT);
  turnOnboardLedOff();
}

uint8_t activateEZ(uint8_t EZ_pin_number_to_activate) {
  uint8_t ret = FOO_OK;
  if ((EZ_pin_number_to_activate <= EZ_COUNT) && (EZ_pin_number_to_activate != 0)) {
#ifdef DEBUG
    Serial.print("Activating EZ");
    Serial.print(EZ_pin_number_to_activate);
    Serial.println("\n");
#endif
    digitalWrite(EZ_Pins[EZ_pin_number_to_activate - 1], HIGH);
  } else {
#ifdef DEBUG
    Serial.println("Error activating EZ, function parameter out of allowed scope:");
    Serial.print("EZ_pin_number_to_activate =");
    Serial.print(EZ_pin_number_to_activate);
    Serial.println("\n");
#endif
    ret = FOO_ERROR;
  }
  return ret;
}

uint8_t deactivateEZ(uint8_t EZ_pin_number_to_deactivate) {
  uint8_t ret = FOO_OK;
  if ((EZ_pin_number_to_deactivate <= EZ_COUNT) && (EZ_pin_number_to_deactivate != 0)) {
#ifdef DEBUG
    Serial.print("Deactivating EZ");
    Serial.print(EZ_pin_number_to_deactivate);
    Serial.println("\n");
#endif
    digitalWrite(EZ_Pins[EZ_pin_number_to_deactivate - 1], LOW);
  } else {
#ifdef DEBUG
    Serial.println("Error activating EZ, function parameter out of allowed scope:");
    Serial.print("EZ_pin_number_to_deactivate =");
    Serial.print(EZ_pin_number_to_deactivate);
    Serial.println("\n");
#endif
    ret = FOO_ERROR;
  }
  return ret;
}

void activateREL(void) {
#ifdef DEBUG
  Serial.println("Activating relay\n");
#endif
  digitalWrite(PIN_REL, HIGH);
  turnOnboardLedOn();
}

void deactivateREL(void) {
#ifdef DEBUG
  Serial.println("Deactivating relay\n");
#endif
  digitalWrite(PIN_REL, LOW);
  turnOnboardLedOff();
}

void turnOnboardLedOn(void) {
#ifdef DEBUG
  Serial.println("Turning on onboard led\n");
#endif
  digitalWrite(PIN_LED, HIGH);
}

void turnOnboardLedOff(void) {
#ifdef DEBUG
  Serial.println("Turning off onboard led\n");
#endif
  digitalWrite(PIN_LED, LOW);
}

void checkForUserInput(void) {
  String cmd_cmp;
  String someText;
  if (Serial.available()) {
    someText = Serial.readString();
    Serial.print(">");
    Serial.print(someText);
    someText.toUpperCase();
    cmd_cmp = CMD_ENTER_CONFIG;
    if ((someText == cmd_cmp) || (someText == (cmd_cmp + "\n"))) {
      uint8_t menu_flag = 1;
      while (menu_flag) {
        Serial.println("------------------DEVICE CONFIG-----------------");
        Serial.println("1. Konfiugracja elektrozaworów");
        Serial.println("2. Konfiguracja czasu załączenia przekaźnika");
        Serial.println("3. Kontrola manualna");
        Serial.println("4. Wyjście z menu konfiguracyjnego");
        uint8_t iterator = 1;
        while (Serial.available() < 1);
        someText = Serial.readString();
        Serial.print(">");
        Serial.print(someText);
        char choice = someText.charAt(0);
        switch (choice) {
          case '1':
            Serial.println("------------------1. ELEKTROZAWORY-----------------");
            break;
          case '2':
            Serial.println("------------------2. PRZEKAŹNIK-----------------");
            Serial.println("Aktualny czas załączenia: ");
            //Serial.print()
            break;
          case '3':
            Serial.println("------------------3. TRYB MANUALNY-----------------");
            for (uint8_t i = 0; i < EZ_COUNT; i++) {
              Serial.print(String(iterator) + ". ");
              Serial.print("Elektrozawór numer: " + String(i + 1) + " | Stan: ");
              if (digitalRead(EZ_Pins[i]) == 1)
                Serial.println("ON");
              else
                Serial.println("OFF");
              iterator++;
            }

            Serial.print(String(iterator) + ". ");
            Serial.print("Przekaźnik | Stan: ");
            if (digitalRead(PIN_REL) == 1)
              Serial.println("ON");
            else Serial.println("OFF");
            iterator++;

            Serial.print(String(iterator) + ". ");
            Serial.print("Dioda LED | Stan: ");
            if (digitalRead(PIN_LED) == 1)
              Serial.println("ON");
            else Serial.println("OFF");
            iterator++;
            Serial.println("----------------------------------------------------------");
            Serial.println("Komenda \"EXIT\" - wyjscie do poprzedniego menu");
            Serial.println("----------------------------------------------------------");
            Serial.println("Aby zmienić stan danego wyjscia należy wpisać numer poprzedzajacy wyjście i stan np.");
            Serial.println("Przykladowe wyjście: x. Elektrozawór | x - liczba");
            Serial.println("Komenda do zmiany stanu wyjścia: \"x ON\" lub \"x OFF\"");
            Serial.println("Po wpisaniu poprawnej komendy nastąpi wykonanie akcji powiazanej z nią powiązanej i powrot do poprzedniego menu");
            Serial.println("----------------------------------------------------------");

            while (Serial.available() < 1)
              ;
            someText = Serial.readString();
            Serial.print(">");
            Serial.print(someText);
            break;
          case '4':
            menu_flag = 0;
            break;
          default:
            break;
        }
      }
    }
  }
}

void checkForPilotMessage(void) {
}

void readConfigFromEEPROM(void) {
}

void writeConfigToEEPROM(void) {
}

//TODO
//dodać timer w menu tak, żeby po przekroczeniu danej wartości wychodził do głównego progrmau
//dodać zmianę czasu timera menu z minimalną wartością i wartością nieskończoną
//dodać możliwośc inwersji stanu logicznego na wyjściu
//dodać możliwość wyłączenia diody LED w przekaźniku
//po wyłączeniu diody LED w ustawieniach przekaźnika ma być niebosługiwana w trybie manualnym
//dodać zapisywanie do eepromu w menu
