using System.Windows;
using System;

namespace Xbelmark;

/// <summary>
/// Shows a message in various forms.
/// </summary>
public static class ShowMessage
{
    /// <summary>
    /// Shows an error message as a dialog box in Windows, or prints an error
    /// message to the standard error in other operating systems.
    /// </summary>
    /// <param name="message">
    /// Message to show.
    /// </param>
    /// <param name="caption">
    /// Caption of the dialog box if the message is displayed in one.
    /// </param>
    public static void Error(string message, string caption = "Error")
    {
#if WINDOWS_NT
        MessageBox.Show(
            message, caption, MessageBoxButton.OK, MessageBoxImage.Error);
#else
        Console.Error.WriteLine(message);
#endif
    }
}
