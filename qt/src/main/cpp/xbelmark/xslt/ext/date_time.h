#ifndef XBELMARK_XSLT_EXT_DATE_TIME_H
#define XBELMARK_XSLT_EXT_DATE_TIME_H

#include <libxml/xmlstring.h>
#include <libxml/xpath.h>
#include <libxslt/transform.h>

namespace xbelmark {
namespace xslt {
namespace ext {

/**
 *  Conversions of date/time formats.
 */
class DateTime final {
 public:
  /**
   *  URI of the namespace of the extension.
   *
   *  It is currently `xalan://io.github.hc1839.xbelmark.xslt.ext.DateTime` for
   *  compatibility with the Java extension.
   *
   *  @return
   *    URI of the namespace of the extension.
   */
  static const xmlChar *NamespaceUri();

  /**
   *  Register the extension functions associated with @link NamespaceUri
   *  @endlink.
   *
   *  @param ctxt
   *    libxslt transform context.
   *
   *  @param URI
   *    URI returned by @link NamespaceUri @endlink.
   */
  static void *InitFunction(xsltTransformContextPtr ctxt, const xmlChar *URI);

  /**
   *  `dateTimeToUnix` extension function that converts `xs:dateTime` or
   *  `xs:date` in XML Schema format to seconds since epoch.
   *
   *  Input date/time formats are tried, in order, `xs:dateTime` and `xs:date`.
   *
   *  @param ctxt
   *    libxslt transform context.
   *
   *  @param nargs
   *    Number of arguments on the stack. It must be `1`.
   */
  static void dateTimeToUnix(xmlXPathParserContextPtr ctxt, int nargs);
};

} // namespace ext
} // namespace xslt
} // namespace xbelmark

#endif
