using System.IO;
using System.Text.RegularExpressions;
using System;

namespace Xbelmark.Paste;

/// <summary>
/// Container class for the `paste` subcommand.
/// </summary>
public static class Main
{
    /// <summary>
    /// Executes the `paste` subcommand.
    /// </summary>
    /// <param name="baseFileName">
    /// File name without the extension of the output file, or empty string for
    /// the standard output.
    /// </param>
    /// <param name="format">
    /// Format of the bookmark.
    /// </param>
    /// <param name="uri">
    /// URI of the bookmark.
    /// </param>
    /// <param name="htmlTitle">
    /// HTML title of the web page.
    /// </param>
    public static void Execute(
        string baseFileName, Format format, Uri uri, string htmlTitle)
    {
        string bookmarkText = format.Text(uri, htmlTitle);
        if (baseFileName.Length != 0)
        {
            string bookmarkFileName =
                baseFileName + "." + format.ToString().ToLower();
            string bookmarkFilePath = Path.Combine(
                Directory.GetCurrentDirectory(), bookmarkFileName);
            if (File.Exists(bookmarkFilePath))
            {
                ShowMessage.Error(
                    "Bookmark with the same file name exists:\n" +
                    bookmarkFilePath);
            }
            using (var outputFile = new StreamWriter(bookmarkFilePath))
            {
                outputFile.WriteLine(bookmarkText);
            }
        }
        else
        {
            Console.WriteLine(bookmarkText);
        }
    }
}
