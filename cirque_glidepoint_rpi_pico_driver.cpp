#include "cirque_driver.h"
#include "hardware/divider.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include <stdio.h>

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID   uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for
// information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

// GPIO defines
// Example uses GPIO 2
#define CIRQUE_HW_DR                                                           \
  9 // Cirque Hardware Ready interrput (new data is available to be retreive
    // from the trackpoint)
#define ALIAZ_SWITCH 22
#define STATUS_LED   25

int main()
{
  stdio_init_all();

  // Set up our UART
  uart_init(UART_ID, BAUD_RATE);
  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  // GPIO initialisation.
  // We will make this GPIO an input, and pull it up by default
  gpio_init(CIRQUE_HW_DR);
  gpio_set_dir(CIRQUE_HW_DR, GPIO_IN);
  gpio_pull_up(CIRQUE_HW_DR);

  gpio_init(ALIAZ_SWITCH);
  gpio_set_dir(ALIAZ_SWITCH, GPIO_IN);
  gpio_pull_up(ALIAZ_SWITCH);

  gpio_init(STATUS_LED);
  gpio_set_dir(STATUS_LED, GPIO_OUT);

  // Example of using the HW divider. The pico_divider library provides a more
  // user friendly set of APIs over the divider (and support for 64 bit
  // divides), and of course by default regular C language integer divisions are
  // redirected thru that library, meaning you can just use C level `/` and `%`
  // operators and gain the benefits of the fast hardware divider.
  int32_t dividend = 123456;
  int32_t divisor  = -321;
  // This is the recommended signed fast divider for general use.
  divmod_result_t result = hw_divider_divmod_s32(dividend, divisor);
  printf("%d/%d = %d remainder %d\n", dividend, divisor,
         to_quotient_s32(result), to_remainder_s32(result));
  // This is the recommended unsigned fast divider for general use.
  int32_t         udividend = 123456;
  int32_t         udivisor  = 321;
  divmod_result_t uresult   = hw_divider_divmod_u32(udividend, udivisor);
  printf("%d/%d = %d remainder %d\n", udividend, udivisor,
         to_quotient_u32(uresult), to_remainder_u32(uresult));

  // SPI initialization. This example will use SPI at 1MHz.
  spi_cpha_t spi_cpha_e = SPI_CPHA_1;
  spi_cpol_t spi_cpol_e = SPI_CPOL_0;

  spi_init(SPI_PORT, 1e6);
  spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_CS, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

  int one_off_cnt = 0;
  while (1)
  {
    if (gpio_get(ALIAZ_SWITCH))
    {
      one_off_cnt = 0;
      gpio_put(STATUS_LED, true);
    } else
    {
      if (one_off_cnt < 1)
      {
        puts("Your depressing the KEY_SWITH!");
        startupSequence();
        gpio_put(STATUS_LED, false);
        one_off_cnt++;
      }
    }

    sleep_ms(1);
  }

  return 0;
}
