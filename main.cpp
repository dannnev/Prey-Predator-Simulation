#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "customSFML.hpp"
#include "sunfunctions.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(1600,900), "Cells evolution");
    window.setFramerateLimit(60);

    RectangleShape suncover[mapsize][mapsize];

    Text elapsed("",arcade,50);
    elapsed.setPosition(Vector2f(900,100));

    arcade.loadFromFile("arcade.ttf");

    tablo_back.loadFromFile("tablo.png");
    tablo.setTexture(tablo_back);
    tablo.setPosition(900,0);

    for (int i=0; i<mapsize; i++)
        for (int j=0; j<mapsize; j++)
            {
                suncover[i][j].setSize(Vector2f(900/mapsize,900/mapsize));
                suncover[i][j].setPosition(Vector2f(900*j/mapsize,900*i/mapsize));
            }

    Clock clock; 

    while (window.isOpen())
    {
        // Button events e
        Event e;
        while (window.pollEvent(e))
        {
            // Close window
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                    window.close();

            // Sunrise & sunset
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Space)
                    {
                        sunset();
                        sunrise();
                        for (int i=0; i<mapsize; i++)
                            for (int j=0; j<mapsize; j++)
                                {
                                    if (field_of_sun[i][j] > 0)
                                    {suncover[i][j].setFillColor(sungradient[min(15,div(field_of_sun[i][j],15).quot)]);}
                                }
                    }
        }

        // Update time
        timeelapsed = clock.getElapsedTime();
        elapsed.setString(" Time is " + std::to_string(timeelapsed.asMilliseconds()));

        // Automatic sunrise & sunset
        if (timeelapsed.asMilliseconds() > regime)
            {
                sunset();
                sunrise();
                for (int i=0; i<mapsize; i++)
                    for (int j=0; j<mapsize; j++)
                        {
                            if (field_of_sun[i][j] > 0)
                            {suncover[i][j].setFillColor(sungradient[min(15,div(field_of_sun[i][j],15).quot)]);}
                        }
                regime+=5000;
            }

        window.clear(grey);
        window.draw(tablo);
        window.draw(elapsed);
        for (int i=0; i<mapsize; i++)
            for (int j=0; j<mapsize; j++)
                if (field_of_sun[i][j] > 0) window.draw(suncover[i][j]);
        window.display();
    }
   
    return 0;
}