#include "screenplay_doc.hpp"

//TODO: MISSING TYPES
std::string SPTypeToString(SPType type)
{
    switch (type) {
        case SPType::SP_ACTION:                 return std::string("SP_ACTION");
        
        case SPType::SP_CHARACTER:              return std::string("SP_CHARACTER");
        case SPType::SP_CHARACTER_EXTENSION:    return std::string("SP_CHARACTER_EXTENSION");
        case SPType::SP_PARENTHETICAL:          return std::string("SP_PARENTHETICAL");
        case SPType::SP_DIALOGUE:               return std::string("SP_DIALOGUE");
        case SPType::SP_DG_MORE_CONTINUED:      return std::string("SP_DG_MORE_CONTINUED");
        //DUAL DIALOGUE
        case SPType::SP_DD_L_CHARACTER:         return std::string("SP_DD_L_CHARACTER");
        case SPType::SP_DD_L_PARENTHETICAL:     return std::string("SP_DD_L_PARENTHETICAL");
        case SPType::SP_DD_L_DIALOGUE:          return std::string("SP_DD_L_DIALOGUE");
        case SPType::SP_DD_L_MORE_CONTINUED:    return std::string("SP_DD_L_MORE_CONTINUED");
        case SPType::SP_DD_R_CHARACTER:         return std::string("SP_DD_R_CHARACTER");
        case SPType::SP_DD_R_PARENTHETICAL:     return std::string("SP_DD_R_PARENTHETICAL");
        case SPType::SP_DD_R_DIALOGUE:          return std::string("SP_DD_R_DIALOGUE");
        case SPType::SP_DD_R_MORE_CONTINUED:    return std::string("SP_DD_R_MORE_CONTINUED");
        // SCENE HEADING
        case SPType::SP_SCENE_HEADING:          return std::string("SP_SCENE_HEADING");
        case SPType::SP_INT_EXT:                return std::string("SP_INT_EXT");
        case SPType::SP_TIME_OF_DAY:            return std::string("SP_TIME_OF_DAY");
        case SPType::SP_LOCATION:               return std::string("SP_LOCATION");
        case SPType::SP_SUBLOCATION:            return std::string("SP_SUBLOCATION");
        case SPType::SP_TRANSITION:             return std::string("SP_TRANSITION");
        case SPType::SP_MORE_CONTINUED:         return std::string("SP_MORE_CONTINUED");
        // TITLE PAGE
        case SPType::TP_TITLE:                  return std::string("TP_TITLE");
        case SPType::TP_BYLINE:                 return std::string("TP_BYLINE");
        case SPType::TP_AUTHOR:                 return std::string("TP_AUTHOR");
        case SPType::TP_DRAFT_DATE:             return std::string("TP_DRAFT_DATE");
        case SPType::TP_CONTACT:                return std::string("TP_CONTACT");
        //-------------
        case SPType::NON_CONTENT_TOP:           return std::string("NON_CONTENT_TOP");
        case SPType::NON_CONTENT_BOTTOM:        return std::string("NON_CONTENT_BOTTOM");
        case SPType::NON_CONTENT_LEFT:          return std::string("NON_CONTENT_LEFT");
        case SPType::NON_CONTENT_RIGHT:         return std::string("NON_CONTENT_RIGHT");
        case SPType::SP_OTHER:                  return std::string("SP_OTHER");
        case SPType::NONE:                      return std::string("NONE");
        default:                                return std::string("!!--INVALID--!!");
    }
}

std::array<std::string, SPType::_TYPECOUNT> getSPTypesAsStrings() 
{
    std::array<std::string, _TYPECOUNT> alltypes;

    for (int i = SP_ACTION; i < _TYPECOUNT; i++) {
        SPType t = static_cast<SPType>(i);
        alltypes[i] = SPTypeToString(t);
    }

    return alltypes;
}

std::string SPGetLineAsString(ScreenplayLine line)
{
    if (line.text_elements.size() < 0)
    {
        return std::string("");
    }
    std::string str;
    for (size_t i = 0; i < line.text_elements.size(); i++)
    {
        ScreenplayTextElement cur_text_element = line.text_elements.at(i);
        str.append(cur_text_element.preceding_whitespace_chars, ' ');
        str.append(cur_text_element.text);
        printf("-----PRECEDING WHITESPACE: %d\n", cur_text_element.preceding_whitespace_chars);
    }
    return str;
}
