#include "mbed.h"

#define Set_data_command 0b00100010
#define Display_ON 0b11110001
#define read_buttons 0b01000010


int seg7[10] = {0b11111100, 0b01100000, 0b11011010,
                0b11110010, 0b01100110, 0b10110110,
                0b10111110, 0b11100000, 0b11111110,
                0b11110110};
int dig[8] = {0b00000011, 0b01000011, 0b00100011, 0b01100011, 0b00010011, 0b01010011, 0b00110011, 0b01110011};
int led[8] = {0b10000011, 0b11000011, 0b10100011, 0b11100011, 0b10010011, 0b11010011, 0b10110011, 0b11110011};


InterruptIn start(PA_10);
InterruptIn stop(PA_6);
InterruptIn reset(PA_1);


DigitalOut STB(PB_12);
PortOut buttonOut(PortC, 0xFF);

SPI spi(PB_15, PB_14, PB_13);

Ticker ledTicker;
Ticker displayTicker;

uint8_t reverse(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}


void setDigit(int digitIndex, int number) {
    STB = 0;
    spi.write(Set_data_command);
    STB = 1;

    STB = 0;
    spi.write(dig[digitIndex]);
    spi.write(seg7[number]);
    STB = 1;
}

void setLed(int index, bool on) {
    STB = 0;
    spi.write(Set_data_command);
    STB = 1;
    STB = 0;
    spi.write(led[index]);
    spi.write(on << 7);
    STB = 1;
}

uint8_t getButtons() {
    uint8_t data = 0;
    STB = 0;
    spi.write(read_buttons);
    for (int i = 0; i < 4; i++) {
        data |= spi.write(0xFF) >> i;
    }
    STB = 1;
    return reverse(data);
}


int ledCounter = 0b10000000;
int millis, seconds, minutes, hours;


void displayISR() {


    millis++;
    if (millis > 999) {
        seconds++;
        millis = 0;

    }
    if (seconds >= 60) {
        minutes++;
        seconds = 0;

    }
    if (minutes >= 60) {
        hours++;
        minutes = 0;

    }
    if (hours >= 10) {
        hours = 0;

    }

}

void ledISR() {
    ledCounter = ledCounter >> 1;

    if (ledCounter <= 0) {
        ledCounter = 0b10000000;
    }

}

void startTimer(){

    ledTicker.attach(ledISR, 50ms);
    displayTicker.attach(displayISR, 1ms);


}

void stopTimer(){
    displayTicker.detach();
    ledTicker.detach();

}

void resetTimer(){
    millis = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;

}


int main() {
    DigitalInOut MOSI(PB_15);
    MOSI.mode(OpenDrain);
    STB = 1;
    spi.format(8, 3);
    spi.frequency(100000);

    STB = 0;
    spi.write(Display_ON);
    STB = 1;

    ledTicker.attach(ledISR, 50ms);
    displayTicker.attach(displayISR, 1ms);

    start.mode(PullDown);
    start.rise(startTimer);

    stop.mode(PullDown);
    stop.rise(stopTimer);

    reset.mode(PullDown);
    reset.rise(resetTimer);


    while (true) {

        STB = 0;

        setDigit(0, hours);
        setDigit(1, (minutes / 10) % 10);
        setDigit(2, minutes % 10);
        setDigit(3, (seconds / 10) % 10);
        setDigit(4, seconds % 10);
        setDigit(5, (millis / 100) % 10);
        setDigit(6, (millis / 10) % 10);
        setDigit(7, millis % 10);

        for (int i = 0; i < 8; i++) {

            setLed(i, (ledCounter >> i) & 1);
        }

        // buttonOut = getButtons();

        STB = 1;

    }
}