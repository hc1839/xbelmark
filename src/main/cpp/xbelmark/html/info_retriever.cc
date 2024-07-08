#include "xbelmark/html/info_retriever.h"

#include <functional>
#include <regex>
#include <string>

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <libxml/HTMLparser.h>

namespace xbelmark {
namespace html {

class InfoRetriever::Impl final {
 public:
  static const std::regex title_re;

  static void StartElement(
      void *ctx, const xmlChar *name, const xmlChar **atts) {
    Impl *obj = static_cast<Impl *>(ctx);
    if (std::regex_match(reinterpret_cast<const char *>(name), title_re)) {
      obj->is_title_ = true;
    }
  }

  static void EndElement(void *ctx, const xmlChar *name) {
    Impl *obj = static_cast<Impl *>(ctx);
    if (std::regex_match(reinterpret_cast<const char *>(name), title_re)) {
      obj->is_title_ = false;
    }
  }

  static void Characters(void *ctx, const xmlChar *ch, int len) {
    Impl *obj = static_cast<Impl *>(ctx);
    if (obj->is_title_) {
      obj->title_ += reinterpret_cast<const char *>(ch);
    }
  }

  static htmlSAXHandler html_sax_handler() {
    htmlSAXHandler retval;
    retval.internalSubset = nullptr;
    retval.isStandalone = nullptr;
    retval.hasInternalSubset = nullptr;
    retval.hasExternalSubset = nullptr;
    retval.resolveEntity = nullptr;
    retval.getEntity = nullptr;
    retval.entityDecl = nullptr;
    retval.notationDecl = nullptr;
    retval.attributeDecl = nullptr;
    retval.elementDecl = nullptr;
    retval.unparsedEntityDecl = nullptr;
    retval.setDocumentLocator = nullptr;
    retval.startDocument = nullptr;
    retval.endDocument = nullptr;
    retval.startElement = &StartElement;
    retval.endElement = &EndElement;
    retval.reference = nullptr;
    retval.characters = &Characters;
    retval.ignorableWhitespace = nullptr;
    retval.processingInstruction = nullptr;
    retval.comment = nullptr;
    retval.warning = nullptr;
    retval.error = nullptr;
    retval.fatalError = nullptr;
    retval.getParameterEntity = nullptr;
    retval.cdataBlock = &Characters;
    retval.externalSubset = nullptr;
    return retval;
  }

  QUrl url_;

  QNetworkAccessManager manager_;

  std::string html_;

  std::string title_;

  bool is_title_ = false;
};

const std::regex InfoRetriever::Impl::title_re(
    "title",
    std::regex_constants::icase | std::regex_constants::optimize);

InfoRetriever::InfoRetriever(const QUrl &url) : p_impl_(new Impl()) {
  p_impl_->url_ = url;
  {
    QNetworkReply *reply(p_impl_->manager_.get(QNetworkRequest(url)));
    QEventLoop loop;
    QObject::connect(
        reply, &QNetworkReply::finished,
        &loop, &QEventLoop::quit);
    loop.exec();
    p_impl_->html_ = QString(reply->readAll()).toUtf8().constData();
    reply->deleteLater();
  }
  {
    htmlSAXHandler handler(Impl::html_sax_handler());
    htmlParserCtxtPtr ctxt(
        htmlCreatePushParserCtxt(
            &handler, p_impl_.get(), "", 0, "", XML_CHAR_ENCODING_NONE));
    htmlParseChunk(ctxt, p_impl_->html_.data(), p_impl_->html_.size(), 0);
    htmlParseChunk(ctxt, "", 0, 1);
    htmlFreeParserCtxt(ctxt);
  }
}

InfoRetriever::~InfoRetriever() = default;

const QUrl &InfoRetriever::url() const {
  return p_impl_->url_;
}

const std::string &InfoRetriever::html() const {
  return p_impl_->html_;
}

const std::string &InfoRetriever::title() const {
  return p_impl_->title_;
}

std::string InfoRetriever::win_title_name() const {
  const std::regex reserved_re("<|>|:|\"|/|\\\\|\\||\\?|\\*");
  std::string retval(title());
  if (retval.empty()) {
    retval = url().toString().toUtf8().constData();
  }
  retval = std::regex_replace(retval, std::regex("^\\s+|\\s+$"), "");
  retval = std::regex_replace(retval, std::regex("\\s+"), " ");
  retval = std::regex_replace(retval, reserved_re, "_");
  return retval;
}

} // namespace html
} // namespace xbelmark
