#ifndef TIA_H
#define TIA_H

#include "NTSC.h"

// Definição dos registradores da TIA com seus respectivos endereços
enum TIARegisterWrite : uint16_t
{
    VSYNC = 0x00,  // Vertical Sync Set-Clear
    VBLANK = 0x01, // Vertical Blank Set-Clear
    WSYNC = 0x02,  // Wait for Horizontal Blank
    RSYNC = 0x03,  // Reset Horizontal Sync Counter
    NUSIZ0 = 0x04, // Number-Size player/missle 0
    NUSIZ1 = 0x05, // Number-Size player/missle 1
    COLUP0 = 0x06, // Color-Luminance Player 0
    COLUP1 = 0x07, // Color-Luminance Player 1
    COLUPF = 0x08, // Color-Luminance Playfield
    COLUBK = 0x09, // Color-Luminance Background
    CTRLPF = 0x0A, // Control Playfield, Ball, Collisions
    REFP0 = 0x0B,  // Reflection Player 0
    REFP1 = 0x0C,  // Reflection Player 1
    PF0 = 0x0D,    // Playfield Register Byte 0
    PF1 = 0x0E,    // Playfield Register Byte 1
    PF2 = 0x0F,    // Playfield Register Byte 2
    RESP0 = 0x10,  // Reset Player 0
    RESP1 = 0x11,  // Reset Player 1
    RESM0 = 0x12,  // Reset Missle 0
    RESM1 = 0x13,  // Reset Missle 1
    RESBL = 0x14,  // Reset Ball
    AUDC0 = 0x15,  // Audio Control 0
    AUDC1 = 0x16,  // Audio Control 1
    AUDF0 = 0x17,  // Audio Frequency 0
    AUDF1 = 0x18,  // Audio Frequency 1
    AUDV0 = 0x19,  // Audio Volume 0
    AUDV1 = 0x1A,  // Audio Volume 1
    GRP0 = 0x1B,   // Graphics Register Player 0
    GRP1 = 0x1C,   // Graphics Register Player 1
    ENAM0 = 0x1D,  // Graphics Enable Missle 0
    ENAM1 = 0x1E,  // Graphics Enable Missle 1
    ENABL = 0x1F,  // Graphics Enable Ball
    HMP0 = 0x20,   // Horizontal Motion Player 0
    HMP1 = 0x21,   // Horizontal Motion Player 1
    HMM0 = 0x22,   // Horizontal Motion Missle 0
    HMM1 = 0x23,   // Horizontal Motion Missle 1
    HMBL = 0x24,   // Horizontal Motion Ball
    VDELP0 = 0x25, // Vertical Delay Player 0
    VDELP1 = 0x26, // Vertical Delay Player 1
    VDELBL = 0x27, // Vertical Delay Ball
    RESMP0 = 0x28, // Reset Missle 0 to Player 0
    RESMP1 = 0x29, // Reset Missle 1 to Player 1
    HMOVE = 0x2A,  // Apply Horizontal Motion
    HMCLR = 0x2B,  // Clear Horizontal Move Registers
    CXCLR = 0x2C,  // Clear Collision Latches
};

enum TIARegisterRead : uint16_t
{
    CXM0P = 0x00,  // Read Collision  M0-P1   M0-P0
    CXM1P = 0x01,  //                 M1-P0   M1-P1
    CXP0FB = 0x02, //                 P0-PF   P0-BL
    CXP1FB = 0x03, //                 P1-PF   P1-BL
    CXM0FB = 0x04, //                 M0-PF   M0-BL
    CXM1FB = 0x05, //                 M1-PF   M1-BL
    CXBLPF = 0x06, //                 BL-PF   -----
    CXPPMM = 0x07, //                 P0-P1   M0-M1
    INPT0 = 0x08,  // Read Pot Port 0
    INPT1 = 0x09,  // Read Pot Port 1
    INPT2 = 0x0A,  // Read Pot Port 2
    INPT3 = 0x0B,  // Read Pot Port 3
    INPT4 = 0x0C,  // Read Input (Trigger) 0
    INPT5 = 0x0D,  // Read Input (Trigger) 1
};

class TelevisionInterfaceAdapter
{
private:
    std::array<uint8_t, 4096> &memory;
    sf::VideoMode &vm;

    sf::RectangleShape pixelBuffer;

    const bool isPlayfieldActive(const unsigned x, const unsigned y);

    const bool isPlayerActive(const unsigned x, const unsigned y, const uint8_t grp, const uint8_t resp);

    const bool isMissileActive(const unsigned x, const unsigned y, const uint8_t enam, const uint8_t resm);

    const bool isBallActive(const unsigned x, const unsigned y, const uint8_t enabl, const uint8_t resbl);

    const sf::Color readColorFromMemory(const uint16_t &addr);

public:
    TelevisionInterfaceAdapter(std::array<uint8_t, 4096> &memory, sf::VideoMode &vm);

    virtual ~TelevisionInterfaceAdapter();

    uint8_t &getRegister(TIARegisterWrite reg);

    void setRegister(TIARegisterWrite reg, uint8_t value);

    void renderFrame(sf::RenderTarget &target);
};

#endif // TIA_H