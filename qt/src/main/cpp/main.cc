#include <iostream>
#include <string>

#include "xbelmark/paste/paste.h"
#include "xbelmark/xslt/xslt.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "No subcommand specified." << std::endl;
    return 1;
  }
  const std::string subcommand(argv[1]);
  if (subcommand == "--help" || subcommand == "-h") {
    if (argc == 2) {
      std::cout << "Available subcommands:" << std::endl;
      std::cout << "  paste" << std::endl;
      std::cout << "  xslt" << std::endl;
      std::cout << "Type `xbelmark [subcommand] --help`";
      std::cout << " for help on a subcommand." << std::endl;
      return 1;
    } else {
      std::string help_opt("--help");
      char *args[] = { argv[0], argv[2], help_opt.data() };
      return main(3, args);
    }
  } else if (subcommand == "paste") {
    return xbelmark::paste::Execute(argc, argv);
  } else if (subcommand == "xslt") {
    return xbelmark::xslt::Execute(argc, argv);
  } else {
    std::cerr << ("Invalid subcommand: `" + subcommand + "`.") << std::endl;
    return 1;
  }
}
