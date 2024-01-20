//
// Created by asmin on 1/14/24.
//

#ifndef H
#define H

//for RP2040
#define pinDRDY 5
#define pinRST 6
#define pinCS 7

#define SPI_PORT spi0

//ADS1256 Registers
#define STATUS 0x00
#define MUX 0x01
#define ADCON 0x02
#define DRATE 0x03
#define IO 0x04
#define OFC0 0x05
#define OFC1 0x06
#define OFC2 0x07
#define FSC0 0x08
#define FSC1 0x09
#define FSC2 0x0A

//ADS1256 Commands
#define WAKEUP 0x00
#define RDATA 0x01
#define RDATAC 0x03
#define SDATAC 0x0f
#define RREG 0x10
#define WREG 0x50
#define SELFCAL 0xF0
#define SELFOCAL 0xF1
#define SELFGCAL 0xF2
#define SYSOCAL 0xF3
#define SYSGCAL 0xF4
#define SYNC 0xFC
#define STANDBY 0xFD
#define RESET 0xFE

//Multiplexer codes
#define MUXP_AIN0 0x00
#define MUXP_AIN1 0x10
#define MUXP_AIN2 0x20
#define MUXP_AIN3 0x30
#define MUXP_AIN4 0x40
#define MUXP_AIN5 0x50
#define MUXP_AIN6 0x60
#define MUXP_AIN7 0x70
#define MUXP_AINCOM 0x80

#define MUXN_AIN0 0x00
#define MUXN_AIN1 0x01
#define MUXN_AIN2 0x02
#define MUXN_AIN3 0x03
#define MUXN_AIN4 0x04
#define MUXN_AIN5 0x05
#define MUXN_AIN6 0x06
#define MUXN_AIN7 0x07
#define MUXN_AINCOM 0x08

// Gain Codes
#define GAIN_1 0x00
#define GAIN_2 0x01
#define GAIN_4 0x02
#define GAIN_8 0x03
#define GAIN_16 0x04
#define GAIN_32 0x05
#define GAIN_64 0x06


//Data Rates when the crystal frequency is 7.68Mhz
#define DRATE_30000SPS 0xF0
#define DRATE_15000SPS 0xE0
#define DRATE_7500SPS 0xD0
#define DRATE_3750SPS 0xC0
#define DRATE_2000SPS 0xB0
#define DRATE_1000SPS 0xA1
#define DRATE_500SPS 0x92
#define DRATE_100SPS 0x82
#define DRATE_60SPS 0x72
#define DRATE_50SPS 0x63
#define DRATE_30SPS 0x53
#define DRATE_25SPS 0x43
#define DRATE_15SPS 0x33
#define DRATE_10SPS 0x23
#define DRATE_5SPS 0x13
#define DRATE_2_5SPS 0x03

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

class ADS1256{
public:
    ADS1256(spi_inst *spiNum, float clockSpeedMhz, float vRef, bool userResetPin);
    void writeRegister(uint8_t reg, uint8_t wData);
    uint8_t readRegister(uint8_t reg);
    void sendCommand(uint8_t cmd);
    float readCurrentChannel();
    void setConversionFactor(float val);
    void setChannel(uint8_t channel);
    void setChannel(uint8_t AIP, uint8_t AIN);
    void begin(uint8_t dRate, uint8_t gain, bool bufferenable);
    void waitDRDY();
    void setGain(uint8_t gain);
    void setDelayTime(uint64_t t11_delay, uint64_t t6_delay);
    void readTest();

private:
    uint64_t t11_delay = 8;
    uint64_t t6_delay = 200;
    spi_inst *spiNum;
    void CSON();
    void CSOFF();
    unsigned long read_uint24();
    long read_int32();
    float read_float32();
    uint8_t _pga;
    float _VREF;
    float _conversionFactor;
};
#endif //H


