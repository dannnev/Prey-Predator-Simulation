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
#include "wolves.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(width,heigth), "Cells evolution");
    window.setFramerateLimit(60);

    Font arcade;
    arcade.loadFromFile("arcade.ttf");

    Text general("",arcade,40);
    general.setPosition(Vector2f(heigth+100,100));

    Time timeelapsed;

    Clock clock;

    Rabbits zhoverch(200);
    Wolves silnychyi(100);

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

            // Kill random rabbit
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::K)
                    zhoverch.kill(rand()% zhoverch.fluffle);

            // Hide rabbits
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::R)
                    show_rabbits = !show_rabbits;

            // Hide sun
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::S)
                    show_sun = !show_sun;
        }

        // Update text screen
        timeelapsed = clock.getElapsedTime();
        general.setString(  "Time   " + std::to_string(timeelapsed.asMilliseconds()) + "\n"
                            "Map size   " + std::to_string(mapsize) + "\n"
                            "Expected value   " + std::to_string(expected) + "\n"
                            "Actual value   " + std::to_string(actual) + "\n"
                            "Rabbits Population   " + std::to_string(zhoverch.fluffle) + "\n"
                            "Wolves Population   " + std::to_string(silnychyi.pack) + "\n"
                            "Gradus   " + std::to_string(gradus) + "\n"
                        );

        // general.setString("");
        // for(int i=0;i<mapsize;i++)
        //     {for(int j=0;j<mapsize;j++)
        //         {
        //             general.setString(general.getString() + std::to_string(zhoverch.rabbitsmap[j][i]) + "  ");
        //         }
        //     general.setString(general.getString() + "\n");
        //     }
        // Automatic sun
        if (timeelapsed.asMilliseconds() > sunrise_regime)
            {
                renewsun();
                sunrise_regime+=60000;                       
            }

        if (timeelapsed.asMilliseconds() > sunset_regime)
            {
                removesun();
                sunset_regime+=60000;                       
            }

        if (timeelapsed.asMilliseconds() > rabbit_regime)
            {
                zhoverch.update();
                rabbit_regime+=rabbit_speed;
            }

        if (timeelapsed.asMilliseconds() > wolf_regime)
            {
                silnychyi.update(zhoverch);
                wolf_regime+=wolf_speed;
            }

        window.clear(grey);
        window.draw(general);
        if(show_sun) window.draw(sun);
        if(show_rabbits) window.draw(zhoverch);
        window.draw(silnychyi);
        window.display();
    }
   
    return 0;
}