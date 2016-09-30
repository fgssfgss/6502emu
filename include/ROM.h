#ifndef ROM_H
#define ROM_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

class ROM {
public:
    ROM();

    uint8_t Read(uint16_t offset);

    virtual ~ROM();

protected:
private:
    const size_t size = 0xFFFF - 0x8000;
    std::vector<uint8_t> data;
};

#endif // ROM_H
