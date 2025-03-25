#pragma once

// MARGINS are relative from 0,0 , the BOTTOM and LEFT
struct SPMarginsInches
{
    float pagewidth =       8.50;
    float pageheight =      11.00;

    float left =            1.50;
    float right =           7.50;
    float top =             10.00;
    float bottom =          1.00;
    // Element margins, relative to left side of page
    float action =          1.50;
    float character =       3.70;
    float dialogue =        2.50;
    float parenthetical =   3.10;

    //TODO: dual dialogue...
    //TODO: 


};

struct SPMarginsPoints
{
    float pagewidth =       72.0 * 8.50;
    float pageheight =      72.0 * 11.00;
    float left =            72.0 * 1.50;
    float right =           72.0 * 7.50;
    float top =             72.0 * 10.00;
    float bottom =          72.0 * 1.00;
    float action =          72.0 * 1.50; // Element margins, relative to left side of page
    float character =       72.0 * 3.70;
    float dialogue =        72.0 * 2.50;
    float parenthetical =   72.0 * 3.10;

    //TODO: dual dialogue...
    //TODO: 
};