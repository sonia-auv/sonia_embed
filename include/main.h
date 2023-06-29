#ifndef __MAIN_H__
#define __MAIN_H__

#include <mbed.h>
#include <rtos.h>
#include "pin.h"

#ifdef INTERFACE
BufferedSerial ftdi(FTDI_IN,FTDI_OUT,FTDI_BAUD_RATE);
#endif

//DigitalOut test(RS485_DRIVER_IN);
  BufferedSerial rs485(RS485_DRIVER_IN,RS485_RECEIVER_OUT,RS485_BAUDRATE);
BufferedSerial debug_link(DEBUG_TX,DEBUG_RX,9600);

DigitalOut rs485_receiver_en(RS485_RECEIVER_EN);
DigitalOut rs485_driver_en(RS485_DRIVER_EN);
DigitalOut rs485_term_en(RS485_TERM_EN);

CAN can(CAN_RX,CAN_TX);


DigitalOut ledRed(LED_RED);
DigitalOut ledYlw(LED_YLW);
DigitalOut ledGrn(LED_GRN);
#endif