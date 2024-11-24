#include "stdafx.h"
#include "TIA.h"

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
    pixelBuffer.setSize(sf::Vector2f(vm.width / NTSC_WIDTH, vm.height / NTSC_HEIGHT));
    // pixelBuffer.setOutlineThickness(-1.f);
    // pixelBuffer.setOutlineColor(sf::Color(255, 255, 255, 100));
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

    const int BLOCK_SIZE = 4; // Cada bit controla 4 pixels
    const int MIDPOINT = NTSC_WIDTH / 2;

    // Carregar padrões do playfield
    uint8_t playfieldPatterns[3] = {
        getRegister(PF0),
        getRegister(PF1),
        getRegister(PF2)};

    // Configurar cor do playfield
    pixelBuffer.setFillColor(readColorFromMemory(COLUPF));

    // Percorrer cada pixel da tela
    for (int y = 0; y < NTSC_HEIGHT; ++y)
    {
        for (int x = 0; x < NTSC_WIDTH; ++x)
        {
            bool mirror_mode = (getRegister(CTRLPF) & 1) != 0;
            int midpoint = NTSC_WIDTH / 2;

            int bit_index = (mirror_mode ? (x >= midpoint ? (NTSC_WIDTH - 1 - x) : x) : x) % (NTSC_WIDTH / 2);
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

            // Renderizar se o bit estiver ativo
            if ((pattern >> (7 - bit_index)) & 1)
            {
                // std::cout << "bit index: " << bit_index << "\n";
                pixelBuffer.setPosition(x * pixelBuffer.getSize().x, y * pixelBuffer.getSize().y);
                target.draw(pixelBuffer);
            }
        }
    }

    // Renderizar Sprites
    auto renderSprite = [&](uint8_t grp, uint8_t resp, sf::Color color)
    {
        pixelBuffer.setFillColor(color);
        for (int y = 0; y < 8; ++y)
        {
            for (int x = 0; x < 8; ++x)
            {
                if (grp & (1 << (7 - x)))
                {
                    int px = resp + x;
                    int py = y;
                    if (px < NTSC_WIDTH && py < NTSC_HEIGHT)
                    {
                        pixelBuffer.setPosition(px * pixelBuffer.getSize().x, py * pixelBuffer.getSize().y);
                        target.draw(pixelBuffer);
                    }
                }
            }
        }
    };

    renderSprite(getRegister(GRP0), getRegister(RESP0), readColorFromMemory(COLUP0));
    renderSprite(getRegister(GRP1), getRegister(RESP1), readColorFromMemory(COLUP1));
}
