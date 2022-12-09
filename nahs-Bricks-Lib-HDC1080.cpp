#include "nahs-Bricks-Lib-HDC1080.h"


NahsBricksLibHDC1080::NahsBricksLibHDC1080() {
    _addr = 0x40;
}

bool NahsBricksLibHDC1080::begin(TwoWire &wirePort) {
    _wirePort = &wirePort;
    return isConnected();
}

bool NahsBricksLibHDC1080::isConnected() {
    _pullData();
    _wirePort->beginTransmission(_addr);
    _wirePort->write(0xFF);
    uint8_t r = _wirePort->endTransmission();

    if (r != 0) return false;

    r = _wirePort->requestFrom(_addr, (uint8_t)2);
    if (r != 2) return false;

    uint16_t tmp = _wirePort->read() << 8;
    tmp |= _wirePort->read();

    return tmp == 0x1050;
}

void NahsBricksLibHDC1080::triggerRead() {
    if(!_triggered || _dataPulled) {
        uint8_t conf = 0x10;
        if(_tPrecision == 11) conf |= 0b100;
        if(_hPrecision == 11) conf |= 0b1;
        if(_hPrecision == 8) conf |= 0b10;
        _wirePort->beginTransmission(_addr);
        _wirePort->write(0x02);
        _wirePort->write(conf);
        _wirePort->write(0x00);
        _wirePort->endTransmission();

        _wirePort->beginTransmission(_addr);
        _wirePort->write(0x00);
        _wirePort->endTransmission();

        _triggered = true;
        _dataPulled = false;
    }
}

void NahsBricksLibHDC1080::setPrecision(uint8_t t, uint8_t h) {
    _tPrecision = t;
    _hPrecision = h;
}

double NahsBricksLibHDC1080::getT() {
    _pullData();
    return (_t / pow(2, 16)) * 165.0 - 40.0;
}

double NahsBricksLibHDC1080::getH() {
    _pullData();
    return (_h / pow(2, 16)) * 100.0;
}

void NahsBricksLibHDC1080::getSN(uint8_t* sn) {
    _pullData();
    _wirePort->beginTransmission(_addr);
    _wirePort->write(0xFB);
    _wirePort->endTransmission();

    _wirePort->requestFrom(_addr, (uint8_t) 2);
    sn[0] = _wirePort->read();
    sn[1] = _wirePort->read();

    _wirePort->beginTransmission(_addr);
    _wirePort->write(0xFC);
    _wirePort->endTransmission();

    _wirePort->requestFrom(_addr, (uint8_t) 2);
    sn[2] = _wirePort->read();
    sn[3] = _wirePort->read();

    _wirePort->beginTransmission(_addr);
    _wirePort->write(0xFD);
    _wirePort->endTransmission();

    _wirePort->requestFrom(_addr, (uint8_t) 1);
    sn[4] = _wirePort->read();
}

String NahsBricksLibHDC1080::snToString(HDC1080_SerialNumber sn) {
    String str = "";
    for (uint8_t i = 0; i < 5; ++i) {
        if (sn[i] < 16 ) str += String(0, HEX);
        str += String(sn[i], HEX);
    }
    return str;
}

void NahsBricksLibHDC1080::_pullData() {
    if (!_dataPulled && _triggered) {
        for (uint8_t i = 0; i < 20; ++i) {
            uint8_t r = _wirePort->requestFrom(_addr, (uint8_t)4);
            if (r == 0) {
                delay(1);
                continue;
            }
            _t = (_wirePort->read() << 8);
            _t |= _wirePort->read();
            _h = (_wirePort->read() << 8);
            _h |= _wirePort->read();
            _dataPulled = true;
            _triggered = false;
            break;
        }
    }
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_HDC1080)
NahsBricksLibHDC1080 HDC1080;
#endif
