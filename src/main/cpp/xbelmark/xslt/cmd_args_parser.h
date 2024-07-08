#ifndef XBELMARK_XSLT_CMD_ARGS_PARSER_H
#define XBELMARK_XSLT_CMD_ARGS_PARSER_H

#include <memory>

#include "xbelmark/xslt/cmd_args.h"

namespace xbelmark {
namespace xslt {

/**
 *  Parser of command-line arguments for the `xslt` subcommand.
 */
class CmdArgsParser final {
 public:
  CmdArgsParser();

  ~CmdArgsParser();

  /**
   *  Parse command-line arguments.
   *
   *  @param first
   *    Pointer to the first command-line argument.
   *
   *  @param last
   *    Pointer to past-the-last command-line argument.
   */
  std::unique_ptr<CmdArgs> Parse(char **first, char **last);

 private:
  class Impl;

  std::unique_ptr<Impl> p_impl_;
};

} // namespace xslt
} // namespace xbelmark

#endif
