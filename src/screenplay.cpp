#include "screenplay.hpp"

ScreenplayDoc Screenplay::get_screenplay_doc_from_file(std::string filepath, ExternalDocType doctype) 
{
    ScreenplayDoc empty = {};
    switch (doctype)
    {
        case ExternalDocType::PDF:
            return Importer::get_doc_from_pdf_filepath(filepath);
        default:
            return empty;
    };
}

ScreenplayDoc Screenplay::_get_screenplay_doc_from_pdfdoc_obj(PDFDoc pdfdoc) {
    return Importer::_get_doc_from_pdfdoc_object(pdfdoc);
}