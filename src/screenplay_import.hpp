#pragma once

#include "../libs/parsers/pdf/pdf_screenplay_parser.hpp"

namespace Importer {


    static ScreenplayDoc get_doc_from_pdf_filepath(std::string filepath) { // need to set up interface to get PDF from filepath via external PDF library
        return PDFScreenplayParser::get_screenplay_doc_from_pdf_filepath(filepath);
    }

    static ScreenplayDoc _get_doc_from_pdfdoc_object(PDFDoc pdfdoc) {
        return PDFScreenplayParser::get_screenplay_doc_from_pdfdoc(pdfdoc);
    }

    static ScreenplayDoc get_doc_from_fdx(std::string filepath);
    static ScreenplayDoc get_doc_from_fountain(std::string filepath);

};