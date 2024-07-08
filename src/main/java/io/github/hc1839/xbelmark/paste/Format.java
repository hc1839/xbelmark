package io.github.hc1839.xbelmark.paste;

import java.io.StringWriter;
import java.net.URI;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 *  Enumeration of the available formats of the output file from the `paste`
 *  subcommand.
 */
public enum Format {
  /**
   *  URL format used in Windows.
   */
  URL {
    @Override
    public String text(URI uri, String htmlTitle) {
      return "[InternetShortcut]\n" + "URL=" + uri.toString();
    }
  },

  /**
   *  XBEL format.
   */
  XBEL {
    @Override
    public String text(URI uri, String htmlTitle) {
      Document xbel;
      try {
        xbel = DocumentBuilderFactory
            .newInstance().newDocumentBuilder().newDocument();
      } catch (Exception e) {
        throw new RuntimeException(
            "Cannot create the builder for the XBEL document.");
      }
      Element xbelRoot = xbel.createElement("xbel");
      xbel.appendChild(xbelRoot);
      xbelRoot.setAttribute("version", "1.0");
      Element bookmark = xbel.createElement("bookmark");
      xbelRoot.appendChild(bookmark);
      bookmark.setAttribute("href", uri.toString());
      bookmark.setAttribute(
          "added",
          DateTimeFormatter.ISO_OFFSET_DATE_TIME
              .format(ZonedDateTime.now().withNano(0)));
      Element title = xbel.createElement("title");
      bookmark.appendChild(title);
      title.setTextContent(htmlTitle);
      var stringWriter = new StringWriter();
      try {
        TransformerFactory.newInstance().newTransformer().transform(
            new DOMSource(xbel), new StreamResult(stringWriter));
      } catch (Exception e) {
        throw new RuntimeException("Cannot convert XBEL to a string.");
      }
      return stringWriter.toString();
    }
  };

  /**
   *  Text of the bookmark.
   *
   *  @param uri
   *    URI of the bookmark.
   *
   *  @param htmlTitle
   *    HTML title of the web page.
   *
   *  @return
   *    Text of the bookmark.
   */
  public abstract String text(URI uri, String htmlTitle);
}
