#include <SFML/Graphics.hpp>

using namespace sf;

// Window size & game map size
const int width = 1600;
const int heigth = 900;
const int mapsize = 150;
const int rang = heigth/mapsize;

// Control keys
bool show_rabbits = true;
bool show_sun = true;

// Nice colours
Color grey(223,223,223);
Color sungradient[16] = {Color (250,215,110), Color (250,206,104), Color (250,198,98), Color (250,189,93),
 Color (249,180,89), Color (248,171,85), Color (247,162,82), Color (245,153,80), 
 Color (244,144,79), Color (242,135,77), Color (239,126,77), Color (237,117,77), 
 Color (234,108,77), Color (230,99,78), Color (226,89,79), Color (222,80,80)
};