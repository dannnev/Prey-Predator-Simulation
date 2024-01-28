// Header for the main.cpp

#include "sunfunctions.hpp"
#include "suncover.hpp"

Suncover sun;

void renewsun()
{
    sunset();
    sunrise();
    sun.enrich(field_of_sun);
}