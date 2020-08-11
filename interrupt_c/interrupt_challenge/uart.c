
// Hardware Imports
#include "inc/hw_memmap.h" // Peripheral Base Addresses
#include "inc/lm3s6965.h" // Peripheral Bit Masks and Registers
#include "inc/hw_types.h" // Boolean type

// Driver API Imports
#include "driverlib/sysctl.h" // Stystem Control API (clock/reset)

// Component Header
#include "uart.h"

// UART Channels - You can use these names, but don't uncomment them!
// #define UART0 0
// #define UART1 1
// #define UART2 2

// Reading modes - You can use these names, but don't uncomment them!
// #define NONBLOCKING 0
// #define BLOCKING 1

void uart_init(uint8_t uart)
{
// modify the uart_init function for the UART0 case so that it enables the receive interrupt signal
        //search the Stellaris datasheet to find the correct register for enabling the UART0 receive interrupt
                        //Interrupt Number (Bit in Interrupt Registers): 5
                //  find the correct regsiter definition and bit fields in inc/lm3s6965.h in the lib/stellaris folder
                            //UART0_ICR_R --> 461
                            // UART0_IFLS_R --> 455
        //make sure to enable the interrupt before UART is enabled at the end of the function
    //** only one line!
    
  switch (uart){
    case UART0:
      UART0_CTL_R &= ~(UART_CTL_UARTEN); // Disable UART
      UART0_IBRD_R &= ~(UART_IBRD_DIVINT_M); // Set integer baudrate part
      UART0_IBRD_R |= 0x0a;
      UART0_FBRD_R &= ~(UART_FBRD_DIVFRAC_M); // Set fractional baudrate part
      UART0_FBRD_R |= 0x36;
      UART0_LCRH_R &= ~(UART_LCRH_WLEN_M); // Set 8-bit word
      UART0_LCRH_R |= UART_LCRH_WLEN_8;
           
      UART0_IM_R &= ~ (UART_IM_RXIM);
      UART0_IM_R |= UART_IM_RXIM;
          
      UART0_CTL_R |= UART_CTL_UARTEN; // Enable UART
      break;
    case UART1:
      UART1_CTL_R &= ~(UART_CTL_UARTEN); // Disable UART
      UART1_IBRD_R &= ~(UART_IBRD_DIVINT_M); // Set integer baudrate part
      UART1_IBRD_R |= 0x0a;
      UART1_FBRD_R &= ~(UART_FBRD_DIVFRAC_M); // Set fractional baudrate part
      UART1_FBRD_R |= 0x36;
      UART1_LCRH_R &= ~(UART_LCRH_WLEN_M); // Set 8-bit word
      UART1_LCRH_R |= UART_LCRH_WLEN_8;
      UART1_CTL_R |= UART_CTL_UARTEN; // Enable UART
      break;
    case UART2:
      UART2_CTL_R &= ~(UART_CTL_UARTEN); // Disable UART
      UART2_IBRD_R &= ~(UART_IBRD_DIVINT_M); // Set integer baudrate part
      UART2_IBRD_R |= 0x0a;
      UART2_FBRD_R &= ~(UART_FBRD_DIVFRAC_M); // Set fractional baudrate part
      UART2_FBRD_R |= 0x36;
      UART2_LCRH_R &= ~(UART_LCRH_WLEN_M); // Set 8-bit word
      UART2_LCRH_R |= UART_LCRH_WLEN_8;
      UART2_CTL_R |= UART_CTL_UARTEN; // Enable UART
      break;
    default:
      break;
  }
}

uint8_t uart_read(uint8_t uart, int blocking, int *read)
{
  switch (uart){
    case UART0:
      if (blocking) {
        while (UART0_FR_R & UART_FR_RXFE); // Wait for byte (FIFO empty)
        *read = 1;
        return UART0_DR_R & 0xFF; // Return Rx value from data register
      } 
      if (UART0_FR_R & UART_FR_RXFF) { // Check if byte is available (FIFO full)
        *read = 1;
        return UART0_DR_R & 0xFF; // Return Rx value
      } else {
        *read = 0;
        return 0;
      }
      break;
    case UART1:
      if (blocking) {
        while (UART1_FR_R & UART_FR_RXFE); // Wait for byte (FIFO empty)
        *read = 1;
        return UART1_DR_R & 0xFF; // Return Rx value from data register
      } 
      if (UART1_FR_R & UART_FR_RXFF) { // Check if byte is available (FIFO full)
        *read = 1;
        return UART1_DR_R & 0xFF; // Return Rx value
      } else {
        *read = 0;
        return 0;
      }
      break;
    case UART2:
      if (blocking) {
        while (UART2_FR_R & UART_FR_RXFE); // Wait for byte (FIFO empty)
        *read = 1;
        return UART2_DR_R; // Return Rx value from data register
      } 
      if (UART2_FR_R & UART_FR_RXFF) { // Check if byte is available (FIFO full)
        *read = 1;
        return UART2_DR_R; // Return Rx value
      } else {
        *read = 0;
        return 0;
      }
      break;
    default:
      return 0;
      break;
  }
}

void uart_write(uint8_t uart, uint32_t data)
{
  switch (uart){
    case UART0:
      while (UART0_FR_R & UART_FR_BUSY); // Wait for UART ready
      UART0_DR_R = data; // Write data
      break;
    case UART1:
      while (UART1_FR_R & UART_FR_BUSY); // Wait for UART ready
      UART1_DR_R = data; // Write data
      break;
    case UART2:
      while (UART2_FR_R & UART_FR_BUSY); // Wait for UART ready
      UART2_DR_R = data; // Write data
      break;
    default:
      break;
  }
}

void uart_write_str(uint8_t uart, char *str) {
  while (*str) { // Loop until null terminator
    uart_write(uart, (uint32_t)*str++);
  }
}

inline void nl(uint8_t uart) {
  uart_write(uart, '\n');
}

void uart_write_hex(uint8_t uart, uint32_t data) {
  uint32_t nibble;

  for (int shift = 28; shift >= 0; shift -=4) {
    nibble = (data >> shift) & 0xF;
    if (nibble > 9) {
      nibble += 0x37;
    } else {
      nibble += 0x30;
    }
    uart_write(uart, nibble);
  }
}


void UART0_IRQHandler(void)
    
    
{
  // Implement Me!!
    // clear the pending interrupt bit in the correct UART0 register
    //then, the ISR needs to check if the value 0x20 was received over the UART0 channel
            //Hint! You can still use the uart_* functions in the uart.c file even during an ISR
    //Finally, if the value 0x20 is received, call the SysCtlReset(); function to restart the device. (just call it, no need to write it)
    
    UART0_ICR_R |= UART_ICR_RXIC; //clears pending interrupt bit
    
    uint8_t dataReceived = uart_read(UART0, 0, 0);
    if (dataReceived == 0x20){
        SysCtlReset();
    }
    
}
    