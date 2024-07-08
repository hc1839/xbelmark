using System.Windows;
using System;

namespace Xbelmark;

public static class ShowMessage
{
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
