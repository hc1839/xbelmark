using System.IO;
using System.Text.RegularExpressions;
using System;

using HtmlAgilityPack;

namespace Xbelmark.Paste;

public static class Main
{
#if WINDOWS_NT
    public const string RESERVED_RE = "<|>|:|\"|/|\\\\|\\||\\?|\\*";
#else
    public const string RESERVED_RE = "<|>|:|\"|/|\\\\|\\||\\?|\\*|\\s";
#endif

    public static void Execute(Format format, Uri uri, bool isStdout)
    {
        HtmlDocument html = new HtmlWeb().Load(uri);
        HtmlNode titleNode = html.DocumentNode.SelectSingleNode("//title");
        string title = titleNode.InnerText.Trim();
        string bookmarkText = format.Text(uri, title);
        if (isStdout)
        {
            Console.WriteLine(bookmarkText);
        }
        else
        {
            string bookmarkFileName =
                title.Length != 0 ? title : uri.ToString();
            bookmarkFileName = Regex.Replace(bookmarkFileName, "\\s+", " ");
            bookmarkFileName =
                Regex.Replace(bookmarkFileName, RESERVED_RE, "_");
            bookmarkFileName += "." + format.ToString().ToLower();
            string bookmarkFilePath = Path.Combine(
                Directory.GetCurrentDirectory(), bookmarkFileName);
            using (var outputFile = new StreamWriter(bookmarkFilePath))
            {
                outputFile.WriteLine(bookmarkText);
            }
        }
    }
}
