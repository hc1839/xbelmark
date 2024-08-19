using System.Collections.Generic;
using System.Text;
using System.Xml.Xsl;
using System.Xml;
using System;

namespace Xbelmark.Xslt;

/// <summary>
/// Container class for the `xslt` subcommand.
/// </summary>
public static class Main
{
    /// <summary>
    /// Executes the `xslt` subcommand.
    /// </summary>
    /// <param name="stylesheetPath">
    /// Path to the XSL stylesheet.
    /// </param>
    /// <param name="inputDocPath">
    /// Path to the input document.
    /// </param>
    /// <param name="paramList">
    /// Name and value of each parameter as alternating elements.
    /// </param>
    public static void Execute(
        string stylesheetPath, string inputDocPath, List<string> paramList)
    {
        var parameters = new Dictionary<string, string>();
        if (paramList.Count % 2 != 0)
        {
            throw new ArgumentException(
                "Name and value of each parameter are not as alternating " +
                "elements.");
        }
        for (int i = 0; i != paramList.Count; i += 2)
        {
            parameters.Add(paramList[i], paramList[i + 1]);
        }
        var xslt = new XslCompiledTransform();
        xslt.Load(stylesheetPath);
        var xslArg = new XsltArgumentList();
        foreach (KeyValuePair<string, string> item in parameters)
        {
            xslArg.AddParam(item.Key, "", item.Value);
        }
        xslArg.AddExtensionObject(
            "xalan://io.github.hc1839.xbelmark.xslt.ext.DateTime",
            new Ext.DateTime());
        var transformBuilder = new StringBuilder();
        using (XmlWriter xmlWriter = XmlWriter.Create(transformBuilder))
        {
            xslt.Transform(inputDocPath, xslArg, xmlWriter);
        }
        Console.WriteLine(transformBuilder.ToString());
    }
}
