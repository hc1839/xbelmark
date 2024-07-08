#include "xbelmark/xslt/cmd_args_parser.h"

#include <stdexcept>
#include <utility>

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
   *  Append the `param` option to the AST.
   */
  void AppendParam() {
    ++arg_it_;
    if (arg_last_ - arg_it_ < 2) {
      throw std::runtime_error("Insufficient arguments for `--param`.");
    }
    const std::string name(*arg_it_++);
    const std::string value(*arg_it_++);
    cmd_args_->xslt_params[name] = value;
  }

  /**
   *  Append the `stringparam` option to the AST.
   */
  void AppendStringParam() {
    ++arg_it_;
    if (arg_last_ - arg_it_ < 2) {
      throw std::runtime_error("Insufficient arguments for `--stringparam`.");
    }
    const std::string name(*arg_it_++);
    const std::string value(*arg_it_++);
    cmd_args_->xslt_params[name] = "'" + value + "'";
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
        " [options] stylesheet input-doc\n\n" +
        "Transform XBEL into XHTML5.\n\n";
    help = help +
        "  --param <name> <value>\n" +
        "\n" +
        "      Name and value of a parameter. <name> is a QName or a string\n" +
        "      of the form {URI}NCName. <value> is an XPath expression.\n" +
        "      String values must be quoted like \"'string'\", or use\n" +
        "      `--stringparam` to avoid the quoting.\n\n";
    help = help +
        "  --stringparam <name> <value>\n" +
        "\n" +
        "      Name and string value of a parameter.\n\n";
    help = help +
        "  --help, -h\n" +
        "\n" +
        "      Print help.";
  }

  /**
   *  Set the path to the XSL stylesheet.
   */
  void SetStylesheetPath() {
    cmd_args_->stylesheet_path = *arg_it_++;
    ++pos_arg_idx_;
  }

  /**
   *  Set the path to the input XML document.
   */
  void SetInputDocPath() {
    cmd_args_->input_doc_path = *arg_it_++;
    ++pos_arg_idx_;
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
      } else if (opt == "--param") {
        p_impl_->AppendParam();
      } else if (opt == "--stringparam") {
        p_impl_->AppendStringParam();
      } else if (opt.front() == '-') {
        throw std::runtime_error("Unrecognized option: " + opt);
      } else {
        ++p_impl_->pos_arg_idx_;
      }
    } else {
      switch (p_impl_->pos_arg_idx_) {
        case 0: {
          p_impl_->SetStylesheetPath();
          break;
        }
        case 1: {
          p_impl_->SetInputDocPath();
          break;
        }
        default: {
          const std::string arg(*p_impl_->arg_it_);
          throw std::runtime_error("Unrecognized positional argument: " + arg);
          break;
        }
      }
    }
  }
  // Ensure the paths to the XSL stylesheet and input XML document are set.
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
