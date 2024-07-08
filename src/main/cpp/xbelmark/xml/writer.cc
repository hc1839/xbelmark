#include "xbelmark/xml/writer.h"

#include <stdexcept>

namespace xbelmark {
namespace xml {

class Writer::Impl final {
 public:
  /**
   *  libxml2 `xmlTextWriterPtr`.
   */
  xmlTextWriterPtr writer_;
};

Writer::Writer(xmlTextWriterPtr writer) : p_impl_(new Impl()) {
  p_impl_->writer_ = writer;
  if (!p_impl_->writer_) {
    throw std::invalid_argument("XML writer is null.");
  }
}

Writer::~Writer() {
  if (p_impl_->writer_) {
    xmlFreeTextWriter(p_impl_->writer_);
  }
}

void Writer::StartDocument(
    const std::string &version,
    const std::string &encoding,
    const std::string &standalone) {
  int num_bytes = xmlTextWriterStartDocument(
      p_impl_->writer_,
      version.empty() ? nullptr : version.c_str(),
      encoding.empty() ? nullptr : encoding.c_str(),
      standalone.empty() ? nullptr : standalone.c_str());
  if (num_bytes == -1) {
    throw std::runtime_error("Cannot start the XML document.");
  }
}

void Writer::StartElement(const std::string &name) {
  int num_bytes = xmlTextWriterStartElement(
      p_impl_->writer_,
      reinterpret_cast<const xmlChar *>(name.c_str()));
  if (num_bytes == -1) {
    throw std::runtime_error("Cannot start the `" + name + "` element.");
  }
}

void Writer::WriteAttribute(
    const std::string &name,
    const std::string &content) {
  int num_bytes = xmlTextWriterWriteAttribute(
      p_impl_->writer_,
      reinterpret_cast<const xmlChar *>(name.c_str()),
      reinterpret_cast<const xmlChar *>(content.c_str()));
  if (num_bytes == -1) {
    throw std::runtime_error("Cannot write the `" + name + "` attribute.");
  }
}

void Writer::WriteString(const std::string &content) {
  int num_bytes = xmlTextWriterWriteString(
      p_impl_->writer_,
      reinterpret_cast<const xmlChar *>(content.c_str()));
  if (num_bytes == -1) {
    throw std::runtime_error("Cannot write the text, '" + content + "'.");
  }
}

void Writer::EndElement() {
  if (xmlTextWriterEndElement(p_impl_->writer_) == -1) {
    throw std::runtime_error("Cannot end the current element.");
  }
}

void Writer::EndDocument() {
  if (xmlTextWriterEndDocument(p_impl_->writer_) == -1) {
    throw std::runtime_error("Cannot end the XML document.");
  }
}

} // namespace xml
} // namespace xbelmark
