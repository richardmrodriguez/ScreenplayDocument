#pragma once

#include <unordered_map>
#include <string>

// MARGINS are relative from 0,0 , the BOTTOM and LEFT
struct SPMarginsInches
{

    std::unordered_map<std::string, float> basic_margins;
    std::unordered_map<std::string, float> elements_indentations;

    //These map directly to the string value for each SPType

    SPMarginsInches()
    {

        basic_margins["PAGEWIDTH"] =       8.50;
        basic_margins["PAGEHEIGHT"] =      11.00;
        basic_margins["LEFT"] =            1.50;
        basic_margins["RIGHT"] =           7.50;
        basic_margins["TOP"] =             10.00;
        basic_margins["BOTTOM"] =          1.00;
        
        // Element margins, relative to left side of page
        elements_indentations["SP_ACTION"] =          1.50;
        elements_indentations["SP_CHARACTER"] =       3.50;
        elements_indentations["SP_DIALOGUE"] =        2.50;
        elements_indentations["SP_PARENTHETICAL"] =   3.10;
    }

    //TODO: dual dialogue...
    //TODO: 


};

struct ElementIndentationsPoints
{
    float pagewidth =       72.0 * 8.50;
    float pageheight =      72.0 * 11.00;
    float left =            72.0 * 1.50;
    float right =           72.0 * 7.50;
    float top =             72.0 * 10.00;
    float bottom =          72.0 * 1.00;

    
    float action =          72.0 * 1.50; // Element margins, relative to left side of page
    float character =       72.0 * 3.5;
    float dialogue =        72.0 * 2.50;
    float parenthetical =   72.0 * 3.10;

    //TODO: dual dialogue...
    //TODO: 
};