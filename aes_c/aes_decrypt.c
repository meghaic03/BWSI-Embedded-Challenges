#include "uart.h"
#include "bearssl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IV_LEN 0x10
#define KEY_LEN 0x10
#define KEY "thistheembseckey"
int main() {
    uart_init(UART2);
    int ret; 
    char * key = KEY;
    unsigned char iv[IV_LEN];
    for(int i = 0; i < IV_LEN; i++) {
        iv[i] = uart_read(UART2, BLOCKING, &ret);
    }
    unsigned short data_len = uart_read(UART2, BLOCKING, &ret) | (uart_read(UART2, BLOCKING, &ret) << 8) ;
    char * data = malloc(data_len);  
    for(int i = 0; i < data_len; i++) {
        data[i] = uart_read(UART2, BLOCKING, &ret);
    }
    // TODO: Decrypt here!
    
    //ve is encrypt, vd is decrypt, vc is ?
    //with_mc = 0
    //with_cbc = anything other than 0 (takes more than a block size of data?)
    FILE *key = fopen("aeskeyfile0.bin","rb");
    
    int num;
    fscanf(key,"%d", &num);
    
    fclose(key);
    
    
    
    char * flag = strstr(data, "embsec{");
    int i = 0;
    while(flag[i] != '}') {
        uart_write(UART2, data[i]);
        i += 1;
    }
    uart_write(UART2, data[i]);
    uart_write(UART2, '\n');
    return 0;
}


