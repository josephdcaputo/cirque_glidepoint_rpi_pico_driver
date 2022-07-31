#ifndef CIRQUE_DRIVER
#define CIRQUE_DRIVER

#include "pico/stdlib.h"
#include <stdio.h>
#include <vector>

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for
// information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

typedef enum : uint8_t
{
  FIRMWARE_ID      = 0x00, // Firmware ASIC ID.
  FIRMWARE_VERSION = 0x01, // Firmware revision number.
  STATUS1          = 0x02, // Contains status flags about the state of Pinnacle.
  SYSCONFIG1       = 0x03, // Contains system operation and configuration bits.
  FEEDCONFIG1      = 0x04, // Contains feed operation and configuration bits.
  FEEDCONFIG2      = 0x05, // Contains feed operation and configuration bits.
  CALCONFIG1       = 0x07, // Contains calibration configuration bits.
  PS2_AUX_CONTROL  = 0x08, // Contains Data register for PS/2 Aux Control.
  SAMPLE_RATE      = 0x09, // Number of samples generated per second.
  ZIDLE          = 0x0A, // Number of Z=0 packets sent when Z goes from >0 to 0.
  Z_SCALER       = 0x0B, // Contains the pen Z_On threshold.
  SLEEP_INTERVAL = 0x0C, //
  SLEEP_TIMER    = 0x0D, //
  PACKETBYTE_0   = 0x12, // trackpad Data
  PACKETBYTE_1   = 0x13, // trackpad Data
  PACKETBYTE_2   = 0x14, // trackpad Data
  PACKETBYTE_3   = 0x15, // trackpad Data (Pinnacle)
  PACKETBYTE_4   = 0x16, // trackpad Data (Pinnacle)
  PACKETBYTE_5   = 0x17, // trackpad Data (Pinnacle)
} Regs_t;

typedef enum
{
  READ  = 0,
  WRITE = 1
} RegsAccessMode_t;

typedef struct
{
  Regs_t           reg;
  RegsAccessMode_t accessMode;
} RegsAccessOp_t;

void writeMask(Regs_t reg, uint8_t* maskedAddress);
void readMask(Regs_t reg, uint8_t* maskedAddress);

void assert_CS(void);
void deAssert_CS(void);

void RAP_WriteByte(Regs_t reg, uint8_t data);
void RAP_ReadByte(Regs_t reg, std::vector<uint8_t> data);
// void cirqueTransfer(RegsAccessOp_t regAccessOp);

void clearFlags(void);
void startupSequence(void);

#endif