#ifndef XBELMARK_PASTE_CMD_ARGS_PARSER_H
#define XBELMARK_PASTE_CMD_ARGS_PARSER_H

#include <memory>

#include "xbelmark/paste/cmd_args.h"

namespace xbelmark {
namespace paste {

/**
 *  Parser of command-line arguments for the `paste` subcommand.
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

} // namespace paste
} // namespace xbelmark

#endif
