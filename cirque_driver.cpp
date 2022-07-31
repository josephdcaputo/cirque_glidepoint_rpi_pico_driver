#include "cirque_driver.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include <array>
#include <stdio.h>
#include <vector>

#define WRITE_MASK  (uint8_t)0x80
#define READ_MASK   (uint8_t)0xA0
#define FILLER_DATA (uint8_t)0xFC

#define RD_BYTES_FILLER_OFFSET 3

#define RD_ARR_MAX 4
#define WR_ARR_MAX 2

void assert_CS(void) { gpio_put(PIN_CS, 1); }
void deAssert_CS(void) { gpio_put(PIN_CS, 0); }

void writeMask(Regs_t reg, uint8_t* maskedAddress)
{
  *maskedAddress = (uint8_t)(reg | WRITE_MASK);
}

void readMask(Regs_t reg, uint8_t* maskedAddress)
{
  *maskedAddress = (uint8_t)(reg | READ_MASK);
}

void RAP_WriteByte(Regs_t reg, uint8_t data)
{
  uint8_t* readData  = (uint8_t*)malloc((WR_ARR_MAX) * sizeof(uint8_t));
  uint8_t* writeData = (uint8_t*)malloc((WR_ARR_MAX) * sizeof(uint8_t));

  writeMask(reg, &(writeData[0]));
  writeData[1] = FILLER_DATA;

  assert_CS();
  spi_write_read_blocking(SPI_PORT, writeData, readData,
                          WR_ARR_MAX * sizeof(uint8_t));
  deAssert_CS();
}

void RAP_ReadBytes(Regs_t reg, std::vector<uint8_t> data, uint8_t count)
{
  //  See SPI auto-crement read for sequential addresses example code from
  //  cirque "Interfacing to the Pinnacle ASIC Using SPI and I2C.pdf"
  uint8_t* readData =
      (uint8_t*)malloc((count + RD_BYTES_FILLER_OFFSET) * sizeof(uint8_t));
  uint8_t* writeData =
      (uint8_t*)malloc((count + RD_BYTES_FILLER_OFFSET) * sizeof(uint8_t));

  // writeData[0] = reg | READ_MASK;
  readMask(reg, &(writeData[0]));
  for (uint8_t i = 1; i < count + RD_BYTES_FILLER_OFFSET; i++)
  {
    writeData[i] = 0xFC; //  Add filler write data
  }

  assert_CS();
  spi_write_read_blocking(SPI_PORT, writeData, readData,
                          (count + RD_BYTES_FILLER_OFFSET) * sizeof(uint8_t));
  deAssert_CS();

  //  Transfer all data after the first three transfers to a vector
  for (int i = RD_BYTES_FILLER_OFFSET; i < count + RD_BYTES_FILLER_OFFSET; i++)
  {
    data.push_back(readData[i]);
  }

  free(writeData);
  free(readData);
}

// void cirqueTransfer(RegsAccessOp_t regAccessOp, std::vector<uint8_t> addr =
// {},
//                      std::vector<uint8_t> data = {})
// {

//   switch (regAccessOp.accessMode)
//   {
//     case READ:
//       {
//       }

//       break;

//     case WRITE:
//       {
//       }
//       break;

//     default:
//       break;
//   }
// }

void clearFlags(void) { RAP_WriteByte(Regs_t(STATUS1), (uint8_t)0x00); }
void startupSequence(void)
{
  clearFlags();
  RAP_WriteByte(Regs_t(SYSCONFIG1), (uint8_t)0x00);
  RAP_WriteByte(Regs_t(FEEDCONFIG2), (uint8_t)0x1F);
  RAP_WriteByte(Regs_t(FEEDCONFIG1), (uint8_t)0x03);
}