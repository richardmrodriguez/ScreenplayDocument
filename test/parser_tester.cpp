#include "parser_tester.hpp"
#include "libs/parsers/pdf/element_indentations.hpp"
#include <vector>

int main()
{

    PDFDoc test_pdf;
    PDFPage test_page;
    PDFLine test_line;
    PDFWord test_word;

    ElementIndentationsPoints indents_pts_ref;

    auto line_with_new_word = [](float x_pos, float y_pos, std::string linetext, PDFLine line = {}) -> PDFLine {
            PDFWord new_word;
            new_word.text = linetext;
            new_word.position.y = y_pos; //TODO: TEST delta between lines and automatic EMPTY LINE adding...
            new_word.position.x = x_pos;
            new_word.text_length = linetext.length() * 7.2f;
            line.words.push_back(new_word);
            return line;
    };

    std::vector<SPType> types_vec;

    // TODO: add REVISION HEADER
    // TODO: add REVISED LINE (with revision asterisks in margins)
    PDFLine scene_number_line = line_with_new_word(518.4f, 720.0f, "1A.");
    test_page.lines.push_back(scene_number_line);

    PDFLine scene_heading_line = line_with_new_word(50.4f, 708.0f, "*1B*");
    scene_heading_line = line_with_new_word(indents_pts_ref.action, 708.0f, "INT.", scene_heading_line);
    scene_heading_line = line_with_new_word(indents_pts_ref.action + (7.2f * 6), 708.0f, "HOUSE", scene_heading_line);
    scene_heading_line = line_with_new_word(indents_pts_ref.action + (7.2f * 13), 708.0f,"-", scene_heading_line);
    scene_heading_line = line_with_new_word(indents_pts_ref.action + (7.2f * 15), 708.0f, "DAY", scene_heading_line);
    scene_heading_line = line_with_new_word(indents_pts_ref.right, 708.0f, "*1B*", scene_heading_line);

    test_page.lines.push_back(scene_heading_line);
    
    test_page.lines.push_back(line_with_new_word(indents_pts_ref.action, 696.0f, "ACTION"));
    PDFLine character_line = line_with_new_word(indents_pts_ref.character, 684.0f,"CHARACTER");
    test_page.lines.push_back(line_with_new_word(indents_pts_ref.character + (72.0),684.0f, "(CHARACTER-EXTENSION)", character_line));
    test_page.lines.push_back(line_with_new_word(indents_pts_ref.parenthetical, 672.0f,"(PARENTHETICAL)")); //
    test_page.lines.push_back(line_with_new_word(indents_pts_ref.dialogue, 660.0f,"DIALOGUE"));
    PDFLine emptyline;
    test_page.lines.push_back(emptyline);

    test_pdf.pages.push_back(test_page);

    ScreenplayDoc new_doc = Screenplay::_get_screenplay_doc_from_pdfdoc_obj(test_pdf);


    printf("Number of pages: %d\n", new_doc.pages.size());
    for (size_t p = 0; p < new_doc.pages.size(); p++)
    {
        ScreenplayPage current_page = new_doc.pages[p];
        
        printf("PAGE NUMBER: %s\n", current_page.page_number.c_str());
        for (size_t l = 0; l < current_page.lines.size(); l++)
        {
            ScreenplayLine current_line = current_page.lines[l];
            std::string line_type = SPTypeToString(current_line.line_type);
            std::string line_text = SPGetLineAsString(current_line);
            printf("%-20s%-26s%23s%-10s\n", 
                "Current Line Type: ", 
                line_type.c_str(),
                "Line text:  ",
                line_text.c_str()
            );
            if (current_line.scene_number != "") printf("-----SCENE NUMBER: %s\n", current_line.scene_number.c_str());
            if (current_line.text_elements.size() < 1)
            {
                printf("------ EMPTY LINE!!! -------\n");
                continue;
            }
            for (size_t e = 0; e < current_line.text_elements.size(); e++)
            {
                ScreenplayTextElement current_text_element = current_line.text_elements[e];
                std::string text_element_type = SPTypeToString(current_text_element.element_type);
                //printf("        Text Element: %s,        Element Type: %s\n", current_text_element.text.c_str(), text_element_type.c_str());
                printf("%20s%8s%-26s%-10s%s\n", 
                    "Element Type:  ",
                    " ",
                    text_element_type.c_str(),
                    "Element text:  ", 
                    current_text_element.text.c_str()
                ); 
            }

        }
    }

    //std::array<std::string, _TYPECOUNT> all_types_as_strings = getSPTypesAsStrings();
//
    //for (size_t i = 0; i < all_types_as_strings.size(); i++){
    //    printf("%s\n", all_types_as_strings[i].c_str());
    //}

    return 0;

}