package io.github.hc1839.xbelmark;

import java.util.Arrays;

import io.github.hc1839.xbelmark.paste.Paste;
import io.github.hc1839.xbelmark.xslt.Xslt;

public class Main {
  public static void main(String[] args) {
    if (args.length < 1) {
      System.err.println("No subcommand specified.");
      return;
    }
    String subcommand = args[0];
    switch (subcommand) {
      case "--help", "-h": {
        if (args.length == 1) {
          System.out.println("Available subcommands:");
          System.out.println("  paste");
          System.out.println("  xslt");
          System.out.println(
              "Type `xbelmark [subcommand] --help` for help on a subcommand.");
        } else {
          main(new String[] { args[1], "--help" });
        }
        break;
      }
      case "paste": {
        Paste.Execute(Arrays.copyOfRange(args, 1, args.length));
        break;
      }
      case "xslt": {
        Xslt.Execute(Arrays.copyOfRange(args, 1, args.length));
        break;
      }
      default: {
        System.err.println("Invalid subcommand: `" + subcommand + "`.");
        break;
      }
    }
  }
}
