namespace Xbelmark.Paste;

/// <summary>
/// Formats of the output file from the `paste` subcommand.
/// </summary>
public enum Format : int
{
    /// <summary>
    /// URL format used in Windows.
    /// </summary>
    URL,

    /// <summary>
    /// XBEL format.
    /// </summary>
    XBEL
}
