package io.github.hc1839.xbelmark;

import javax.swing.JOptionPane;

/**
 *  Shows a message in various forms.
 */
public class ShowMessage {
  /**
   *  Shows an error message as a dialog box or, if GUI is not available, as standard error.
   *
   *  @param message
   *    Message to show.
   */
  public static void error(String message) {
    try {
      JOptionPane.showMessageDialog(null, message, "Error", JOptionPane.ERROR_MESSAGE);
    } catch (Exception e) {
      System.err.println(message);
    }
  }

  /**
   *  Shows an error message as a dialog box or, if GUI is not available, as standard error.
   *
   *  @param message
   *    Message to show.
   *
   *  @param caption
   *    Caption of the dialog box if the message is displayed in one.
   */
  public static void error(String message, String caption) {
    try {
      JOptionPane.showMessageDialog(null, message, caption, JOptionPane.ERROR_MESSAGE);
    } catch (Exception e) {
      System.err.println(message);
    }
  }
}
