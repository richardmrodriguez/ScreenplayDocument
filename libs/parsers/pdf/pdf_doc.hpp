#pragma once

#include <string>
#include <vector>

#include "libs/parsers/pdf/text_position.hpp"

struct PDFPageSize 
{
    float width = -1.0f;
    float height = -1.0f;
    float resolution_points = 72.0f;
};

struct PDFWord 
{
    std::string text; // string value of word
    float text_length = -1.0f; // bounding box x-width
    TextPosition position; // x,y PDF Coordinates
    std::string font_name;
    float font_size = 12.0f;
};

struct PDFLine 
{
    std::vector<PDFWord> words;
    
};

struct PDFPage 
{
    std::vector<PDFLine> lines;
    PDFPageSize pagesize; // PDF Width and Height
};

struct PDFDoc 
{
    std::vector<PDFPage> pages;
    std::string pdf_creator; // Which software made this PDF (Final Draft, Celtx, etc....?)
    
};
