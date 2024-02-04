#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "src/customSFML.hpp"
#include "src/sun.hpp"
#include "src/rabbits.hpp"
#include "src/wolves.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(width,heigth), "Prey-Predator Simulation");
    window.setFramerateLimit(int(1000/speed));

    Font arcade;
    arcade.loadFromFile("images/arcade.ttf");

    Text general_info("",arcade,40), time_info("",arcade,40), parameters_info("",arcade,40), instructions("",arcade,20), info("",arcade,20);
    time_info.setPosition(Vector2f(heigth+250,25));
    time_info.setFillColor(text_color);
    general_info.setPosition(Vector2f(heigth+250,250));
    general_info.setFillColor(text_color);
    parameters_info.setPosition(Vector2f(heigth+250,515));
    parameters_info.setFillColor(text_color);
    graphtop.setFont(arcade);
    graphtop.setCharacterSize(20);
    graphtop.setPosition(Vector2f(heigth+25,5));
    graphtop.setString(std::to_string(absolute_y));
    graphtop.setFillColor(text_color);
    graphright.setFont(arcade);
    graphright.setCharacterSize(20);
    graphright.setPosition(Vector2f(heigth+680,heigth-110));
    graphright.setString(std::to_string(absolute_x));
    graphright.setFillColor(text_color);
    info.setPosition(Vector2f(heigth+200,800));
    info.setFillColor(text_color);
    info.setString("Press\tI\tto\tshow\tinstructions");
    instructions.setPosition(Vector2f(heigth+25,700));
    instructions.setFillColor(text_color);
    instructions.setString(
                            "Hide\trabbits\tR \t\t\t Hide\twolves\tW \t\t\t Hide\tsun\tS \t\t\t Show\tplot\tO\tP\n\n"
                            "Enable  Adjust  and  restart \t Left Shift \t\t\t\t Restart \t\t\t Right Shift\n\n"
                            "Increase\tor\tDecrease\tday\tduration\t\t\t\t\t Q\tA\n"
                            "Increase\tor\tDecrease\tnight\tduration \t\t\t W\tS\n"
                            "Increase\tor\tDecrease\tseeds\t\t\t\t\t\t\t\t\t\t\t\t E\tD\n"
                            "Increase\tor\tDecrease\tchance\t\t\t\t\t\t\t\t\t\t\t R\tF\n"
                            "Increase\tor\tDecrease\tmaxshine \t\t\t\t\t\t\t\t\t T\tG\n"
                            "Increase\tor\tDecrease\tspeed\t\t\t\t\t\t\t\t\t\t\t\t Y\tH\n"
                        );

    Texture tex_arrow, tex_dayclock, tex_nightclock, tex_plot, tex_phaseplot, tex_expect, tex_animation, tex_box;
    tex_arrow.loadFromFile("images/arrow.png");
    tex_arrow.setSmooth(true);
    tex_dayclock.loadFromFile("images/dayclock.png");
    tex_dayclock.setSmooth(true);
    tex_nightclock.loadFromFile("images/nightclock.png");
    tex_nightclock.setSmooth(true);
    tex_plot.loadFromFile("images/plot.png");
    tex_phaseplot.loadFromFile("images/phaseplot.png");
    tex_expect.loadFromFile("images/expect.png");
    tex_animation.loadFromFile("images/bunny.png");
    tex_box.loadFromFile("images/box.png");

    // Current_clock always holds either dayclock or nightclock
    Sprite arrow(tex_arrow), dayclock(tex_dayclock), nightclock(tex_nightclock), current_clock, plot(tex_plot), phase(tex_phaseplot), expect(tex_expect), box(tex_box);
    arrow.setOrigin(15.f,45.f);
    arrow.setPosition(Vector2f(heigth+125,125));
    dayclock.setPosition(Vector2f(heigth+25,25));
    nightclock.setPosition(Vector2f(heigth+25,25));
    plot.setPosition(Vector2f(heigth,0));
    phase.setPosition(Vector2f(heigth,0));
    expect.setPosition(Vector2f(heigth+25,250));
    box.setPosition(Vector2f(heigth+250,520));

    // Load gif
    Sprite animation[24],gif;
    for (int j=0;j<6;j++)
        for(int i=0;i<4;i++)
            {
                animation[i+4*j].setTexture(tex_animation);
                animation[i+4*j].setTextureRect(IntRect(55*i,74*j,55,74));
                animation[i+4*j].setPosition(Vector2f(heigth+40,450));
                animation[i+4*j].setScale(3,3);
            }

    // Bar plot
    VertexArray exp(Quads, 4);
    exp[0].position = Vector2f(heigth+50,489);
    exp[1].position = Vector2f(heigth+100,489);
    exp[2].position = Vector2f(heigth+100,369);
    exp[3].position = Vector2f(heigth+50,369);

    exp[0].color = sungradient[19];
    exp[1].color = sungradient[19];
    exp[2].color = sungradient[19];
    exp[3].color = sungradient[19];
    
    act[0].position = Vector2f(heigth+150,489);
    act[1].position = Vector2f(heigth+200,489);

    // Timeelapsed is a general time. Restarter helps to start rotation of the arrow at 0 degrees each time the day/night starts
    Time timeelapsed, restarter;

    Clock clock;

    // Initial population
    Rabbits zho;
    Wolves sil;

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

            // Hide rabbits
            if (e.type == Event::KeyPressed && !adjust)
                if (e.key.code == Keyboard::R)
                    show_rabbits = !show_rabbits;

            // Hide sun
            if (e.type == Event::KeyPressed && !adjust)
                if (e.key.code == Keyboard::S)
                    show_sun = !show_sun;

            // Hide wolves
            if (e.type == Event::KeyPressed && !adjust)
                if (e.key.code == Keyboard::W)
                    show_wolves = !show_wolves;

            // Hide plot
            if (e.type == Event::KeyPressed && !adjust)
                if (e.key.code == Keyboard::P)
                    {
                        show_timeplot = !show_timeplot;
                        if(show_phaseplot) show_phaseplot = !show_phaseplot;
                        graphtop.setPosition(Vector2f(heigth+25,5));
                    }

            // Hide phaseplot
            if (e.type == Event::KeyPressed && !adjust)
                if (e.key.code == Keyboard::O)
                    {
                        show_phaseplot = !show_phaseplot;
                        if(show_timeplot) show_timeplot = !show_timeplot;
                        graphtop.setPosition(Vector2f(heigth+25,83));
                    }

            // Hide instructions
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::I && !show_timeplot && !show_phaseplot)
                    show_instructions = !show_instructions;

            // Adjust button
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::LShift && !show_timeplot && !show_phaseplot)
                    adjust = !adjust;

            // Increase day duration
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Q && adjust)
                    {
                        day_duration+=1000;
                        if (day > night) {day+=1000;duration = day_duration;}
                        night+=1000;
                        box.setPosition(Vector2f(heigth+250,150));
                    }
                    
            // Decrease day duration
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::A && adjust)
                    if (day_duration > 1000)
                        {
                            day_duration-=1000;
                            if(day > night) {day-=1000;duration = day_duration;}
                            night-=1000;
                            box.setPosition(Vector2f(heigth+250,150));
                        }

            // Increase night duration
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::W && adjust)
                    {
                        night_duration+=1000;
                        if(night > day) {night+=1000;duration = night_duration;}
                        day+=1000;
                        box.setPosition(Vector2f(heigth+250,190));
                    }

            // Decrease night duration
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::S && adjust)
                    if (night_duration > 1000)
                        {
                            night_duration-=1000;
                            if (night > day) {night-=1000;duration = night_duration;}
                            day-=1000;
                            box.setPosition(Vector2f(heigth+250,190)); 
                        }

            // Increase amount of seeds
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::E && adjust)
                    {
                        seeds++;
                        box.setPosition(Vector2f(heigth+250,560));
                    }

            // Decrease amount of seeds
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::D && adjust)
                    if (seeds > 1)
                        {
                            seeds--;
                            box.setPosition(Vector2f(heigth+250,560));
                        }

            // Increase chance of seed to spawn
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::R && adjust)
                    if (chance < 99)
                        {
                            chance++;
                            box.setPosition(Vector2f(heigth+250,600));
                        }

            // Decrease chance of seed to spawn
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::F && adjust)
                    if (chance > 0)
                        {
                            chance--;
                            box.setPosition(Vector2f(heigth+250,600));
                        }

            // Increase maxshine
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::T && adjust)
                    {
                        maxshine++;
                        box.setPosition(Vector2f(heigth+250,640));
                    }

            // Decrease maxshine
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::G && adjust)
                    if (maxshine > 1)
                        {
                            maxshine--;
                            box.setPosition(Vector2f(heigth+250,640));
                        }

            // Increase speed
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Y && adjust)
                    {
                        speed+=5;
                        window.setFramerateLimit(int(1000/speed));
                        gif_speed=5*speed;
                        box.setPosition(Vector2f(heigth+250,520));
                    }


            // Decrease speed
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::H && adjust)
                    if (speed > 10)
                        {
                            speed-=5;
                            window.setFramerateLimit(int(1000/speed));
                            gif_speed=5*speed;
                            box.setPosition(Vector2f(heigth+250,520));
                        }

            // Restart
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::RShift && adjust)
                    {
                        zho.restart();
                        sil.restart();

                        removesun();

                        day = 0;
                        night = day_duration;
                        rabbit_regime = 0;
                        wolf_regime = 0;
                        gif_regime = 0;
                        timeelapsed = clock.restart();
                        adjust = 0;
                    }
        }

        // Update text screen
        timeelapsed = clock.getElapsedTime();
        
        time_info.setString(  
                            "Time      \t\t\t\t\t\t\t\t\t\t" + std::to_string(timeelapsed.asMilliseconds()) + "\n"
                            "Next day   \t\t\t\t\t\t\t" + std::to_string(day) + "\n"
                            "Next night  \t\t\t\t\t" + std::to_string(night) + "\n"
                            "Day duration \t\t\t" + std::to_string(day_duration) + "\n"
                            "Night duration\t" + std::to_string(night_duration) + "\n"
                        );

        general_info.setString(
                            "Mapsize\t\t\t\t\t\t\t\t" + std::to_string(mapsize) + "\n"
                            "Expected   \t\t\t\t\t\t" + std::to_string(expected) + "\n"
                            "Actual\t\t\t\t\t\t\t\t\t" + std::to_string(actual) + "\n"
                            "Degree\t\t\t\t\t\t\t\t\t" + std::to_string(degree) + "\n"
                            "Rabbits\t\t\t\t\t\t\t\t" + std::to_string(zho.fluffle) + "\n"
                            "Wolves\t\t\t\t\t\t\t\t\t" + std::to_string(sil.pack) + "\n"
                        );

        parameters_info.setString(
                            "Speed\t\t\t\t\t\t\t\t\t\t" + std::to_string(speed) + "\n"
                            "Seeds\t\t\t\t\t\t\t\t\t\t" + std::to_string(seeds) + "\n"
                            "Chance   \t\t\t\t\t\t\t\t" + std::to_string(chance) + "\n"
                            "Maxshine  \t\t\t\t\t\t" + std::to_string(maxshine) + "\n"
                        );

        // Update clock arrow
        arrow.setRotation(360*(timeelapsed.asMilliseconds()-restarter.asMilliseconds())/duration);

        // Gif
        if (timeelapsed.asMilliseconds() > gif_regime)
            {
                gif=animation[frame];
                if (frame == 13) frame=8;
                else frame++;
                gif_regime+=gif_speed;
            }

        // Daytime
        if (timeelapsed.asMilliseconds() > day)
            {
                renewsun();
                day+=(day_duration+night_duration);
                current_clock = dayclock;
                duration = day_duration;
                restarter = timeelapsed;
            }

        // Nighttime
        if (timeelapsed.asMilliseconds() > night)
            {
                removesun();
                night+=(day_duration+night_duration);
                current_clock = nightclock;
                duration = night_duration;
                restarter = timeelapsed;
            }

        // Rabbits update
        if (timeelapsed.asMilliseconds() > rabbit_regime)
            {
                zho.update();
                rabbit_regime+=speed;
            }

        // Wolves update
        if (timeelapsed.asMilliseconds() > wolf_regime)
            {
                sil.update(zho);
                wolf_regime+=speed;
            }

        window.clear(grey);
        if(adjust) window.draw(box);
        window.draw(time_info);
        window.draw(general_info);
        window.draw(parameters_info);
        if(show_instructions) window.draw(instructions);
        else window.draw(info);
        if(show_sun) window.draw(sun);
        if(show_rabbits) window.draw(zho);
        if(show_wolves) window.draw(sil);
        window.draw(current_clock);
        window.draw(arrow);
        window.draw(expect);
        window.draw(exp);
        window.draw(act);
        window.draw(gif);
        if(show_phaseplot) {window.draw(phase); window.draw(sil.phaseplot); window.draw(graphtop); window.draw(graphright);}
        if(show_timeplot) {window.draw(plot); window.draw(zho.timeplot);window.draw(sil.timeplot); window.draw(graphtop);}        
        window.display();
    }
   
    return 0;
}