#ifndef XBELMARK_XML_XPATH_XPATH_H
#define XBELMARK_XML_XPATH_XPATH_H

#include <libxml/xpath.h>

#include "xbelmark/memory/smart_ptr.h"

namespace xbelmark {
namespace xml {
namespace xpath {

using xbelmark::memory::UniquePtr;

/**
 *  Create a new libxml2 XPath object.
 */
UniquePtr<xmlXPathObject> NewXmlXPathObject();

/**
 *  Pop the value off the given XPath parser context stack.
 */
UniquePtr<xmlXPathObject> PopValue(xmlXPathParserContextPtr ctxt);

/**
 *  Push the given value onto the given XPath parser context stack.
 */
void PushValue(xmlXPathParserContextPtr ctxt, UniquePtr<xmlXPathObject> obj);

} // namespace xpath
} // namespace xml
} // namespace xbelmark

#endif
