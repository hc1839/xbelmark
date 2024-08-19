using System.Globalization;
using System;

namespace Xbelmark.Xslt.Ext;

/// <summary>
/// Conversions of date/time formats.
/// </summary>
public class DateTime
{
    /// <summary>
    /// Converts `xs:dateTime` or `xs:date` to seconds since epoch.
    /// </summary>
    /// <param name="input">
    /// Date/time string in `xs:dateTime` or `xs:date` format.
    /// </param>
    /// <returns>
    /// Seconds since epoch.
    /// </returns>
    public long dateTimeToUnix(string input)
    {
        DateTimeOffset dto;
        try
        {
            dto = DateTimeOffset.ParseExact(
                input, "O", null, DateTimeStyles.RoundtripKind);
        }
        catch (Exception)
        {
            dto = DateTimeOffset.ParseExact(
                input, "yyyy'-'MM'-'dd'T'HH':'mm':'sszzz", null);
        }
        return dto.ToUnixTimeSeconds();
    }
}
