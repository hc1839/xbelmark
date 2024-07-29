package io.github.hc1839.xbelmark.xslt;

import java.util.Arrays;

import org.apache.xalan.xslt.Process;

/**
 *  Container class for the `xslt` subcommand.
 */
public class Xslt {
  /**
   *  Executes the `xslt` subcommand.
   *
   *  @param args
   *    Command-line arguments.
   */
  public static void Execute(String[] args) {
    if (args.length == 1 && "--help".equals(args[0])) {
      Process.main(new String[0]);
    } else {
      Process.main(args);
    }
  }
}
