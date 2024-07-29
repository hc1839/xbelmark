#ifndef XBELMARK_PASTE_CMD_ARGS_H
#define XBELMARK_PASTE_CMD_ARGS_H

#include <string>

#include "xbelmark/cmd_args.h"
#include "xbelmark/paste/format.h"

namespace xbelmark {
namespace paste {

/**
 *  Command-line arguments for the `paste` subcommand.
 */
struct CmdArgs : public xbelmark::CmdArgs {
 public:
  /**
   *  Format of the output file.
   */
  Format format = Format::XBEL;

  /**
   *  URI of the resource specified by the bookmark, or an empty string if none
   *  was specified.
   */
  std::string uri;

  /**
   *  Whether the spaces in a file name are preserved.
   */
  bool spaces = false;

  /**
   *  Whether the bookmark is written to the standard output.
   */
  bool std_out = false;
};

} // namespace paste
} // namespace xbelmark

#endif
