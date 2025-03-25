#pragma once

#include <stdio.h>
#include <string>
#include "pdf_doc.hpp"
#include "src/screenplay_doc.hpp"
#include "sp_margins.hpp"

class PDFScreenplayParser {
    public:
    static ScreenplayDoc get_screenplay_doc_from_pdf_filepath(std::string filepath);
    static ScreenplayDoc get_screenplay_doc_from_pdfdoc(PDFDoc pdfdoc); // this is only for testing -- this should be deprecated when we integrate a proper PDF Library for extraction
};
