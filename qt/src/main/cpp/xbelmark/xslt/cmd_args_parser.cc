#include "xbelmark/xslt/cmd_args_parser.h"

#include <regex>
#include <stdexcept>
#include <utility>
#include <vector>

#define SUBCOMMAND_NAME "xslt"

namespace xbelmark {
namespace xslt {

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
        "Transform XBEL into XHTML5.\n\n";
    help = help +
        "  --xsl [xsl]\n" +
        "\n" +
        "      Path to the XSL stylesheet.\n\n";
    help = help +
        "  --in [in]\n" +
        "\n" +
        "      Path to the input document.\n\n";
    help = help +
        "  --param [name] [value]\n" +
        "\n" +
        "      Name and value of a parameter.\n\n";
    help = help +
        "  --help, -h\n" +
        "\n" +
        "      Print help.";
  }

  /**
   *  Set the path to the XSL stylesheet.
   */
  void SetStylesheetPath() {
    ++arg_it_;
    cmd_args_->stylesheet_path = *arg_it_++;
  }

  /**
   *  Set the path to the input document.
   */
  void SetInputDocPath() {
    ++arg_it_;
    cmd_args_->input_doc_path = *arg_it_++;
  }

  /**
   *  Append the `param` option to the AST.
   */
  void AppendParam() {
    ++arg_it_;
    if (arg_last_ - arg_it_ < 2) {
      throw std::runtime_error("Insufficient arguments for `--param`.");
    }
    const std::string name(*arg_it_++);
    const std::string value(*arg_it_++);
    cmd_args_->xslt_params[name] = "'" + value + "'";
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
      } else if (opt == "--xsl") {
        p_impl_->SetStylesheetPath();
      } else if (opt == "--in") {
        p_impl_->SetInputDocPath();
      } else if (opt == "--param") {
        p_impl_->AppendParam();
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
  // Ensure the paths to the XSL stylesheet and input document are set.
  if (p_impl_->cmd_args_->help.empty()) {
    if (p_impl_->cmd_args_->stylesheet_path.empty()) {
      throw std::invalid_argument("Path to stylesheet is not provided.");
    }
    if (p_impl_->cmd_args_->input_doc_path.empty()) {
      throw std::invalid_argument("Path to input document is not provided.");
    }
  }
  return std::move(p_impl_->cmd_args_);
}

} // namespace xslt
} // namespace xbelmark
