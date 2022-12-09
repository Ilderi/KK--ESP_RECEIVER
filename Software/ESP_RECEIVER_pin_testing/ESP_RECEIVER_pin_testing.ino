
//#define RELEASE 1

#ifndef RELEASE
#define DEBUG
#endif

#define MESH_PREFIX "LOCAL_MESH"
#define MESH_PASSWORD "LOCALMESHMOTHERFUCKER"
#define MESH_PORT 2137

//variable definitions
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

//file scope variables
static const uint8_t EZ_Pins[] = { PIN_EZ1, PIN_EZ2, PIN_EZ3, PIN_EZ4, PIN_EZ5, PIN_EZ6 };

//function prototypes
uint8_t activateEZ(uint8_t);
uint8_t deactivateEZ(uint8_t);
void activateREL(void);
void deactivateREL(void);
void turnOnboardLedOn(void);
void turnOnboardLedOff(void);
void initalizeGPIO(void);

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
  activateEZ(1);
  delay(250);
  activateEZ(2);
  delay(250);
  activateEZ(3);
  delay(250);
  activateEZ(4);
  delay(250);
  activateEZ(5);
  delay(250);
  activateEZ(6);
  delay(250);
  activateREL();
  delay(250);

  deactivateEZ(1);
  delay(250);
  deactivateEZ(2);
  delay(250);
  deactivateEZ(3);
  delay(250);
  deactivateEZ(4);
  delay(250);
  deactivateEZ(5);
  delay(250);
  deactivateEZ(6);
  delay(250);
  deactivateREL();
  delay(250);
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