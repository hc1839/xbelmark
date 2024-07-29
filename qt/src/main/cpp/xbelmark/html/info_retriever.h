#ifndef XBELMARK_HTML_INFO_RETRIEVER_H
#define XBELMARK_HTML_INFO_RETRIEVER_H

#include <memory>
#include <string>

#include <QObject>
#include <QUrl>

namespace xbelmark {
namespace html {

/**
 *  Retriever of information about an HTML document for XBELmark.
 */
class InfoRetriever final : public QObject {
  Q_OBJECT

 public:
  /**
   *  @param url
   *    URL to the HTML document.
   */
  InfoRetriever(const QUrl &url);

  virtual ~InfoRetriever();

  /**
   *  URL of the HTML document.
   */
  const QUrl &url() const;

  /**
   *  HTML document as a string.
   */
  const std::string &html() const;

  /**
   *  Title of the HTML document.
   */
  const std::string &title() const;

  /**
   *  HTML title with illegal file name characters under Windows replaced with
   *  legal characters.
   *
   *  @param html_title
   *    HTML title to have illegal characters replaced.
   *
   *  @return
   *    Title with leading and trailing whitespace removed, intervening
   *    whitespace replaced with a single space, and illegal characters
   *    replaced with an underscore. If the title is empty, the URL is used.
   */
  std::string win_title_name() const;

 private:
  class Impl;

  std::unique_ptr<Impl> p_impl_;
};

} // namespace html
} // namespace xbelmark

#endif
