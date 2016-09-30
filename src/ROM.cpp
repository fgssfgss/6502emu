#include <ROM.h>


ROM::ROM() {
    // need to load .nes file and make logic harder
    std::ifstream file("prog.dat", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "File is not exist!\n";
        return;
    }
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    data.reserve(fileSize);
    data.insert(data.begin(),
                std::istream_iterator<uint8_t>(file),
                std::istream_iterator<uint8_t>());
}

uint8_t ROM::Read(uint16_t offset) {
    return data[offset];
}

ROM::~ROM() {
}
