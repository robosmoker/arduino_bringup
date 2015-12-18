#include <EventManager.h>
#include <Wire.h>
#include <stdint.h>

#define SLAVE_ADDRESS 0x45

EventManager eventManager;
boolean pin13State;
unsigned long lastToggled;


//current status: working
// get with i2cget 1 0x45 <register> c
// set with i2cset 1 0x45 <rigister> <value>
union RegisterFile {
    struct {
        volatile uint8_t FanSetSpeed;
        volatile uint8_t CurrentFanSpeed;
        volatile uint16_t adc0;
        volatile uint16_t adc1;
        volatile uint16_t adc2;
        volatile uint16_t adc3;
    } regs;
    uint8_t bytes[sizeof(regs)];
};

struct Registers
{
    RegisterFile regFile;
    volatile uint8_t address;

    static inline bool inBounds(uint8_t addr)
    {
        return (addr < sizeof(RegisterFile::regs));
    }
    
    void write(uint8_t addr, uint8_t data)
    {
        if (addr == 0) {
            regFile.bytes[addr] = data;
        }
    }

    uint8_t read()
    {
        if (inBounds(address)) {
            return regFile.bytes[address];
        }
        return 0;
    }

    uint8_t read(uint8_t addr)
    {
        if (inBounds(addr)) {
            return regFile.bytes[addr];
        }
        return 0;
    }
    
} registers;

static void i2cReceiveCallback(int count)
{
    int i = 0;
    uint8_t address = 0;
    uint8_t data = 0;
    
    while (Wire.available() > 0) {
        uint8_t d = Wire.read();
        
        if (i == 0) {
            address = d;
        } else if (i == 1) {
            data = d;
        }
        i++;
    }
    if (i == 1) {
        //This is a read and the byte is the address.
        
        //calling with the c mode seems to cause a request to get issued.
        registers.address = address;
    }
    if (i >= 2) {
        //This is a write
        registers.write(address, data);
    }
}

static void i2cRequestCallback(void)
{
    Wire.write(registers.read());
}

static void blinkListener(int, int eventParam)
{
    pin13State = pin13State ? false : true;
    digitalWrite(13, pin13State ? HIGH : LOW);
    lastToggled = millis();
}

static void ADCPoller(int, int)
{
    registers.regFile.regs.adc0 = analogRead(A0);
    registers.regFile.regs.adc1 = analogRead(A1);
    registers.regFile.regs.adc2 = analogRead(A2);
    registers.regFile.regs.adc3 = analogRead(A3);
}

void setup()
{
    Serial.begin(9600);

    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(i2cReceiveCallback);
    Wire.onRequest(i2cRequestCallback);

    pinMode(13, OUTPUT);
    digitalWrite( 13, HIGH );
    pin13State = true;
    lastToggled = millis();
    eventManager.addListener(EventManager::kEventUser0, blinkListener);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    eventManager.addListener(EventManager::kEventAnalog0, ADCPoller);

    Serial.println(F("We're up and running!"));
}

void loop()
{
    if ((millis() - lastToggled) > 1000) {
        eventManager.queueEvent(EventManager::kEventUser0, 0);
        eventManager.queueEvent(EventManager::kEventAnalog0, 0);
    }
    eventManager.processEvent();
}


/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * mode: c++
 * c-basic-offset: 4
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=4 tabstop=8 expandtab:
 * :indentSize=4:tabSize=8:noTabs=true:
 */
