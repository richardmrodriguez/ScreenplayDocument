#pragma once

#include <string>
#include <vector>
#include <array>



enum ScreenplayPageFormat {
    PS_US,
    PS_A4
};
enum SPType {
    SP_ACTION = 0,

    SP_CHARACTER,
    SP_CHARACTER_EXTENSION, // require context to parse (previous word type)
    SP_DG_MORE_CONTINUED, // specifically has MORE or CONTINUED or CONT'D within parentheses
    SP_PARENTHETICAL,
    SP_DIALOGUE,
    SP_TRANSITION,
    
    // SCENE HEADING
    SP_SCENE_HEADING, // begins with INT. , EXT. , or I./E.
    SP_SCENE_HYPHEN, // Breaks up a slugline -- EXT. BASEBALL FIELD - PITCHER'S MOUND - PAST - NIGHT
    SP_INT_EXT, //
    SP_SCENE_TIMEFRAME, // PAST, PRESENT, FUTURE, arbitrary timeframe "BEFORE DINNER", "AFTER THE EXPLOSION", etc.
    SP_TIME_OF_DAY,
    SP_LOCATION,
    SP_SUBLOCATION,

    SP_SHOT_ANGLE, // SHOT or ANGLE on something, NOT a full scene heading / location
    
    SP_PAGENUM, // Nominal page number
    SP_SCENENUM, // Nominal scene number
    
    SP_MORE_CONTINUED,
    SP_FOOTER, // Not sure what footers are used for but....
    
    SP_PAGE_REVISION_HEADER, //may or may not include the date / color (I think it's two lines usually, but it could be one line potentially...?)
    SP_REVISION_MARGIN_MARKER, // asterisks in the left and/or right margins indicate a line or lines have been revised
    
    //DUAL DIALOGUE
    SP_DUAL_CHARACTERS,
    SP_DUAL_DIALOGUES,

    SP_DD_L_CHARACTER,
    SP_DD_L_CHARACTER_EXTENSION,
    SP_DD_L_PARENTHETICAL,
    SP_DD_L_DIALOGUE,
    SP_DD_L_MORE_CONTINUED,

    SP_DD_R_CHARACTER,
    SP_DD_R_CHARACTER_EXTENSION,
    SP_DD_R_PARENTHETICAL,
    SP_DD_R_DIALOGUE,
    SP_DD_R_MORE_CONTINUED,
    
    // TITLE PAGE
    TP_TITLE,
    TP_BYLINE,
    TP_AUTHOR,
    TP_DRAFT_DATE,
    TP_CONTACT,
    // -------------
    SP_OTHER,
    SP_BLANK, // BLANK element?
    SP_OMITTED,
    // Non- content text (asterisks and/or scene numbers in the margins, headers and footers, page numbers, etc.)
    NON_CONTENT_TOP,
    NON_CONTENT_BOTTOM,
    NON_CONTENT_LEFT,
    NON_CONTENT_RIGHT,
    
    NONE,
    _TYPECOUNT
};

struct ScreenplayTextElement {
    std::string text = "";
    SPType element_type = SPType::NONE;
};
struct ScreenplayLine 
{
    std::vector<ScreenplayTextElement> text_elements;
    std::string scenenum;
    SPType line_type = SPType::NONE;
    bool revised = false;
    bool blank = true;
};
struct ScreenplayPage 
{
    std::vector<ScreenplayLine> lines;
    std::string pagenum;
    bool revised = false;
    std::string revision_color;
    ScreenplayPageFormat page_format = PS_US;
    
};
struct ScreenplayDoc 
{
    std::vector<ScreenplayPage> pages;
};

std::string SPTypeToString(SPType type);

std::array<std::string, SPType::_TYPECOUNT> getSPTypesAsStrings();


