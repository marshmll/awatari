#include "stdafx.h"
#include "Awatari.h"

void Awatari::initVariables()
{
    scale = 4;
}

void Awatari::initVideoMode(const unsigned int width, const unsigned int height)
{
    if (width % NTSC_WIDTH != 0 || height % NTSC_HEIGHT != 0)
    {
        std::cerr << "Erro: Escala incompatível com as dimensões da tela." << "\n";
        exit(EXIT_FAILURE);
    }

    vm = sf::VideoMode(width, height);
}

void Awatari::initTelevisionInterfaceAdapter()
{
    tia = new TelevisionInterfaceAdapter(memory, vm);

    tia->setRegister(COLUPF, 0x02);
    tia->setRegister(CTRLPF, 0x00);
    tia->setRegister(PF0, 0b10100000);
    tia->setRegister(PF1, 0b01010101);
    tia->setRegister(PF2, 0b10101010);
    tia->setRegister(COLUBK, 0);
    tia->setRegister(COLUP0, 0xff);
    tia->setRegister(COLUP1, 0xff);
    tia->setRegister(GRP0, 0b11111111);
    tia->setRegister(GRP1, 0b11111111);
    tia->setRegister(RESP0, 50);
    tia->setRegister(RESP1, 100);
}

void Awatari::initWindow()
{
    window = new sf::RenderWindow(vm, "Atari", sf::Style::Close | sf::Style::Titlebar);
    window->setFramerateLimit(60);
}

Awatari::Awatari()
{
    initVariables();
    initVideoMode(NTSC_WIDTH * scale, NTSC_HEIGHT * scale);
    initTelevisionInterfaceAdapter();
    initWindow();
}

Awatari::~Awatari()
{
    delete tia;
    delete window;
}

void Awatari::run()
{
    while (window->isOpen())
    {
        update();
        render();
    }
}

void Awatari::update()
{
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            std::cout << "Close window" << "\n";
            window->close();
            break;

        default:
            break;
        }
    }
}

void Awatari::render()
{
    window->clear();

    tia->renderFrame(*window);

    window->display();
}
