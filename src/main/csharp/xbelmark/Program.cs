using System.Diagnostics;
using System.Windows;
using System;

using System.CommandLine;

using static Xbelmark.Paste.FormatExt;

namespace Xbelmark;

public class Program
{
    [STAThread]
    public static int Main(string[] args)
    {
        var rootCommand = new RootCommand(
            "XBELmark is a program suite for facilitating the use and " +
            "exchange of Internet bookmarks as XBEL outside a web browser.");
        var pasteCommand = new Command(
            "paste",
            "Paste a URL as a bookmark file.");
        rootCommand.AddCommand(pasteCommand);
        var formatOption = new Option<string>(
            name: "--format",
            getDefaultValue: () => "XBEL",
            description:
                "Format of the output file. Valid values are `URL` (URL " +
                "format used in Windows) and `XBEL` (XBEL format). If not " +
                "specified, the format is `XBEL`.");
        pasteCommand.AddOption(formatOption);
        formatOption.AddAlias("-f");
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
        var stdoutOption = new Option<bool>(
            name: "--stdout",
            description: "Write the bookmark to the standard output.");
        pasteCommand.AddOption(stdoutOption);
        int exitStatus = 0;
        pasteCommand.SetHandler((format, uri, isStdout) =>
            {
                try
                {
                    Paste.Main.Execute(
                        Enum.Parse<Paste.Format>(format),
                        new Uri(uri),
                        isStdout);
                }
                catch (Exception e)
                {
                    ShowMessage.Error(e.Message);
                    exitStatus = 1;
                }
            },
            formatOption, uriOption, stdoutOption);
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
}
