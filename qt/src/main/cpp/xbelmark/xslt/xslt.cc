#include "xbelmark/xslt/xslt.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

#include <libxslt/extensions.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include "xbelmark/xslt/cmd_args.h"
#include "xbelmark/xslt/cmd_args_parser.h"
#include "xbelmark/xslt/ext/date_time.h"

using xbelmark::xslt::ext::DateTime;

namespace xbelmark {
namespace xslt {

int Execute(int argc, char *argv[]) {
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

  int status = xsltRegisterExtModule(
      DateTime::NamespaceUri(), DateTime::InitFunction, nullptr);

  if (status != 0) {
    std::cerr << "Failed to register the extension module." << std::endl;
    return 1;
  }

  std::vector<const char *> xslt_params;
  for (const auto &item : cmd_args->xslt_params) {
    xslt_params.push_back(item.first.c_str());
    xslt_params.push_back(item.second.c_str());
  }
  xslt_params.push_back(nullptr);

  xsltStylesheetPtr stylesheet = xsltParseStylesheetFile(
      reinterpret_cast<const xmlChar *>(cmd_args->stylesheet_path.c_str()));
  xmlDocPtr input_doc = xmlParseFile(cmd_args->input_doc_path.c_str());
  xmlDocPtr output_doc =
      xsltApplyStylesheet(stylesheet, input_doc, xslt_params.data());
  xsltSaveResultToFile(stdout, output_doc, stylesheet);

  xmlFreeDoc(output_doc);
  xmlFreeDoc(input_doc);
  xsltFreeStylesheet(stylesheet);

  xsltCleanupGlobals();
  xmlCleanupParser();

  return 0;
}

} // namespace xslt
} // namespace xbelmark
