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
#include "sun.hpp"
#include "rabbits.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(width,heigth), "Cells evolution");
    window.setFramerateLimit(60);

    Font arcade;
    arcade.loadFromFile("arcade.ttf");

    Text elapsed("",arcade,50);
    elapsed.setPosition(Vector2f(heigth+100,100));

    Text map("",arcade,30);
    map.setPosition(Vector2f(heigth+100,200));

    Time timeelapsed;

    Clock clock;

    Rabbits zhoverch(100);

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

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::K)
                    zhoverch.kill(rand()% zhoverch.fluffle);

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::R)
                    show_rabbits = !show_rabbits;

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::S)
                    show_sun = !show_sun;
        }

        // Update time
        timeelapsed = clock.getElapsedTime();
        elapsed.setString(" Time is " + std::to_string(timeelapsed.asMilliseconds()));

        // Automatic sunrise & sunset
        if (timeelapsed.asMilliseconds() > regime)
            {
                renewsun();
                regime+=45000;
                string lol = "";
                for (int i=0;i<mapsize;i++)
                    {for (int j=0;j<mapsize;j++)
                        {lol+=to_string(field_of_sun[j][i]); lol+="    ";}
                    lol+="\n";}
                map.setString(lol);
                       
            }

        if (timeelapsed.asMilliseconds() > rabbit_regime)
            {
                zhoverch.move(field_of_sun);
                rabbit_regime+=rabbit_speed;
            }

        window.clear(grey);
        window.draw(elapsed);
        // window.draw(map);
        if(show_sun) window.draw(sun);
        if(show_rabbits) window.draw(zhoverch);
        window.display();
    }
   
    return 0;
}