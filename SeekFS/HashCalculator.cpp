//
//  HashCalculator.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#include "HashCalculator.h"
#include <cstring>

namespace {
    const uint32_t S[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
}

MD5::MD5() : finalized(false) {
    count[0] = count[1] = 0;
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

void MD5::update(const unsigned char* input, size_t length) {
    size_t index = count[0] / 8 % 64;
    
    if ((count[0] += (length << 3)) < (length << 3)) count[1]++;
    count[1] += (length >> 29);
    
    size_t first_part = 64 - index;
    size_t i;
    
    if (length >= first_part) {
        memcpy(&buffer[index], input, first_part);
        transform(buffer);
        
        for (i = first_part; i + 63 < length; i += 64)
            transform(&input[i]);
        
        index = 0;
    } else {
        i = 0;
    }
    
    memcpy(&buffer[index], &input[i], length - i);
}

void MD5::update(const std::string& data) {
    update(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
}

std::string MD5::final() {
    if (!finalized) {
        unsigned char padding[64] = {0x80};
        
        unsigned char bits[8];
        encode(count, bits, 8);
        
        size_t index = count[0] / 8 % 64;
        size_t padLen = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLen);
        update(bits, 8);
        
        finalized = true;
    }
    
    unsigned char digest[16];
    encode(state, digest, 16);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; i++) {
        ss << std::setw(2) << static_cast<unsigned int>(digest[i]);
    }
    
    return ss.str();
}

void MD5::transform(const unsigned char block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t x[16];
    
    decode(block, x, 64);
    
    for (uint32_t i = 0; i < 64; i++) {
        uint32_t f, g;
        
        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
        }
        
        f = f + a + K[i] + x[g];
        a = d;
        d = c;
        c = b;
        b = b + ((f << S[i]) | (f >> (32 - S[i])));
    }
    
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void MD5::encode(const uint32_t* input, unsigned char* output, size_t length) {
    for (size_t i = 0, j = 0; j < length; i++, j += 4) {
        output[j] = input[i] & 0xff;
        output[j+1] = (input[i] >> 8) & 0xff;
        output[j+2] = (input[i] >> 16) & 0xff;
        output[j+3] = (input[i] >> 24) & 0xff;
    }
}

void MD5::decode(const unsigned char* input, uint32_t* output, size_t length) {
    for (size_t i = 0, j = 0; j < length; i++, j += 4) {
        output[i] = ((uint32_t)input[j]) |
                   (((uint32_t)input[j+1]) << 8) |
                   (((uint32_t)input[j+2]) << 16) |
                   (((uint32_t)input[j+3]) << 24);
    }
}

std::string MD5::calculate(const std::string& data) {
    MD5 ctx;
    ctx.update(data);
    return ctx.final();
}

std::string MD5::calculateFile(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath.string());
    }
    
    MD5 ctx;
    char buffer[1024 * 16];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        ctx.update(reinterpret_cast<unsigned char*>(buffer), file.gcount());
    }
    
    return ctx.final();
}

std::string HashCalculator::calculateFileSizeHash(const fs::path& filePath) {
    try {
        auto fileSize = fs::file_size(filePath);
        std::stringstream ss;
        ss << fileSize << "_" << filePath.filename().string();
        return ss.str();
    } catch (const std::exception&) {
        return "0_error";
    }
}

std::string HashCalculator::calculateMD5(const fs::path& filePath) {
    return MD5::calculateFile(filePath);
}
