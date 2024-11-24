#ifndef ETARI_H
#define ETARI_H

#include "NTSC.h"
#include "TIA.h"

class Awatari
{
private:
    std::array<uint8_t, 4096> memory;

    TelevisionInterfaceAdapter *tia;

    unsigned int scale;
    sf::VideoMode vm;
    sf::RenderWindow *window;

    sf::Event event;

    void initVariables();
    void initVideoMode(const unsigned int width, const unsigned int height);
    void initTelevisionInterfaceAdapter();
    void initWindow();

public:
    Awatari();
    virtual ~Awatari();

    void run();
    void update();
    void render();
};

#endif // ETARI_H
