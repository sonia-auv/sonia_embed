#define INTERFACE
#include "../include/main.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms

int main(int argc, char const *argv[])
{

    #ifdef INTERFACE
    char buff[2];
    char buff2[2];

    rs485_receiver_en = 0;
    rs485_driver_en = 1;
    rs485_term_en = 1;
    while (true) {
//        buff[0]++;
//        debug_link.write(buff,1);
//        rs485.write(buff,1);
//        ledYlw = !ledYlw;
//        ThisThread::sleep_for(BLINKING_RATE);


        ftdi.read(buff,1);
        rs485.write(buff,1);
        ledRed = !ledRed;
        rs485.read(buff2,1);
        ftdi.write(buff2,1);
    }
    #else
    char buff[2];

    rs485_receiver_en = 0;
    rs485_driver_en = 1;
    rs485_term_en = 1;
    while (true) {
        rs485.read(buff,1);
        ledRed = !ledRed;
        rs485.write(buff,1);
    }
    #endif

}
