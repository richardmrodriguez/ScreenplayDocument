#include "parser_tester.hpp"
#include "libs/parsers/pdf/sp_margins.hpp"

int main()
{

    PDFDoc test_pdf;
    PDFPage test_page;
    PDFLine test_line;
    PDFWord test_word;

    SPMarginsPoints margins_points_reference;

    test_word.text = "INT. HOUSE - NIGHT";
    test_word.position.x = margins_points_reference.action;
    test_word.position.y = 400.0f;

    test_line.words.push_back(test_word);
    //printf("PDF Line word count: %d\n", test_line.words.size());
    //printf("PDFWord in the line: %s\n", test_line.words[0].text.c_str());
    test_page.lines.push_back(test_line);    
    test_pdf.pages.push_back(test_page);

    //printf("Test pdf word: %s\n", test_pdf.pages[0].lines[0].words[0].text.c_str());

    ScreenplayDoc new_doc = Screenplay::_get_screenplay_doc_from_pdfdoc_obj(test_pdf);

    //printf("This compiles lmao\n");

    printf("Number of pages: %d\n", new_doc.pages.size());
    for (size_t p = 0; p < new_doc.pages.size(); p++)
    {
        ScreenplayPage current_page = new_doc.pages[p];
        
        printf("Number of lines: %d\n", current_page.lines.size());
        for (size_t l = 0; l < current_page.lines.size(); l++)
        {
            ScreenplayLine current_line = current_page.lines[l];

            printf("Number of text elements: %d\n", current_line.text_elements.size());
            for (size_t e = 0; e < current_line.text_elements.size(); e++)
            {
                ScreenplayTextElement current_text_element = current_line.text_elements[e];
                std::string text_element_type = SPTypeToString(current_text_element.element_type);
                printf("Text Element: %s,        Element Type: %s\n", current_text_element.text.c_str(), text_element_type.c_str());

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