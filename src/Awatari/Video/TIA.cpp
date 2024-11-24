#include "stdafx.h"
#include "TIA.h"

const bool TelevisionInterfaceAdapter::isPlayfieldActive(const unsigned x, const unsigned y)
{
    uint8_t playfieldPatterns[3] = {
        getRegister(PF0),
        getRegister(PF1),
        getRegister(PF2)};

    const int BLOCK_SIZE = 4; // Cada bit controla 4 pixels
    const int MIDPOINT = NTSC_DISPLAY_WIDTH / 2;

    bool mirror_mode = !(getRegister(CTRLPF) & 0b00001000);
    int midpoint = NTSC_DISPLAY_WIDTH / 2;

    int bit_index = (mirror_mode ? (x >= midpoint ? (NTSC_DISPLAY_WIDTH - 1 - x) : x) : x) % (NTSC_DISPLAY_WIDTH / 2);
    bit_index /= BLOCK_SIZE;

    // Determinar o registrador e ajustar bit_index
    uint8_t pattern;
    if (bit_index < 4)
    {
        pattern = playfieldPatterns[0]; // PF0
        bit_index = 4 - bit_index - 1;  // Ordem reversa
    }
    else if (bit_index < 12)
    {
        pattern = playfieldPatterns[1]; // PF1
        bit_index -= 4;                 // Índice normal para PF1
    }
    else
    {
        pattern = playfieldPatterns[2];   // PF2
        bit_index = 7 - (bit_index - 12); // Inversão de bits para PF2
    }

    // Retorna se o bit estiver ativo
    return (pattern >> (7 - bit_index)) & 1;
}

const bool TelevisionInterfaceAdapter::isPlayerActive(const unsigned x, const unsigned y, const uint8_t grp, const uint8_t resp)
{
    if (grp & static_cast<int>(std::pow(2, x - resp)))
        if (x >= resp && x <= resp + 7) // Player is max 8 pixels wide
            return true;

    return false;
}

const bool TelevisionInterfaceAdapter::isMissileActive(const unsigned x, const unsigned y, const uint8_t enam, const uint8_t resm)
{
    if (enam & 1)
        if (x == resm) // Missile is max 1 pixel wide
            return true;

    return false;
}

const bool TelevisionInterfaceAdapter::isBallActive(const unsigned x, const unsigned y, const uint8_t enabl, const uint8_t resbl)
{
    if (enabl & 1)
    {
        if (x >= resbl && x <= resbl + 7) // Ball is max 8 pixels wide, min 1
        {
            uint8_t ctrlpf_ball = (getRegister(CTRLPF) & 0b00110000) >> 4;
            int ball_width = std::pow(2, ctrlpf_ball);

            if ((x - resbl < ball_width))
                return true;
        }
    }

    return false;
}

const sf::Color TelevisionInterfaceAdapter::readColorFromMemory(const uint16_t &addr)
{
    uint8_t color_value = memory[addr];

    uint8_t r = (color_value >> 5) & 0x07; // Bits 5-7
    uint8_t g = (color_value >> 2) & 0x07; // Bits 2-4
    uint8_t b = color_value & 0x03;        // Bits 0-1

    // Escala para 8 bits (0-255)
    r = (r * 255) / 7;
    g = (g * 255) / 7;
    b = (b * 255) / 3;

    return sf::Color(r, g, b);
}

TelevisionInterfaceAdapter::TelevisionInterfaceAdapter(std::array<uint8_t, 4096> &memory, sf::VideoMode &vm)
    : memory(memory), vm(vm)
{
    pixelBuffer.setSize(sf::Vector2f(
        static_cast<float>(vm.width / NTSC_DISPLAY_WIDTH),
        static_cast<float>(vm.height / NTSC_DISPLAY_HEIGHT)));
}

TelevisionInterfaceAdapter::~TelevisionInterfaceAdapter() {}

uint8_t &TelevisionInterfaceAdapter::getRegister(TIARegisterWrite reg)
{
    return memory[reg];
}

void TelevisionInterfaceAdapter::setRegister(TIARegisterWrite reg, uint8_t value)
{
    memory[reg] = value;
}

void TelevisionInterfaceAdapter::renderFrame(sf::RenderTarget &target)
{
    // Preencher o fundo com a cor do playfield
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(vm.width, vm.height));
    background.setFillColor(readColorFromMemory(COLUBK));
    target.draw(background);

    // Percorrer cada pixel da tela
    for (int y = 0; y < NTSC_DISPLAY_HEIGHT; ++y)
    {
        for (int x = 0; x < NTSC_DISPLAY_WIDTH; ++x)
        {
            pixelBuffer.setPosition(x * pixelBuffer.getSize().x, y * pixelBuffer.getSize().y);
            if (isPlayfieldActive(x, y))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUPF));
                target.draw(pixelBuffer);
            }
            if (isPlayerActive(x, y, getRegister(GRP0), getRegister(RESP0)))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUP0));
                target.draw(pixelBuffer);
            }
            if (isPlayerActive(x, y, getRegister(GRP1), getRegister(RESP1)))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUP1));
                target.draw(pixelBuffer);
            }
            if (isMissileActive(x, y, getRegister(ENAM0), getRegister(RESM0)))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUP0));
                target.draw(pixelBuffer);
            }
            if (isMissileActive(x, y, getRegister(ENAM1), getRegister(RESM1)))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUP1));
                target.draw(pixelBuffer);
            }
            if (isBallActive(x, y, getRegister(ENABL), getRegister(RESBL)))
            {
                pixelBuffer.setFillColor(readColorFromMemory(COLUPF));
                target.draw(pixelBuffer);
            }
        }
    }
}
