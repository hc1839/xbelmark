using System.Collections.Generic;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Windows;
using System;

using HtmlAgilityPack;
using System.CommandLine;

using static Xbelmark.Paste.FormatExt;

namespace Xbelmark;

/// <summary>
/// Main program.
/// </summary>
public class Program
{
    /// <summary>
    /// Main method.
    /// </summary>
    /// <param name="args">
    /// Command-line arguments.
    /// </param>
    [STAThread]
    public static int Main(string[] args)
    {
        int exitStatus = 0;

        var rootCommand = new RootCommand(
            "XBELmark is a program suite for facilitating the use and " +
            "exchange of Internet bookmarks as XBEL.");

        var pasteCommand = new Command(
            "paste", "Paste a URL as a bookmark file.");
        rootCommand.AddCommand(pasteCommand);
        var formatOption = new Option<string>(
            name: "--format",
            getDefaultValue: () => "XBEL",
            description:
                "Format of the output file. Valid values are `URL` (URL " +
                "format used in Windows) and `XBEL` (XBEL format).");
        pasteCommand.AddOption(formatOption);
        var uriOption = new Option<string>(
            name: "--uri",
            description:
                "URI of the resource specified by the bookmark."
#if WINDOWS_NT
                + " If not specified, the clipboard text is used.",
            getDefaultValue: () => Clipboard.GetText(TextDataFormat.Text)
#endif
        );
        pasteCommand.AddOption(uriOption);
        var spacesOption = new Option<bool>(
            name: "--spaces",
            description:
                "Spaces in a file name are preserved. If not set, each " +
                "space is replaced with an underscore. It is ignored if " +
                "`--stdout` is set.");
        pasteCommand.AddOption(spacesOption);
        var stdoutOption = new Option<bool>(
            name: "--stdout",
            description: "Write the bookmark to the standard output.");
        pasteCommand.AddOption(stdoutOption);
        pasteCommand.SetHandler((format, uri, isSpaces, isStdout) =>
            {
                try
                {
                    // Get the HTML title.
                    HtmlDocument html = new HtmlWeb().Load(uri);
                    HtmlNode titleNode =
                        html.DocumentNode.SelectSingleNode("//title");
                    string htmlTitle = titleNode.InnerText.Trim();

                    // Create the base file name of the bookmark.
                    string baseFileName = "";
                    if (!isStdout) {
                        baseFileName =
                            htmlTitle.Length != 0 ? htmlTitle : uri.ToString();
                        baseFileName = Regex.Replace(
                            baseFileName, "\\s+", isSpaces ? " " : "_");
                        baseFileName =
                            Regex.Replace(baseFileName, RESERVED_RE, "_");
                    }
                    Paste.Main.Execute(
                        baseFileName,
                        Enum.Parse<Paste.Format>(format),
                        new Uri(uri),
                        htmlTitle);
                }
                catch (Exception e)
                {
                    ShowMessage.Error(e.Message);
                    exitStatus = 1;
                }
            },
            formatOption, uriOption, spacesOption, stdoutOption);

        var xsltCommand = new Command("xslt", "Transform XBEL into XHTML5.");
        rootCommand.AddCommand(xsltCommand);
        var xslOption = new Option<string>(
            name: "--xsl", description: "Path to the XSL stylesheet.");
        xsltCommand.AddOption(xslOption);
        var inOption = new Option<string>(
            name: "--in", description: "Path to the input document.");
        xsltCommand.AddOption(inOption);
        var paramOption = new Option<List<string>>(
            name: "--param",
            description:
                "Name and value of each parameter as `[name]=[value]`.");
        xsltCommand.AddOption(paramOption);
        paramOption.Arity = ArgumentArity.ZeroOrMore;
        xsltCommand.SetHandler((stylesheetPath, inputDocPath, paramList) =>
            {
                try
                {
                    Xslt.Main.Execute(stylesheetPath, inputDocPath, paramList);
                }
                catch (Exception e)
                {
                    ShowMessage.Error(e.Message);
                    exitStatus = 1;
                }
            },
            xslOption, inOption, paramOption);

        try
        {
            rootCommand.Invoke(args);
        }
        catch (Exception e)
        {
            ShowMessage.Error(e.Message);
            exitStatus = 1;
        }
        return exitStatus;
    }

    /// <summary>
    /// Reserved characters of a file name in Windows.
    /// </summary>
    public const string RESERVED_RE = "<|>|:|\"|/|\\\\|\\||\\?|\\*";
}
