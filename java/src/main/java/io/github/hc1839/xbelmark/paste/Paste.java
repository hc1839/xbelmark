package io.github.hc1839.xbelmark.paste;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringWriter;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.jsoup.Jsoup;

import io.github.hc1839.xbelmark.ShowMessage;

/**
 *  Container class for the `paste` subcommand.
 */
public class Paste {
  /**
   *  Help information.
   */
  private static class Help {
    public static final String Syntax = "xbelmark paste [options]";

    public static final String Header = "Paste a URL as a bookmark file.";

    public static final String Footer = "";
  }

  /**
   *  Reserved characters of a file name in Windows.
   */
  public static final Pattern RESERVED_RE = Pattern.compile("<|>|:|\"|/|\\\\|\\||\\?|\\*");

  /**
   *  Default bookmark format.
   */
  public static final Format DEFAULT_FORMAT = Format.XBEL;

  /**
   *  Command-line options.
   */
  private static final Options cliOptions;

  static {
    cliOptions = new Options();

    Option helpOption = Option
        .builder("h")
        .longOpt("help")
        .desc("Print help.")
        .hasArg(false)
        .build();

    cliOptions.addOption(helpOption);

    Option formatOption = Option
        .builder()
        .longOpt("format")
        .desc(
            "Format of the output file. Valid values are `URL` (URL format used in Windows) and "
            + "`XBEL` (XBEL format). If not specified, the format is `XBEL`.")
        .numberOfArgs(1)
        .build();

    cliOptions.addOption(formatOption);

    Option uriOption = Option
        .builder()
        .longOpt("uri")
        .desc(
            "URI of the resource specified by the bookmark. If not specified, the clipboard text "
            + "is used.")
        .numberOfArgs(1)
        .build();

    cliOptions.addOption(uriOption);

    Option spacesOption = Option
        .builder()
        .longOpt("spaces")
        .desc(
            "Spaces in a file name are preserved. If not set, each space is replaced with an "
            + "underscore. It is ignored if `--stdout` is set.")
        .hasArg(false)
        .build();

    cliOptions.addOption(spacesOption);

    Option stdoutOption = Option
        .builder()
        .longOpt("stdout")
        .desc("Write the bookmark to the standard output.")
        .hasArg(false)
        .build();

    cliOptions.addOption(stdoutOption);
  }

  /**
   *  Prints the help.
   */
  public static void printHelp() {
    new HelpFormatter().printHelp(Help.Syntax, Help.Header, cliOptions, Help.Footer, true);
  }

  /**
   *  Executes the `paste` subcommand.
   *
   *  @param args
   *    Command-line arguments.
   */
  public static void Execute(String[] args) {
    // Parse the command-line options.
    CommandLine cmdline;
    try {
      cmdline = new DefaultParser().parse(cliOptions, args);
    } catch (Exception e) {
      printHelp();
      return;
    }

    // Determine whether to print the help and exit.
    boolean isHelp = Arrays.stream(cmdline.getOptions()).anyMatch(
        item -> "help".equals(item.getLongOpt()) || "h".equals(item.getOpt()));

    if (isHelp) {
      printHelp();
      return;
    }

    boolean hasFormatOption = Arrays.stream(cmdline.getOptions()).anyMatch(
        item -> "format".equals(item.getLongOpt()));

    Format bookmarkFormat =
        hasFormatOption ? Format.valueOf(cmdline.getOptionValue("format")) : DEFAULT_FORMAT;

    String bookmarkUriString = cmdline.getOptionValue("uri");

    // Get the URL from the clipboard if one was not specified on the command line.
    if (bookmarkUriString == null) {
      try {
        Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
        bookmarkUriString = clipboard.getData(DataFlavor.stringFlavor).toString().strip();
      } catch (UnsupportedFlavorException e) {
        ShowMessage.error("No string in clipboard.");
        return;
      } catch (IOException e) {
        ShowMessage.error("Cannot retrieve string in clipboard.");
        return;
      }
    }

    URI bookmarkUri;
    try {
      bookmarkUri = new URI(bookmarkUriString);
    } catch (URISyntaxException e) {
      ShowMessage.error("Not a valid URI: " + bookmarkUriString);
      return;
    }

    String htmlTitle;
    try {
      htmlTitle = Jsoup.connect(bookmarkUriString).get().title().strip();
    } catch (IOException e) {
      htmlTitle = bookmarkUriString;
    }

    String bookmarkText = bookmarkFormat.text(bookmarkUri, htmlTitle);

    boolean isStdout = Arrays.stream(cmdline.getOptions()).anyMatch(
        item -> "stdout".equals(item.getLongOpt()));

    // Output the bookmark.
    if (isStdout) {
      System.out.println(bookmarkText);
    } else {
      boolean isSpaces = Arrays.stream(cmdline.getOptions()).anyMatch(
          item -> "spaces".equals(item.getLongOpt()));

      // Create the file name of the bookmark.
      String bookmarkFileName = htmlTitle;
      bookmarkFileName = bookmarkFileName.replaceAll("\\s+", isSpaces ? " " : "_");
      bookmarkFileName = RESERVED_RE.matcher(bookmarkFileName).replaceAll("_");
      bookmarkFileName += bookmarkFormat.name().toLowerCase();
      Path bookmarkFilePath = Paths.get("").toAbsolutePath().resolve(bookmarkFileName);
      try (var fileWriter = new FileWriter(bookmarkFilePath.toFile())) {
        fileWriter.write(bookmarkText);
        fileWriter.write("\n");
      } catch (IOException e) {
        ShowMessage.error(e.getMessage());
        return;
      }
    }
  }
}
