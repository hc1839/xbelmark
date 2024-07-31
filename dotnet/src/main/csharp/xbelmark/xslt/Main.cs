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
    /// Name and value of each parameter as `[name]=[value]`.
    /// </param>
    public static void Execute(
        string stylesheetPath, string inputDocPath, List<string> paramList)
    {
        var parameters = new Dictionary<string, string>();
        foreach (string item in paramList) {
            string[] param = item.Split("=", 2);
            if (param.Length != 2) {
                throw new ArgumentException(
                    "Parameter is not in the form `[name]=[value]`.");
            }
            parameters.Add(param[0], param[1]);
        }
        var xslt = new XslCompiledTransform();
        xslt.Load(stylesheetPath);
        var xslArg = new XsltArgumentList();
        foreach (KeyValuePair<string, string> item in parameters) {
            xslArg.AddParam(item.Key, "", item.Value);
        }
        xslArg.AddExtensionObject(
            "xalan://io.github.hc1839.xbelmark.xslt.ext.DateTime",
            new Ext.DateTime());
        var transformBuilder = new StringBuilder();
        using (XmlWriter xmlWriter = XmlWriter.Create(transformBuilder)) {
            xslt.Transform(inputDocPath, xslArg, xmlWriter);
        }
        Console.WriteLine(transformBuilder.ToString());
    }
}
