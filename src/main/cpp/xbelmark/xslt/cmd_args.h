#ifndef XBELMARK_XSLT_CMD_ARGS_H
#define XBELMARK_XSLT_CMD_ARGS_H

#include <map>
#include <string>

#include "xbelmark/cmd_args.h"

namespace xbelmark {
namespace xslt {

/**
 *  Command-line arguments for the `xslt` subcommand.
 */
struct CmdArgs : public xbelmark::CmdArgs {
 public:
  /**
   *  Names and values of the XSLT parameters.
   */
  std::map<std::string, std::string> xslt_params;

  /**
   *  Path to the XSL stylesheet.
   */
  std::string stylesheet_path;

  /**
   *  Path to the input XML document.
   */
  std::string input_doc_path;
};

} // namespace xslt
} // namespace xbelmark

#endif
