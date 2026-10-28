#include <iostream>
#include <fmt/core.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> //Busca en el sistema o en alguna dependencia

#include "fractal-serial.h" //Entre comillas si el módulo está dentro del Proyecto

// Ejes en coordenadas x,y
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

#define WIDTH 1600
#define HEIGHT 900

// imagen, textura
uint32_t *buffer = nullptr;


int main()
{

    buffer = new uint32_t[WIDTH*HEIGHT];

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Dibujo funcion");

    julia_serial(x_min, y_min, x_max, y_max, WIDTH, HEIGHT,buffer);

    // Load a sprite to display
    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);

    texture.update((const uint8_t*)buffer);


    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Update the window
        window.display();

        
 
    }

    return 0;
}