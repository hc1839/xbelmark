#include "xbelmark/paste/paste.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <regex>
#include <string>

#include <QClipboard>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QGuiApplication>
#include <QIODevice>
#include <QMessageBox>
#include <QMimeData>
#include <QTextStream>
#include <QUrl>
#include <libxml/xmlwriter.h>

#include "xbelmark/html/info_retriever.h"
#include "xbelmark/paste/cmd_args.h"
#include "xbelmark/paste/cmd_args_parser.h"
#include "xbelmark/paste/format.h"
#include "xbelmark/xml/writer.h"

#ifdef WIN32
#include <codecvt>

#include "xbelmark/winshell/winshell.h"

#define TIMEOUT_MILLISECONDS 5000
#endif

namespace xbelmark {
namespace paste {

#ifdef WIN32
/**
 *  Get the index of an item in a File Explorer window on Windows.
 *
 *  @param fv
 *    File Explorer window with the item to get.
 *
 *  @param item_name
 *    Item to get.
 *
 *  @return
 *    Item index.
 */
int WinGetItemIndex(
    xbelmark::winshell::FolderView &fv,
    const std::string &item_name) {
  fv.pfv->SelectItem(0, SVSI_DESELECTOTHERS | SVSI_FOCUSED);
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wstring_converter;
  xbelmark::winshell::ShellItemIterator shell_item_iter(fv);
  int item_index = 0;
  while (shell_item_iter.HasNext()) {
    auto psi = shell_item_iter.Next();
    LPWSTR lpwstr;
    psi->GetDisplayName(SIGDN_PARENTRELATIVEFORADDRESSBAR, &lpwstr);
    const std::string current_item_name(
        wstring_converter.to_bytes(std::wstring(lpwstr)));
    CoTaskMemFree(lpwstr);
    if (current_item_name == item_name) {
      return item_index;
    }
    ++item_index;
  }
  return -1;
}

/**
 *  Select an item in a File Explorer window on Windows.
 *
 *  @param fv
 *    File Explorer window with the item to select.
 *
 *  @param item_name
 *    Item to select.
 *
 *  @param timeout
 *    Timeout in milliseconds.
 */
void WinSelectItem(
    xbelmark::winshell::FolderView &fv,
    const std::string &item_name,
    long timeout) {
  QElapsedTimer timer;
  timer.start();
  int item_index = -1;
  while (item_index == -1 && timer.elapsed() < timeout) {
    item_index = WinGetItemIndex(fv, item_name);
  }
  if (item_index != -1) {
    // Workaround a bug where the returned index was based on outdated status
    // of the File Explorer.
    item_index = WinGetItemIndex(fv, item_name);
    fv.pfv->SelectItem(
        item_index,
        SVSI_SELECT | SVSI_DESELECTOTHERS | SVSI_ENSUREVISIBLE | SVSI_FOCUSED);
  }
}
#endif

/**
 *  Displays a Qt message box with a critical message titled "Error".
 *
 *  @param message
 *    Message to display.
 */
void DisplayError(const std::string &message) {
  QMessageBox::critical(nullptr, "Error", message.data());
}

/**
 *  Pastes a URI as a bookmark file in the URL format with the `.url` extension
 *  in the current working directory.
 *
 *  @param base_file_name
 *    File name without the extension of the output file, or empty string for
 *    the standard output.
 *
 *  @param bookmark_uri
 *    URI to paste.
 *
 *  @return
 *    Exit status, where `0` indicates success.
 */
int PasteUrl(
    const std::string &base_file_name,
    const std::string &bookmark_uri) {
  std::string url_file_text;
  url_file_text += "[InternetShortcut]\n";
  url_file_text += "URL=" + bookmark_uri + "\n";
  if (base_file_name.empty()) {
    std::cout << url_file_text << std::endl;
  } else {
    const std::string file_name(base_file_name + ".url");
    QFile out_file(QDir::current().filePath(file_name.data()));
    const std::string out_file_path(out_file.fileName().toUtf8().constData());
    if (out_file.exists()) {
      std::string message;
      message += "Bookmark with the same file name exists:\n";
      message += out_file_path;
      DisplayError(message);
      return 1;
    }
    if (!out_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      std::string message;
      message += "Cannot write bookmark at\n";
      message += out_file_path;
      DisplayError(message);
      return 1;
    }
    QTextStream(&out_file) << url_file_text.data();
#ifdef WIN32
    try {
      xbelmark::winshell::FolderView fv(GetForegroundWindow());
      WinSelectItem(fv, file_name, TIMEOUT_MILLISECONDS);
    } catch (const std::exception &) {
    }
#endif
  }
  return 0;
}

/**
 *  Pastes a URI as a bookmark file in the XBEL format with the `.xbel`
 *  extension in the current working directory.
 *
 *  @param base_file_name
 *    File name without the extension of the output file, or empty string for
 *    the standard output.
 *
 *  @param html_title
 *    HTML title.
 *
 *  @param bookmark_uri
 *    URI to paste.
 *
 *  @return
 *    Exit status, where `0` indicates success.
 */
int PasteXbel(
    const std::string &base_file_name,
    const std::string &html_title,
    const std::string &bookmark_uri) {
  const std::string file_name(base_file_name + ".xbel");
  QFile out_file(QDir::current().filePath(file_name.data()));
  const std::string out_file_path(out_file.fileName().toUtf8().constData());
  if (out_file.exists()) {
    std::string message;
    message += "Bookmark with the same file name exists:\n";
    message += out_file_path;
    DisplayError(message);
    return 1;
  }
  try {
    xmlTextWriterPtr text_writer;
    if (base_file_name.empty()) {
      text_writer =
          xmlNewTextWriter(xmlOutputBufferCreateFile(stdout, nullptr));
    } else {
      text_writer = xmlNewTextWriterFilename(out_file_path.c_str(), 0);
    }
    xbelmark::xml::Writer xml_writer(text_writer);
    xml_writer.StartDocument("1.0", "UTF-8", "");
    xml_writer.StartElement("xbel");
    xml_writer.WriteAttribute("version", "1.0");
    xml_writer.StartElement("bookmark");
    xml_writer.WriteAttribute("href", bookmark_uri);
    // Set the added time of the bookmark.
    {
      char dt[255];
      std::time_t t(std::time(nullptr));
      std::strftime(dt, sizeof(dt), "%FT%T%z", std::localtime(&t));
      std::string dt_str(dt);
      if (dt_str.back() != 'Z') {
        dt_str.insert(dt_str.size() - 2, 1, ':');
      }
      xml_writer.WriteAttribute("added", dt_str);
    }
    xml_writer.StartElement("title");
    xml_writer.WriteString(html_title);
    xml_writer.EndElement();
    xml_writer.EndElement();
    xml_writer.EndElement();
    xml_writer.EndDocument();
  } catch (const std::exception &e) {
    DisplayError(e.what());
    return 1;
  }
#ifdef WIN32
  try {
    xbelmark::winshell::FolderView fv(GetForegroundWindow());
    WinSelectItem(fv, file_name, TIMEOUT_MILLISECONDS);
  } catch (const std::exception &) {
  }
#endif
  return 0;
}

int Execute(int argc, char *argv[]) {
  QGuiApplication gui_app(argc, argv);
#ifdef WIN32
  CoInitialize(nullptr);
#endif
  std::unique_ptr<CmdArgs> cmd_args;
  try {
    cmd_args = CmdArgsParser().Parse(&argv[2], &argv[argc]);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  if (!cmd_args->help.empty()) {
    std::cout << cmd_args->help << std::endl;
    return 1;
  }
  QUrl url;
  if (cmd_args->uri.empty()) {
    const QClipboard &clipboard = *QGuiApplication::clipboard();
    if (!clipboard.mimeData()->hasText() || clipboard.text().isEmpty()) {
      DisplayError("No text in clipboard.");
      return 1;
    }
    url = clipboard.text();
  } else {
    url = cmd_args->uri.data();
  }
  if (!url.isValid() || url.isRelative()) {
    std::string message;
    message += "Not a valid URL:\n";
    message += url.toString().toUtf8().constData();
    DisplayError(message);
    return 1;
  }
  const std::string bookmark_uri(url.toString().toUtf8().constData());
  xbelmark::html::InfoRetriever html_info_retriever(url);
  std::string base_file_name;
  if (cmd_args->std_out) {
    base_file_name = "";
  } else {
    base_file_name = html_info_retriever.win_title_name();
    if (!cmd_args->spaces) {
      base_file_name =
          std::regex_replace(base_file_name, std::regex("\\s"), "_");
    }
  }
  int exit_status;
  switch (cmd_args->format) {
    case Format::URL: {
      exit_status = PasteUrl(base_file_name, bookmark_uri);
      break;
    }
    case Format::XBEL: {
      exit_status =
          PasteXbel(base_file_name, html_info_retriever.title(), bookmark_uri);
      break;
    }
    default: {
      std::cerr <<
          "Internal error: enumeration is not exhaustive." <<
          std::endl;
      exit_status = 1;
      break;
    }
  }
#ifdef WIN32
  CoUninitialize();
#endif
  return exit_status;
}

} // namespace paste
} // namespace xbelmark
