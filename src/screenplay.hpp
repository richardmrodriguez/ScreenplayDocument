#pragma once

#include "screenplay_doc.hpp"
#include "screenplay_import.hpp"
#include "screenplay_export.hpp"

class Screenplay {
    enum class ExternalDocType {
        PDF,
        FDX,
        FOUNTAIN,
    };
    
    public:
    
    static ScreenplayDoc get_screenplay_doc_from_file(std::string filepath, ExternalDocType doctype);
    static ScreenplayDoc _get_screenplay_doc_from_pdfdoc_obj(PDFDoc pdfdoc);
    
    
};
