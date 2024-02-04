// Header for main.cpp

#include "sunfunctions.hpp"
#include "suncover.hpp"

Suncover sun;

// Fills field_of_sun
void renewsun()
{
    sunrise();
    sun.enrich();
}

// Clears field_of_sun
void removesun()
{
    sunset();
    sun.lessen();
}