

#pragma once

#include <cstddef>
#include <codecvt>
#include <locale>
#include <cmath>
#include "pdf_screenplay_parser.hpp"

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

ScreenplayDoc get_screenplay_doc(PDFDoc pdf_doc) {
    ScreenplayDoc new_screenplay_doc;
    new_screenplay_doc.pages.reserve(pdf_doc.pages.size());

    for (size_t p = 0; p < pdf_doc.pages.size(); p++) 
    {
        const PDFPage&  pdfpage = pdf_doc.pages[p];
        ScreenplayPage new_page;
        new_page.lines.reserve(pdfpage.lines.size());
        
        float prev_line_y_pos = 0.0f; // used to insert blank lines before adding the current line
        float line_height = 12.0f;

        SPMarginsInches current_margins;
        float current_resolution = 72.0f;


        for (size_t l = 0; l < pdfpage.lines.size(); l++) 
        {         
            const PDFLine& pdfline = pdfpage.lines[l];
            ScreenplayLine new_line;
            new_line.text_elements.reserve(pdfline.words.size());
            
            for (size_t w = 0; w < pdfline.words.size(); w++) 
            {
                const PDFWord& pdfword = pdfline.words[w];
                ScreenplayTextElement new_text_element;
                // slighly more complex logic in here ;
                // Might need to carry a running "context" of the previous line(s) / type(s) to determine certain elements
                SPType new_type = get_type_for_word(pdfword,
                                                    current_margins,
                                                    current_resolution
                                                    );

                switch(new_type)
                {
                    case SPType::SP_PAGENUM: {
                        new_page.pagenum = pdfword.text;
                        continue;
                    }
                    case SPType::SP_PAGE_REVISION_HEADER: {
                        // TODO: parse the word string to find either the reviison "color / name" or the revision date, or both
                        // TODO: then add to the page accordingly IF the page doesn't have it yet
                        continue;
                    }
                    case SPType::NON_CONTENT_TOP:
                    case SPType::NON_CONTENT_BOTTOM:
                    case SPType::NON_CONTENT_LEFT:
                    case SPType::NON_CONTENT_RIGHT: {
                        // if the get_type_for_word() didn't catch this as some sort of revision marker, header, footer, or scene number, idk what this is lmao
                        continue;
                    }
                    case SPType::SP_SCENENUM:
                        // parse for the scene number, remove asterisks if any, but also set revised if there are asterisks
                        // add scenenum to this line
                        new_line.scenenum = pdfword.text; // TODO: actually remove any asterisks
                        continue;
                    case SPType::SP_REVISION_MARGIN_MARKER:
                        new_line.revised = true;
                        continue;

                }
                
                new_text_element.element_type = new_type;
                new_text_element.text = pdfword.text;                
                new_line.text_elements.push_back(new_text_element);

            }

            // Insert BLANK LINES **before** adding the current line, to ensure proper vertical spacing!
            float cur_y_pos = pdfline.words[0].position.y;
            if (prev_line_y_pos > 1.0) 
            {
                float y_delta = prev_line_y_pos - cur_y_pos;
                if (y_delta > line_height)  
                {
                    int blank_lines = ceil(y_delta / line_height);
                    ScreenplayLine blank_line;
                    ScreenplayTextElement blank_element;
                    blank_line.text_elements.push_back(blank_element);

                    for (int i = 1; i < blank_lines; i++)
                    {
                        new_page.lines.push_back(blank_line);
                    }
                }
            }

            prev_line_y_pos = cur_y_pos;
            new_page.lines.push_back(new_line);
        }
        new_screenplay_doc.pages.push_back(new_page);
    }
    
    return new_screenplay_doc;
}



SPType get_type_for_line(PDFLine line, SPMarginsInches margins_inches, float resolution_points){
    // TODO -- calls get_type_for_word
}

SPType get_type_for_word(PDFWord pdfword, SPMarginsInches margins_inches, float resolution_points) {
    float charwidth = pdfword.font_size * 0.6f;
    float position_tolerance = 0.01f;
    SPMarginsPoints margins; // TODO: calculate the margins in points based on margins_inches and resolution_points...

    
    if (pdfword.position.y < margins.top && pdfword.position.y > margins.bottom) 
    { // within vertical content zone
        
        if (pdfword.position.x < margins.left)
        {
            // verify if it's a scene number
            // it could also be a revision marker...
            // the revision asterisks could also be surrounding / attached to the scene number ...... ow my head
            return  SPType::SP_SCENENUM;
        } else if (pdfword.position.x > margins.right) 
        {
            //figure out if it's a scene number (it probably should be... but still)
            return SPType::SP_SCENENUM;
        }
        // within vertical AND horizontal content zone
        
        // TODO:
        
        auto _within_tolerance = [&x = pdfword.position.x, &position_tolerance](float& b) -> bool {
            if (abs(x - b) > position_tolerance) return false;
            return true;
        };

        // ACTION
        if (_within_tolerance(margins.action)) 
        {

            //check for INT. / EXT. or maybe even bold font or smth, otherwise probably action
            return SP_ACTION;
        };
        if (_within_tolerance(margins.character)) return SP_CHARACTER;
        if (_within_tolerance(margins.dialogue)) return SP_DIALOGUE;
        if (_within_tolerance(margins.parenthetical)) return SP_PARENTHETICAL;
        
        // implement dual dialogue later...

        
    }

    // now the text is EITHER above the top margins or below the bottom margins


    if (pdfword.position.y > margins.top) 
    { 
        if (pdfword.position.x < (margins.pagewidth / 3.0f)) 
        {
            return SPType::NON_CONTENT_TOP;
        }

        float wordwidth = charwidth * pdfword.text.size();
        float rightedge = wordwidth + pdfword.position.x;

        if (((rightedge - margins.right) < position_tolerance) && (pdfword.text.back() == '.')) 
        {
            return SPType::SP_PAGENUM;
        }
        return SPType::NON_CONTENT_TOP;
    }

    if (pdfword.text == "MORE") return SPType::SP_MORE_CONTINUED;
    if (pdfword.text == "CONTINUED") return SPType::SP_MORE_CONTINUED;

    return SPType::NON_CONTENT_BOTTOM; // or (MORE) or (CONTINUED) or just some other normal type
    

    
}


