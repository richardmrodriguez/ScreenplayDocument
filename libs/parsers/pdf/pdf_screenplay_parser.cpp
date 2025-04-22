#include <cstddef>
#include <codecvt>
#include <locale>
#include <cmath>
#include "pdf_screenplay_parser.hpp"
#include <string>
#include <vector>
#include <algorithm>


//TODO: FIX SCENE HEADING PARSING

//TODO:

bool _is_int_ext_marker(const std::string& text, // TODO: make this a lambda in the get_type_for_word func, and pass in the int_ext_strings vector 
    std::vector<std::string> int_ext_strings = {
        "INT.",
        "EXT.",
        "I./E.",
        "INT./EXT."
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

bool _is_scene_heading_modifier(const std::string& text, const std::vector<std::string> scene_heading_modifers)
{
    for (int i = 0; i < scene_heading_modifers.size(); i++)
    {
        if (text.rfind(scene_heading_modifers[i], 0) == 0)
        {
            return true;
        }
    }
    return false;
}

SPType get_type_for_word(const PDFWord pdfword, 
    const ElementIndentationsPoints margins, 
    const float resolution_points,
    const std::vector<std::string> scene_heading_modifiers,
    const SPType previous_type = SPType::NONE,
    const SPType current_line_type = SPType::NONE,
    const SPType previous_previous_type = SPType::NONE,
    const std::string revision_marker = "*"
)
{
        float charwidth = pdfword.font_size * 0.6f; // TODO: calculate actual character width based on font metrics...
        const float position_tolerance = 0.01f;
        


        // within vertical content zone
        if (pdfword.position.y < margins.top && pdfword.position.y > margins.bottom) 
        {
            // RIGHT of right-margin...
            if (
                (
                    (pdfword.position.x) + (pdfword.text_length*charwidth)
                ) >= margins.right
            )
            {
                if (pdfword.text == revision_marker) return SPType::SP_LINE_REVISION_MARKER;
                if (any_of(pdfword.text.begin(), pdfword.text.end(), ::isdigit)) return SPType::SP_SCENENUM;
            }
             // LEFT of left-margin...
                if (pdfword.position.x < margins.left)
                {
                if (any_of(pdfword.text.begin(), pdfword.text.end(), ::isdigit)) return SPType::SP_SCENENUM;
                    
                } 

            
            switch (current_line_type)
            {
                case SPType::SP_SCENE_HEADING:
                    if (pdfword.text == "-") return SPType::SP_SCENE_HEADING_SEPARATOR;

                    switch (previous_type)
                    {
                        case SPType::SP_SCENE_LOCATION:
                        case SPType::SP_SCENE_SUBLOCATION:
                            return previous_type;

                        case SPType::SP_SCENE_HEADING_SEPARATOR:
                            switch (previous_previous_type)
                            {
                                case SPType::SP_SCENE_SUBLOCATION:
                                case SPType::SP_SCENE_LOCATION:
                                    // TODO: BETTER MATCHING AGAINST MODIFIER PATTERNS
                                    if (_is_scene_heading_modifier(pdfword.text, scene_heading_modifiers)) return SPType::SP_SCENE_MODIFIER;
                                    return SPType::SP_SCENE_SUBLOCATION;
                                case SPType::SP_SCENE_MODIFIER:
                                    return SP_SCENE_MODIFIER;
                            }
                    }
            }

            switch (previous_type)
            {   
                
                case SPType::SP_DIALOGUE:
                    return previous_type;
                case SPType::SP_ACTION:
                    return previous_type;
                case SPType::SP_PARENTHETICAL:
                    return previous_type;
                
                case SPType::SP_SCENE_ENVIRONMENT:
                    return SPType::SP_SCENE_LOCATION;

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
            
            auto _within_tolerance = [&x = pdfword.position.x, &position_tolerance](float b) -> bool {
                if (abs(x - b) > position_tolerance) return false;
                return true;
            };
           
           
            // within vertical AND horizontal content zone
            

            // ACTION
            if (_within_tolerance(margins.action)) 
            {

                if (_is_int_ext_marker(pdfword.text))
                {
                    return SPType::SP_SCENE_ENVIRONMENT;
                }


                return SPType::SP_ACTION;
            };
            if (_within_tolerance(margins.character)) return SPType::SP_CHARACTER;
            if (_within_tolerance(margins.dialogue)) return SPType::SP_DIALOGUE;
            if (_within_tolerance(margins.parenthetical)) return SPType::SP_PARENTHETICAL;

            //default for stuff within vertical content zone... 
            return SPType::SP_OTHER;
            //TODO: Title Page detection...


            
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


        // TODO: match these against patterns passed in by the user as a Vector of Strings
        if (pdfword.text == "(MORE)") return SPType::SP_MORE_CONTINUED;
        if (pdfword.text == "(CONTINUED)") return SPType::SP_MORE_CONTINUED;
        if (pdfword.text == "(CONT'D)") return SPType::SP_MORE_CONTINUED;

        return SPType::NON_CONTENT_BOTTOM; // or (MORE) or (CONTINUED) or just some other normal type
    

    
}


//TODO: LET USER PASS IN CUSTOM element_indentations !
//TODO: LET USER PASS IN CUSTOM scene_heading_modifiers
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

        std::vector<std::string> scene_heading_modifiers = {
            "DAY", 
            "NIGHT",
            "MORNING",
            "EVENING",
            "CONTINUOUS",
        };
        ElementIndentationsPoints element_indentations;
        float current_resolution = 72.0f;

        for (size_t l = 0; l < pdfpage.lines.size(); l++) 
        {         
            const PDFLine& pdfline = pdfpage.lines[l];
            if (pdfline.words.size() < 1) continue; //line has no words, SKIP
            
            ScreenplayLine new_line;
            new_line.text_elements.reserve(pdfline.words.size());
            SPType previous_element_type = SPType::NONE;
            SPType previous_previous_element_type = SPType::NONE;
            for (size_t w = 0; w < pdfline.words.size(); w++) 
            {
                const PDFWord& pdfword = pdfline.words[w];
                ScreenplayTextElement new_text_element;
                

                SPType new_type = get_type_for_word(pdfword, // OPTIMIZATION TODO: skipt calling this func sometimes if it's redundant?
                    element_indentations,
                    current_resolution,
                    scene_heading_modifiers,
                    previous_element_type,
                    new_line.line_type,
                    previous_previous_element_type
                );
                
                new_text_element.element_position = pdfword.position; // setting position this early for debug purposes
                
                switch(new_type)
                {   
                    case SPType::SP_DIALOGUE:
                        new_line.line_type = SPType::SP_DIALOGUE;
                        break;
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
                    case SPType::SP_SCENE_ENVIRONMENT:
                        new_line.line_type = SPType::SP_SCENE_HEADING;
                        break;
                        
                    // DON'T add the following as TEXT ELEMENTS -- only the ABOVE will be valid text elements, otherwise CONTINUE
                    case SPType::SP_PAGENUM: {
                        new_page.page_number = pdfword.text;
                        new_line.line_type = SPType::SP_OTHER;
                        continue;
                        //break;
                    }
                    case SPType::SP_PAGE_REVISION_LABEL: {
                        // TODO: parse the word string to find either the reviison "color / name" or the revision date, or both
                        // TODO: then add to the page accordingly IF the page doesn't have it yet

                        new_page.revised = true;
                        new_line.line_type = SPType::SP_OTHER;
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
                
                previous_previous_element_type = previous_element_type;
                previous_element_type = new_type;
                new_line.text_elements.push_back(new_text_element);

            }

            // Account for preceding "blank lines..."
            // this might be redundant if we are still passing the text position to the text elements...
            float cur_y_pos = pdfline.words[0].position.y;
            if (prev_line_y_pos > 1.0) 
            {
                float y_delta = prev_line_y_pos - cur_y_pos;
                if (y_delta > line_height)  
                {
                    u_int8_t blank_lines = ceil(y_delta / line_height);
                    new_line.preceding_empty_lines = blank_lines;
                    
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




