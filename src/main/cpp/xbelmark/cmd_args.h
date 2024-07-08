#ifndef XBELMARK_CMD_ARGS_H
#define XBELMARK_CMD_ARGS_H

#include <map>
#include <string>

namespace xbelmark {

/**
 *  Command-line arguments for the base command.
 */
struct CmdArgs {
 public:
  /**
   *  @param command_name
   *    Name of the command.
   */
  std::string command_name = "xbelmark";

  /**
   *  @param subcommand_name
   *    Name of the subcommand.
   */
  std::string subcommand_name;

  /**
   *  Help message that can be printed.
   */
  std::string help;
};

} // namespace xbelmark

#endif
