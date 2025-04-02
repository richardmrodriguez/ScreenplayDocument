#pragma once

#include <string>
#include <vector>

#include "text_position.hpp"

struct PDFPageSize 
{
    float width = -1.0f;
    float height = -1.0f;
    float resolution_points = 72.0f;
};

struct PDFWord 
{
    std::string text;
    float text_length = -1.0f;
    TextPosition position;
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
    PDFPageSize pagesize;
};

struct PDFDoc 
{
    std::vector<PDFPage> pages;
};
