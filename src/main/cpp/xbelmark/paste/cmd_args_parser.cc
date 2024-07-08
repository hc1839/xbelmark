#include "xbelmark/paste/cmd_args_parser.h"

#include <stdexcept>
#include <utility>

#define SUBCOMMAND_NAME "paste"

namespace xbelmark {
namespace paste {

using xbelmark::enumeration::EnumValueOf;

class CmdArgsParser::Impl final {
 public:
  /**
   *  Reset the parser.
   */
  void Reset() {
    cmd_args_.reset(new CmdArgs());
    arg_it_ = nullptr;
    arg_last_ = nullptr;
    pos_arg_idx_ = -1;
  }

  /**
   *  Set the help message that can be printed.
   */
  void SetHelpMessage() {
    ++arg_it_;
    std::string &help = cmd_args_->help;
    if (!help.empty()) {
      return;
    }
    help = help +
        "Usage: " +
        cmd_args_->command_name + " " + cmd_args_->subcommand_name +
        " [options]\n\n" +
        "Paste a URL as a bookmark file.\n\n";
    help = help +
        "  --format, -f <format>\n" +
        "\n" +
        "      Format of the output file. Valid values are `URL` (URL\n" +
        "      format used in Windows) and `XBEL` (XBEL format). If not\n" +
        "      specified, the format is `XBEL`.\n\n";
    help = help +
        "  --uri <uri>\n" +
        "\n" +
        "      URI of the resource specified by the bookmark. If not\n" +
        "      specified, the clipboard text is used.\n\n";
    help = help +
        "  --stdout\n" +
        "\n" +
        "      Write the bookmark to the standard output.\n\n";
    help = help +
        "  --help, -h\n" +
        "\n" +
        "      Print help.";
  }

  /**
   *  Set format.
   */
  void SetFormat() {
    ++arg_it_;
    if (arg_it_ == arg_last_) {
      throw std::runtime_error("Insufficient arguments for `--format`.");
    }
    cmd_args_->format = EnumValueOf<Format>(*arg_it_++);
  }

  /**
   *  Set URI.
   */
  void SetUri() {
    ++arg_it_;
    if (arg_it_ == arg_last_) {
      throw std::runtime_error("Insufficient arguments for `--uri`.");
    }
    cmd_args_->uri = *arg_it_++;
  }

  /**
   *  Set that the bookmark is written to the standard output.
   */
  void SetStdOut() {
    ++arg_it_;
    cmd_args_->std_out = true;
  }

  /**
   *  Parsed command-line arguments.
   */
  std::unique_ptr<CmdArgs> cmd_args_;

  /**
   *  Pointer to the current command-line argument.
   */
  char **arg_it_;

  /**
   *  Pointer to past-the-last command-line argument.
   */
  char **arg_last_;

  /**
   *  Zero-based index of the current positional command-line argument.
   *
   *  It is `-1` if the current command-line argument is not positional.
   */
  int pos_arg_idx_;
};

CmdArgsParser::CmdArgsParser() : p_impl_(new Impl()) {
}

CmdArgsParser::~CmdArgsParser() = default;

std::unique_ptr<CmdArgs> CmdArgsParser::Parse(char **first, char **last) {
  p_impl_->Reset();
  p_impl_->cmd_args_->subcommand_name = SUBCOMMAND_NAME;
  p_impl_->arg_it_ = first;
  p_impl_->arg_last_ = last;
  // Parse the command-line arguments.
  while (p_impl_->arg_it_ != p_impl_->arg_last_) {
    if (p_impl_->pos_arg_idx_ == -1) {
      const std::string opt(*p_impl_->arg_it_);
      if (opt == "--help" || opt == "-h") {
        p_impl_->SetHelpMessage();
      } else if (opt == "--format" || opt == "-f") {
        p_impl_->SetFormat();
      } else if (opt == "--uri") {
        p_impl_->SetUri();
      } else if (opt == "--stdout") {
        p_impl_->SetStdOut();
      } else if (opt.front() == '-') {
        throw std::runtime_error("Unrecognized option: " + opt);
      } else {
        ++p_impl_->pos_arg_idx_;
      }
    } else {
      const std::string arg(*p_impl_->arg_it_);
      throw std::runtime_error("Unrecognized positional argument: " + arg);
    }
  }
  return std::move(p_impl_->cmd_args_);
}

} // namespace paste
} // namespace xbelmark
