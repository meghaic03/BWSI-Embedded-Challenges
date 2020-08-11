
// Hardware Imports
#include "inc/hw_types.h" // Boolean type
#include "inc/hw_ints.h" // Interrupt numbers

// Driver API Imports
#include "driverlib/sysctl.h" // System control API (clock/reset)
#include "driverlib/interrupt.h" // Interrupt API

// Application Imports
#include "uart.h"

int main() {
//modify the main function so that global interrupts are enabled
    //add the appropriate initialization code for the UARTs
    //Second, the global interrupts need to be enabled. The first function needed for this, IntEnable(<interrupt number>), will allow the provided interrupt signal to pass through the NVIC. You will have to search through the inc/hw_ints.h file in lib/stellaris for the appropriate definition to pass as an argument.
  char flag[64];
  int ret;
  int i;
  char resp;


  // Initialize!!
    uart_init(UART0); 
    uart_init(UART2); //maybe
    
  // Enable UART0 Interrupt!!
    IntEnable(21); //pass 21 as an argument
    
    
    
  // Enable Interrupts!!
    IntMasterEnable();

  uart_write_str(UART2, "Device is booted!\n");

  // Initialize flag buffer
  for (i = 0; i < 64; i++) {
    flag[i] = 0;
  }
  
  // Do 10 non-blocking reads - assumes the flag is longer than 10 characters
  for (i = 0; i < 10; i++){
    ret = 0; // Clear previous result
    while (ret == 0){
      resp = uart_read(UART2, NONBLOCKING, &ret); // Wait for successful read
    }
    flag[i] = resp;
  }

  // Rest of reads are blocking - stops at newline
  flag[i] = uart_read(UART2, BLOCKING, &ret); // i == 11
  while(flag[i] != '\n'){
      i++;
      flag[i] = uart_read(UART2, BLOCKING, &ret);
  }
  uart_write_str(UART2, flag);
  
  return 0;
}
    