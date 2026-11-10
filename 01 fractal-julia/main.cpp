#include <iostream>
#include <fmt/core.h>
#include <fmt/format.h>
#include <complex>
#include <cstring>//memset

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> //Busca en el sistema o en alguna dependencia

#include "fractal-serial.h" //Entre comillas si el módulo está dentro del Proyecto
#include "fractal-simd.h" 



// Ejes en coordenadas x,y
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

#define WIDTH 1600
#define HEIGHT 900

// imagen, textura
uint32_t *buffer = nullptr;


enum class runtime_type {
    SERIAL_1,
    //Puede tener mas variaciones
    SIMD
};

int main()
{

    buffer = new uint32_t[WIDTH * HEIGHT];

    runtime_type r_type = runtime_type::SERIAL_1;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Dibujo funcion");

    // Configurar textos
    sf::Font font("arial.ttf");
    sf::Text text(font, "Fractal", 24);
    text.setFillColor(sf::Color::Blue);
    text.setPosition({10, 10});
    text.setStyle(sf::Text::Bold);

    std::string options = "[1] Serial [3] SIMD| Up/Downn: Iterations";
    sf::Text text_options(font, options, 24);
    text_options.setFillColor(sf::Color::Blue);
    text_options.setPosition({10, window.getView().getSize().y - 40});
    text.setStyle(sf::Text::Bold);

    // Load a sprite to display
    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);

    int frames = 0;
    int fps = 0;
    sf::Clock clock;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::KeyReleased>())
            {
                auto evt = event->getIf<sf::Event::KeyReleased>();

                switch (evt->scancode)
                {
                case sf::Keyboard::Scan::Up:
                    max_iteraciones += 10;
                    break;
                case sf::Keyboard::Scan::Down:
                    max_iteraciones -= 10;
                    break;
                case sf::Keyboard::Scan::Num1:
                    r_type=runtime_type::SERIAL_1;
                    break;
                case sf::Keyboard::Scan::Num3:
                    r_type=runtime_type::SIMD;
                    break;

                
            }
            std::memset(buffer,0,WIDTH*HEIGHT*sizeof(uint32_t));
        }
    }

        //julia_serial(x_min, y_min, x_max, y_max, WIDTH, HEIGHT, buffer);

        std::string mode="";

        if(r_type==runtime_type::SERIAL_1){
            mode="Serial 1";
        julia_serial(x_min, y_min, x_max, y_max, WIDTH, HEIGHT, buffer);
        
        }else if(r_type==runtime_type::SIMD){
            mode="Simd";
            julia_simd(x_min, y_min, x_max, y_max, WIDTH, HEIGHT, buffer);

        }

        texture.update((const uint8_t *)buffer);

        // medir fps
        frames++;

        if (clock.getElapsedTime().asSeconds() >= 1.0f)
        {
            fps = frames;
            frames = 0;
            clock.restart();
        }

        auto msg = fmt::format("Fractal{} - Iteraciones: {},FPS:{}", mode, max_iteraciones,fps);
        text.setString(msg);

        // std::cout<<"FPS: "<<fps<<std::endl;

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);
        window.draw(text);
        window.draw(text_options);

        // Update the window
        window.display();
    }

    return 0;
    
}