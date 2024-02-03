//
// Created by asmin on 1/14/24.
//



#include "ADS1256.h"

void blinky(){
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    while (1) {
        gpio_put(25, 0);
        sleep_ms(250);
        gpio_put(25, 1);
        sleep_ms(1000);
    }
}

ADS1256::ADS1256(spi_inst *spiNum, float clockSpeedMhz, float vRef, bool userResetPin,uint SCK,uint MISO,uint MOSI,uint pinCS,uint pinDRDY, uint pinRST) {
    //set DRDY pin as input
    this->pinDRDY=pinDRDY;

    this->pinCS=pinCS;
    this->pinRST=pinRST;
    gpio_init(this->pinDRDY);
    gpio_set_dir(this->pinDRDY,GPIO_IN);

    //set CS as output
    gpio_init(this->pinCS);
    gpio_set_dir(this->pinCS,GPIO_OUT);

    //pull RESETPIN HIGH
    gpio_init(this->pinRST);
    gpio_set_dir(this->pinRST,GPIO_OUT);
    gpio_put(this->pinRST, 1);
    //Reference voltage
    _VREF=vRef;

    //Default Conversion factor
    _conversionFactor =1.0;

    this->spiNum = spiNum;
    spi_init(this->spiNum, (int)clockSpeedMhz*1000000/4 );
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);



}


void ADS1256::writeRegister(uint8_t reg, uint8_t wData) {
    CSON();
    uint8_t buf = WREG | reg;
    spi_write_blocking(this->spiNum,&buf,1);
    spi_write_blocking(this->spiNum,0x00,1);
    spi_write_blocking(this->spiNum,&wData,1);
    sleep_us(this->t11_delay);

    CSOFF();
}

uint8_t ADS1256::readRegister(uint8_t reg) {
    uint8_t readValue;

    CSON();
    uint8_t buf = RREG | reg;
    spi_write_blocking(this->spiNum,&buf,1);
    spi_write_blocking(this->spiNum,0x00,1);
    sleep_us(this->t6_delay);
    readValue = spi_write_blocking(this->spiNum,0x00,1);
    sleep_us(this->t11_delay);
    CSOFF();

    return readValue;
}

void ADS1256::sendCommand(uint8_t reg) {
    CSON();
    waitDRDY();

    spi_write_blocking(this->spiNum, &reg, 1);
    sleep_us(this->t11_delay);  // t11
    CSOFF();
}

void ADS1256::setConversionFactor(float val) { _conversionFactor = val; }

void ADS1256::readTest() {
    uint8_t _highByte, _midByte, _lowByte;
    CSON();
    uint8_t buf=RDATA;
    spi_write_blocking(this->spiNum, &buf, 1);
    sleep_us(this->t11_delay);
    buf = WAKEUP;
    _highByte = spi_write_blocking(this->spiNum, &buf, 1);
    _midByte = spi_write_blocking(this->spiNum, &buf, 1);
    _lowByte = spi_write_blocking(this->spiNum, &buf, 1);

    CSOFF();
}



float ADS1256::readCurrentChannel() {
    CSON();
    uint8_t  buff=RDATA;
    spi_write_blocking(this->spiNum, &buff, 1);
    sleep_us(this->t6_delay);  // t6 delay
    float adsCode = read_float32();
    CSOFF();
    return ((adsCode / 0x7FFFFF) * ((2 * _VREF) / (float)_pga)) *
           _conversionFactor;
}

// Call this ONLY after RDATA command
unsigned long ADS1256::read_uint24() {
    uint8_t _highByte, _midByte, _lowByte;
    unsigned long value;

    uint8_t buf = WAKEUP;
    _highByte = spi_write_blocking(this->spiNum, &buf, 1);
    _midByte = spi_write_blocking(this->spiNum, &buf, 1);
    _lowByte = spi_write_blocking(this->spiNum, &buf, 1);

    // Combine all 3-bytes to 24-bit data using byte shifting.
    value = ((long)_highByte << 16) + ((long)_midByte << 8) + ((long)_lowByte);
    return value;
}

// Call this ONLY after RDATA command
long ADS1256::read_int32() {
    long value = read_uint24();

    if (value & 0x00800000) {
        value |= 0xff000000;
    }

    return value;
}

// Call this ONLY after RDATA command
float ADS1256::read_float32() {
    long value = read_int32();
    return (float)value;
}

// Channel switching for single ended mode. Negative input channel are
// automatically set to AINCOM
void ADS1256::setChannel(uint8_t channel) { setChannel(channel, -1); }

// Channel Switching for differential mode. Use -1 to set input channel to
// AINCOM
void ADS1256::setChannel(uint8_t AIN_P, uint8_t AIN_N) {
    uint8_t MUX_CHANNEL;
    uint8_t MUXP;
    uint8_t MUXN;

    switch (AIN_P) {
        case 0:
            MUXP = MUXP_AIN0;
            break;
        case 1:
            MUXP = MUXP_AIN1;
            break;
        case 2:
            MUXP = MUXP_AIN2;
            break;
        case 3:
            MUXP = MUXP_AIN3;
            break;
        case 4:
            MUXP = MUXP_AIN4;
            break;
        case 5:
            MUXP = MUXP_AIN5;
            break;
        case 6:
            MUXP = MUXP_AIN6;
            break;
        case 7:
            MUXP = MUXP_AIN7;
            break;
        default:
            MUXP = MUXP_AINCOM;
    }

    switch (AIN_N) {
        case 0:
            MUXN = MUXN_AIN0;
            break;
        case 1:
            MUXN = MUXN_AIN1;
            break;
        case 2:
            MUXN = MUXN_AIN2;
            break;
        case 3:
            MUXN = MUXN_AIN3;
            break;
        case 4:
            MUXN = MUXN_AIN4;
            break;
        case 5:
            MUXN = MUXN_AIN5;
            break;
        case 6:
            MUXN = MUXN_AIN6;
            break;
        case 7:
            MUXN = MUXN_AIN7;
            break;
        default:
            MUXN = MUXN_AINCOM;
    }

    MUX_CHANNEL = MUXP | MUXN;

    CSON();
    writeRegister(MUX, MUX_CHANNEL);
    sendCommand(SYNC);
    sendCommand(WAKEUP);
    CSOFF();
}



void ADS1256::begin(uint8_t drate, uint8_t gain, bool buffenable) {
    _pga = 1 << gain;
    sendCommand(
            SDATAC);  // send out SDATAC command to stop continous reading mode.
    writeRegister(DRATE, drate);  // write data rate register
    uint8_t bytemask = 0x07;
    uint8_t adcon = readRegister(ADCON);
    uint8_t byte2send = (adcon & ~bytemask) | gain;
    writeRegister(ADCON, byte2send);
    if (buffenable) {
        uint8_t status = readRegister(STATUS);

        status=status | 0x02;
        writeRegister(STATUS, status);
    }
    sendCommand(SELFCAL);  // perform self calibration
    waitDRDY();
    ;  // wait ADS1256 to settle after self calibration
}

void ADS1256::CSON() {
    gpio_put(pinCS, 1);
}  // digitalWrite(_CS, LOW); }

void ADS1256::CSOFF() {
    gpio_put(pinCS, 0);
}  // digitalWrite(_CS, HIGH); }

void ADS1256::waitDRDY() {
    while (gpio_get(pinDRDY))
        ;
}