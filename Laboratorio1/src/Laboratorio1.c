#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "system_TM4C1294.h" 


extern void UARTStdioIntHandler(void);
extern uint32_t calculaFreq(void);
extern uint32_t calculaLow(void);
extern uint32_t calculaHigh(void);
void ftoa(float n, char *res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char *str, int len);

void UARTInit(void){
  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 250000, SystemCoreClock);
} // UARTInit

void UART0_Handler(void){
  UARTStdioIntHandler();
} // UART0_Handler

void main(void){
  int N;
  int m=1;
  int clock;
    if (m==0){
      clock=24000000;
      N=1;}
    else{
      clock=120000000;
      N=5;}
  
  UARTInit();
  UARTprintf("Inicializado\t");
  SysTickPeriodSet(clock/2); // f = 1Hz para clock = 24MHz
  
  
  //--------------------------------PORT K------------------------------------
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);// Habilita o port K do periferico GPIO   
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK)){}// Verifica se o port K do periferico GPIO e espera ate ocorrer de fato a habilitacao
  GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);// Configura o pino 0 do port N como entrada 


  //Inicia loop de leitura do pino de entrada
  while(1)
  {
    //Variaveis para os calculos a serem exibidos pela uart
    uint32_t t_low;
    uint32_t t_high;
    uint32_t pontos;
    uint32_t calculoReal;
    uint32_t calculoInteiro;
    uint32_t DutyCicle;
    float DC;
    char buffer[6];
    
    t_low = 0;
    t_high = 0;
    pontos = 0;
    
    t_high = calculaHigh();//Realiza a leitura do numero de bit 1
    t_low = calculaLow();//Realiza a leitura do numero de bit 0
    pontos = calculaFreq();//Realiza a leitura da frequencia
    
    calculoReal = clock/(pontos*6*N);//Converte o númeri de pontos para frequencia
    calculoInteiro = (int)calculoReal;//Converte a frequencia lida em inteira
    
    DutyCicle = (t_high*100)/(t_high+t_low);//Calcula o duty cycle em numero inteiro
    //DC = ((float)t_high*100)/(float)(t_high+t_low);//Calcula o duty cycle em float
    DC=(float)(1-(float)(t_low/pontos));

    int valor=DC*1000;//Converte o duty em inteiro    
    
    //Envia as informacoes lidas pelas funcoes de numeros de pontos lidos, frequencia, numero de bit 1 lido, numero de bit 0 lido e valor do duty cycle
    UARTprintf("Pontos: %d \t Frequencia: %d \t High: %d \t Low: %d \t DutyCicle: %d%c\n", pontos, calculoReal, t_high, t_low, valor,'%');
  }
} // main

//Funcao que raliza a conversao de variavel float para string
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

//Funcao que realiza a inversao de um vetor
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 

//Funcao que realiza a conversao de variavel inteira para string
int intToStr(int x, char str[], int d)
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 