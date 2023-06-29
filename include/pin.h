
#ifndef __PIN_DEF_INTERFACE__
#define __PIN_DEF_INTERFACE__

#if defined(INTERFACE)
#define LED_RED             PA_5
#define LED_YLW             PA_6
#define LED_GRN             PA_7

#define FTDI_IN             PA_9
#define FTDI_OUT            PA_10
#define FTDI_RTS            PA_12
#define FTDI_CTS            PA_11
#define FTDI_DTR            PD_8
#define FTDI_DSR            PD_10
#define FTDI_DCD            PD_9
#define FTDI_RI             PD_11
#define FTDI_BAUD_RATE      (115200)

#define RS485_RECEIVER_OUT  PA_12
#define RS485_DRIVER_IN     PA_11
#define RS485_RECEIVER_EN   PE_12
#define RS485_DRIVER_EN     PE_13
#define RS485_TERM_EN       PE_14
#define RS485_BAUDRATE      (115200)

#define CAN_RX              PB_12
#define CAN_TX              PB_13
#define CAN_SILENT_MODE     PA_3

#define DEBUG_RX            PD_14
#define DEBUG_TX            PD_15

#else
#define LED_RED             PD_1
#define LED_YLW             PD_2
#define LED_GRN             PD_3

#define RS485_RECEIVER_OUT  PA_10
#define RS485_DRIVER_IN     PA_9
#define RS485_RECEIVER_EN   PA_8
#define RS485_DRIVER_EN     PA_11
#define RS485_TERM_EN       PA_12
#define RS485_BAUDRATE      (115200)

#define CAN_RX              PB_12
#define CAN_TX              PB_13
#define CAN_SILENT_MODE     PA_3

#define DEBUG_RX            PD_14
#define DEBUG_TX            PD_15

#define KILL_SWITCH         PB_0
#define MISSION_SWITCH      PB_1
#endif

#endif