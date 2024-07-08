using System.Xml;
using System;

namespace Xbelmark.Paste;

public static class FormatExt
{
    private static string UrlText(Uri uri, string htmlTitle) =>
        "[InternetShortcut]\n" + "URL=" + uri.ToString();

    private static string XbelText(Uri uri, string htmlTitle)
    {
        var xbel = new XmlDocument();
        xbel.AppendChild(xbel.CreateXmlDeclaration("1.0", null, null));
        XmlElement xbelRoot = xbel.CreateElement("xbel");
        xbel.AppendChild(xbelRoot);
        xbelRoot.SetAttribute("version", "1.0");
        XmlElement bookmark = xbel.CreateElement("bookmark");
        xbelRoot.AppendChild(bookmark);
        bookmark.SetAttribute("href", uri.ToString());
        XmlElement title = xbel.CreateElement("title");
        bookmark.AppendChild(title);
        title.InnerText = htmlTitle;
        return xbel.OuterXml;
    }

    public static string Text(this Format format, Uri uri, string htmlTitle) =>
        format switch
        {
            Format.URL => UrlText(uri, htmlTitle),
            Format.XBEL => XbelText(uri, htmlTitle),
            _ => throw new ArgumentOutOfRangeException(
                nameof(format),
                $"Not expected enumeration value: `{format}`.")
        };
}
