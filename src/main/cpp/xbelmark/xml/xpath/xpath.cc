#include "xbelmark/xml/xpath/xpath.h"

#include <libxml/xpathInternals.h>

namespace xbelmark {
namespace xml {
namespace xpath {

UniquePtr<xmlXPathObject> NewXmlXPathObject() {
  return UniquePtr<xmlXPathObject>(
      new xmlXPathObject(),
      [](xmlXPathObject *ptr) -> void {
        xmlXPathFreeObject(ptr);
      });
}

UniquePtr<xmlXPathObject> PopValue(xmlXPathParserContextPtr ctxt) {
  return UniquePtr<xmlXPathObject>(
      valuePop(ctxt),
      [](xmlXPathObject *ptr) -> void {
        xmlXPathFreeObject(ptr);
      });
}

void PushValue(xmlXPathParserContextPtr ctxt, UniquePtr<xmlXPathObject> obj) {
  valuePush(ctxt, obj.release());
}

} // namespace xpath
} // namespace xml
} // namespace xbelmark
