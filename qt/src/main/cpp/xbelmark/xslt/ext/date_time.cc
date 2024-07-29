#include "xbelmark/xslt/ext/date_time.h"

#include <deque>
#include <stdexcept>
#include <string>
#include <utility>

#include <libxml/xpathInternals.h>
#include <libxslt/extensions.h>

#include "xbelmark/datetime/datetime.h"
#include "xbelmark/xml/xpath/xpath.h"

using xbelmark::memory::UniquePtr;
using xbelmark::xml::xpath::NewXmlXPathObject;
using xbelmark::xml::xpath::PopValue;
using xbelmark::xml::xpath::PushValue;

namespace xbelmark {
namespace xslt {
namespace ext {

const xmlChar *DateTime::NamespaceUri() {
  return reinterpret_cast<const xmlChar *>(
      "xalan://io.github.hc1839.xbelmark.xslt.ext.DateTime");
}

void *DateTime::InitFunction(xsltTransformContextPtr ctxt, const xmlChar *URI) {
  xsltRegisterExtFunction(
      ctxt,
      reinterpret_cast<const xmlChar *>("dateTimeToUnix"),
      URI,
      dateTimeToUnix);
  return nullptr;
}

void DateTime::dateTimeToUnix(xmlXPathParserContextPtr ctxt, int nargs) {
  if (nargs != 1) {
    throw std::invalid_argument(
        "Invalid number of arguments for `dateTimeToUnix`.");
  }
  std::deque<UniquePtr<xmlXPathObject>> args;
  // Pop arguments from the stack, and push them onto the deque.
  for (int i = 0; i != nargs; ++i) {
    args.push_front(PopValue(ctxt));
  }
  // Convert argument to a string.
  if (args[0]->type != xmlXPathObjectType::XPATH_STRING) {
    PushValue(ctxt, std::move(args[0]));
    xmlXPathStringFunction(ctxt, 1);
    args[0] = PopValue(ctxt);
  }
  const std::string input(reinterpret_cast<const char *>(args[0]->stringval));
  double input_time;
  // Convert date/time to seconds since epoch.
  try {
    try {
      input_time = xbelmark::datetime::DateTime(input);
    } catch (const std::exception &) {
      input_time = xbelmark::datetime::Date(input);
    }
  } catch (const std::exception &) {
    throw std::invalid_argument(
        "Not a valid `xs:dateTime` or `xs:date` format: " + input);
  }
  // Push result onto the stack.
  UniquePtr<xmlXPathObject> result(xbelmark::xml::xpath::NewXmlXPathObject());
  result->type = xmlXPathObjectType::XPATH_NUMBER;
  result->floatval = input_time;
  PushValue(ctxt, std::move(result));
}

} // namespace ext
} // namespace xslt
} // namespace xbelmark
