// Header for main.cpp
// Customly chosen properties and SFML library

#include <SFML/Graphics.hpp>

using namespace sf;

// Window size and game map size
const int width = 1650;
const int heigth = 900;
const int mapsize = 300;
const int rang = heigth/mapsize; // size x size of a cell

// Plot size
const int absolute_y = int(log(mapsize)*log(mapsize)*mapsize);
const int absolute_x = int(mapsize);
const int points_horizontal = 692;
const int points_vertical = 851;
const int phase_length = 10000;

//796
//26

// Control keys
bool show_sun = true;
bool show_rabbits = true;
bool show_wolves = true;
bool show_timeplot = false;
bool show_phaseplot = false;
bool show_instructions = false;
bool adjust = false;

// Nice colours
Color grey(223,223,223);
Color rabbit_fur(255, 255, 255);
Color rabbit_plot(255, 0, 79);
Color wolf_fur(86, 86, 86);
Color wolf_plot(0, 29, 255);
Color phase_color(159, 0, 84);
Color text_color(92, 72, 98);
Color sungradient[40] = {Color (255, 228, 92), Color (255, 224, 88), Color (255, 219, 85), Color (255, 215, 81),
Color (255, 210, 78), Color (255, 206, 75), Color (255, 201, 72), Color (255, 197, 70),
Color (255, 192, 67), Color (255, 188, 65), Color (255, 183, 63), Color (255, 179, 62),
Color (255, 174, 60), Color (255, 170, 59), Color (255, 165, 57), Color (255, 160, 57),
Color (255, 156, 56), Color (255, 151, 55), Color (255, 146, 55), Color (255, 141, 55),
Color (254, 137, 55), Color (254, 132, 55), Color (253, 127, 55), Color (252, 122, 56),
Color (251, 117, 56), Color (250, 112, 57), Color (248, 107, 57), Color (247, 102, 58),
Color (246, 97, 59), Color (244, 92, 60), Color (243, 86, 61), Color (241, 81, 62),
Color (239, 75, 63), Color (237, 70, 64), Color (235, 64, 66), Color (233, 57, 67),
Color (231, 51, 68), Color (229, 44, 69), Color (226, 36, 71), Color (224, 26, 72)
};