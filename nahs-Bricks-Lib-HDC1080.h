#ifndef NAHS_BRICKS_LIB_HDC1080_H
#define NAHS_BRICKS_LIB_HDC1080_H

#include <Arduino.h>
#include <Wire.h>

typedef uint8_t HDC1080_SerialNumber[5];

class NahsBricksLibHDC1080 {
    private:
        uint8_t _addr;
        bool _triggered = false;
        bool _dataPulled = false;
        TwoWire *_wirePort;
        uint16_t _t;
        uint16_t _h;
        uint8_t _tPrecision = 14;
        uint8_t _hPrecision = 14;
    public:
        NahsBricksLibHDC1080();
        bool begin(TwoWire &wirePort = Wire);
        bool isConnected();
        void triggerRead();
        void setPrecision(uint8_t t, uint8_t h);
        double getT();
        double getH();
        void getSN(uint8_t* sn);
        String snToString(HDC1080_SerialNumber sn);
    private:
        void _pullData();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_HDC1080)
extern NahsBricksLibHDC1080 HDC1080;
#endif

#endif // NAHS_BRICKS_LIB_HDC1080_H
