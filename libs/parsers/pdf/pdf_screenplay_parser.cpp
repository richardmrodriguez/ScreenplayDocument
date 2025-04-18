#include <cstddef>
#include <codecvt>
#include <locale>
#include <cmath>
#include "pdf_screenplay_parser.hpp"
#include <string>
#include <vector>


bool _is_int_ext_marker(const std::string& text, // TODO: make this a lambda in the get_type_for_word func, and pass in the int_ext_strings vector 
    std::vector<std::string> int_ext_strings = {
        "INT.",
        "EXT.",
        "I./E."
    })
{
    for (int i = 0; i < int_ext_strings.size(); i++)
    {
        if (text.rfind(int_ext_strings[i], 0) == 0)
        {
            return true;
        }
    }
    return false;
}

SPType get_type_for_word(const PDFWord pdfword, 
    const ElementIndentationsPoints margins, 
    const float resolution_points,
    const SPType previous_type = SPType::NONE,
    const SPType current_line_type = SPType::NONE)
{
        float charwidth = pdfword.font_size * 0.6f; // TODO: calculate actual character width based on font metrics...
        const float position_tolerance = 0.01f;
        // TODO: calculate the margins in points based on margins_inches and resolution_points...

        switch (current_line_type)
        {
            case SPType::SP_SCENE_HEADING:
                if (pdfword.position.x >= margins.right) return SPType::SP_SCENENUM;
        }

        switch (previous_type)
        {   
            case SPType::SP_PARENTHETICAL:
                return previous_type;

            case SPType::SP_SUBLOCATION:
                if (pdfword.text == "-")
                {
                    return previous_type; // TODO: this is improper and invalid; need to figure out how to use this...
                }
                return previous_type;
            case SPType::SP_LOCATION:
                if (pdfword.text == "-")
                {
                    return previous_type;
                }
                return SPType::SP_SUBLOCATION;
            case SPType::SP_INT_EXT:
                return SPType::SP_LOCATION;

            case SPType::SP_CHARACTER:
                if (pdfword.text.rfind("(", 0) == 0)
                {
                    return SPType::SP_CHARACTER_EXTENSION;
                }
                break;
            case SPType::SP_DD_L_CHARACTER:
                if (pdfword.text.rfind("(", 0) == 0)
                {
                    return SPType::SP_DD_L_CHARACTER_EXTENSION;
                }
                break;
            case SPType::SP_DD_R_CHARACTER:
                if (pdfword.text.rfind("(", 0) == 0)
                {
                    return SPType::SP_DD_R_CHARACTER_EXTENSION;
                }
                break;   
            case SPType::SP_DD_L_CHARACTER_EXTENSION:
                return previous_type;
            case SPType::SP_DD_R_CHARACTER_EXTENSION:
                return previous_type;
            case SPType::SP_CHARACTER_EXTENSION:
                return previous_type;
            
        }
        
        
        if (pdfword.position.y < margins.top && pdfword.position.y > margins.bottom) // within vertical content zone
        { 
            
            if (pdfword.position.x < margins.left)
            {
                // verify if it's a scene number
                // it could also be a revision marker...
                // the revision asterisks could also be surrounding / attached to the scene number ...... ow my head
                return  SPType::SP_SCENENUM;
                
            } else if (pdfword.position.x >= margins.right) 
            {
                //figure out if it's a scene number (it probably should be... but still)
                return SPType::SP_SCENENUM;
            }
            // within vertical AND horizontal content zone
            
            auto _within_tolerance = [&x = pdfword.position.x, &position_tolerance](float b) -> bool {
                if (abs(x - b) > position_tolerance) return false;
                return true;
            };

            // ACTION
            if (_within_tolerance(margins.action)) 
            {

                if (_is_int_ext_marker(pdfword.text))
                {
                    return SPType::SP_INT_EXT;
                }


                return SPType::SP_ACTION;
            };
            if (_within_tolerance(margins.character)) return SPType::SP_CHARACTER;
            if (_within_tolerance(margins.dialogue)) return SPType::SP_DIALOGUE;
            if (_within_tolerance(margins.parenthetical)) return SPType::SP_PARENTHETICAL;
            
            // implement dual dialogue later...

            
        }

        // now the text is EITHER above the top margins or below the bottom margins


        if (pdfword.position.y >= margins.top) 
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

        if (pdfword.text == "(MORE)") return SPType::SP_MORE_CONTINUED;
        if (pdfword.text == "(CONTINUED)") return SPType::SP_MORE_CONTINUED;
        if (pdfword.text == "(CONT'D)") return SPType::SP_MORE_CONTINUED;

        return SPType::NON_CONTENT_BOTTOM; // or (MORE) or (CONTINUED) or just some other normal type
    

    
}

ScreenplayDoc PDFScreenplayParser::get_screenplay_doc_from_pdfdoc(PDFDoc pdf_doc) {
    ScreenplayDoc new_screenplay_doc;
    if (pdf_doc.pages.size() < 1) return new_screenplay_doc; //FIXME: RAISE EXCEPTIONS instead of giving blank documents back...,
    
    new_screenplay_doc.pages.reserve(pdf_doc.pages.size());

    for (size_t p = 0; p < pdf_doc.pages.size(); p++) 
    {
        const PDFPage&  pdfpage = pdf_doc.pages[p];
        if (pdfpage.lines.size() < 1) continue; //page has no lines, SKIP

        ScreenplayPage new_page;
        new_page.lines.reserve(pdfpage.lines.size());
        
        float prev_line_y_pos = 0.0f; // used to insert blank lines before adding the current line
        float line_height = 12.0f;

        ElementIndentationsPoints element_indentations;
        float current_resolution = 72.0f;

        for (size_t l = 0; l < pdfpage.lines.size(); l++) 
        {         
            const PDFLine& pdfline = pdfpage.lines[l];
            if (pdfline.words.size() < 1) continue; //line has no words, SKIP

            ScreenplayLine new_line;
            new_line.text_elements.reserve(pdfline.words.size());
            SPType previous_element_type = SPType::NONE;
            for (size_t w = 0; w < pdfline.words.size(); w++) 
            {
                const PDFWord& pdfword = pdfline.words[w];
                ScreenplayTextElement new_text_element;
                
                SPType new_type = get_type_for_word(pdfword, // OPTIMIZATION TODO: skipt calling this func sometimes if it's redundant?
                                                    element_indentations,
                                                    current_resolution,
                                                    previous_element_type,
                                                    new_line.line_type
                                                    );
                
                
                switch(new_type)
                {   
                    case SPType::SP_DIALOGUE:
                        new_line.line_type = SPType::SP_DIALOGUE;
                    case SPType::SP_PARENTHETICAL:
                        new_line.line_type = SPType::SP_PARENTHETICAL;
                        break;
                    case SPType::SP_DD_L_PARENTHETICAL:
                    case SPType::SP_DD_R_PARENTHETICAL:
                    case SPType::SP_DD_L_DIALOGUE:
                    case SPType::SP_DD_R_DIALOGUE:
                        new_line.line_type = SPType::SP_DUAL_DIALOGUES;
                        break;
                    case SPType::SP_CHARACTER:
                        new_line.line_type = SPType::SP_CHARACTER;
                        break;
                    case SPType::SP_DD_L_CHARACTER:
                    case SPType::SP_DD_R_CHARACTER:
                        new_line.line_type = SPType::SP_DUAL_CHARACTERS;
                        break;
                    case SPType::SP_ACTION:
                        new_line.line_type = SPType::SP_ACTION;
                        break;
                    case SPType::SP_INT_EXT:
                        new_line.line_type = SPType::SP_SCENE_HEADING;
                        break;
                        
                    // DON'T add the following as TEXT ELEMENTS -- only the ABOVE will be valid text elements, otherwise CONTINUE
                    case SPType::SP_PAGENUM: {
                        new_page.page_number = pdfword.text;
                        continue;
                    }
                    case SPType::SP_PAGE_REVISION_LABEL: {
                        // TODO: parse the word string to find either the reviison "color / name" or the revision date, or both
                        // TODO: then add to the page accordingly IF the page doesn't have it yet
                        new_page.revised = true;
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
                        new_line.scene_number = pdfword.text; // TODO: actually remove any asterisks
                        new_line.line_type = SPType::SP_SCENE_HEADING;
                        previous_element_type = SPType::SP_SCENENUM;
                        continue;
                    case SPType::SP_LINE_REVISION_MARKER:
                        new_line.revised = true;
                        previous_element_type = SPType::SP_LINE_REVISION_MARKER;
                        continue;

                }
                
                new_text_element.element_type = new_type;
                new_text_element.text = pdfword.text;
                new_text_element.element_position = pdfword.position;
                
                // CALCULATE PRECEDING WHITESPACE CHARS, IF ANY
                if (w > 0)
                {
                    PDFWord last_word = pdfline.words[w-1];

                    float char_width = 7.2f;
                    int maybe_whitespace_chars = int(round((
                        pdfword.position.x - (last_word.position.x + last_word.text_length)) / char_width)
                    );

                    if ((maybe_whitespace_chars >= 1))
                    {

                        switch (previous_element_type)
                        {
                            case SPType::SP_SCENENUM:
                            case SPType::SP_LINE_REVISION_MARKER:
                                new_text_element.preceding_whitespace_chars = 0;
                                break;
                            
                            default:
                                new_text_element.preceding_whitespace_chars = u_int8_t(maybe_whitespace_chars);
                                break;
                        }
                    }
                    else {
                        printf("NEW TEXT ELEMENT OVERLAPS WITH PREVIOUS ELEMENT! default to 1 space...\n");
                        new_text_element.preceding_whitespace_chars = 1;
                    }
                }
                
                previous_element_type = new_type;
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

ScreenplayDoc PDFScreenplayParser::get_screenplay_doc_from_pdf_filepath(std::string filepath) {
    ScreenplayDoc blank = {};
    return blank;
}




