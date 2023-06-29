//#define INTERFACE
#include "../include/main.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms

int main(int argc, char const *argv[])
{

    #ifdef INTERFACE
    char buff[2];
    char buff2[2];

#ifdef SERIAL_TEST
    rs485_receiver_en = 0;
    rs485_driver_en = 1;
    rs485_term_en = 1;
    while (true) {
        ftdi.read(buff,1);
        rs485.write(buff,1);
        ledRed = !ledRed;
        rs485.read(buff2,1);
        ftdi.write(buff2,1);
    }
#else
    CANMessage msg;
    while (true) {
        ftdi.read(buff,1);
        can.write(CANMessage(123,buff,1));
        ledRed = !ledRed;
        while(can.read(msg) == 0) {}
        ftdi.write(&(msg.data[0]),1);
    }
#endif


    #else
    char buff[2];

#ifdef SERIAL_TEST
    rs485_receiver_en = 0;
    rs485_driver_en = 1;
    rs485_term_en = 1;
    while (true) {
        rs485.read(buff,1);
        ledRed = !ledRed;
        rs485.write(buff,1);
    }
#else
    CANMessage msg;
    while (true) {
        while(can.read(msg) == 0) {};
        ledRed = !ledRed;
        can.write(CANMessage(124,&(msg.data[0]),1));
    }
#endif
    #endif

}
