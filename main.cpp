#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include <ctime>

#include "customSFML.hpp"
#include "sunfunctions.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(1600,900), "Cells evolution");

    tablo_back.loadFromFile("tablo.png");
    tablo.setTexture(tablo_back);
    tablo.setPosition(900,0);

    sunrise();

    RectangleShape r[mapsize][mapsize];
    for (int i=0; i<mapsize; i++)
        for (int j=0; j<mapsize; j++)
            {
                if (field_of_sun[i][j] > 0)
                {r[i][j].setSize(Vector2f(10,10));
                r[i][j].setPosition(Vector2f(10*j,10*i));
                r[i][j].setFillColor(sungradient[rand()%16]);}
            }
     


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
        }

        window.clear(grey);
        window.draw(tablo);
        for (int i=0; i<mapsize; i++)
            for (int j=0; j<mapsize; j++)
                window.draw(r[i][j]);
        window.display();
    }





    
    return 0;
}