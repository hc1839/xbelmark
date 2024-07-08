#ifndef XBELMARK_XML_WRITER_H
#define XBELMARK_XML_WRITER_H

#include <memory>
#include <string>

#include <libxml/xmlwriter.h>

namespace xbelmark {
namespace xml {

/**
 *  Wrapper of libxml2 `xmlTextWriterPtr`.
 */
class Writer final {
 public:
  /**
   *  @param writer
   *    libxml2 `xmlTextWriterPtr`. Ownership is transferred.
   */
  Writer(xmlTextWriterPtr writer);

  ~Writer();

  /**
   *  Starts the XML document.
   *
   *  @param version
   *    XML version, or empty string for the default.
   *
   *  @param encoding
   *    Encoding of the XML document, or empty string for the default.
   *
   *  @param standalone
   *    Standalone of the XML document, or empty string for the default.
   */
  void StartDocument(
      const std::string &version,
      const std::string &encoding,
      const std::string &standalone);

  void StartElement(const std::string &name);

  void WriteAttribute(const std::string &name, const std::string &content);

  void WriteString(const std::string &content);

  void EndElement();

  void EndDocument();

 private:
  class Impl;

  std::unique_ptr<Impl> p_impl_;
};

} // namespace xml
} // namespace xbelmark

#endif
