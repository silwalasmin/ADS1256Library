#include "stdio.h"
#include "ADS1256.h"

float clockMHz=7.68;
float vRef=2.5;

#define CLK1 2
#define MOSI1 3
#define MISO1 4
#define pinCS1 5
#define pinDRDY1 6
#define pinRST1 7

float sensor1;

int main(){
    stdio_init_all();
    printf("ADS Starting...");
    //ADS1256 adc1(spi0, clockMHz,vRef,false, CLK1, MISO1, MOSI1, pinCS1, pinDRDY1, pinRST1);

    //adc1.begin( DRATE_100SPS, GAIN_1, false);
    printf("ADS Started");
    while(1){
        printf("mula ");
        //adc1.waitDRDY();
        //adc1.setChannel(0,1);
        //sensor1=adc1.readCurrentChannel();
        printf("%f \n",sensor1);
    }

}

