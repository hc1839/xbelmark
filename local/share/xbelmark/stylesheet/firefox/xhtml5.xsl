<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:moz="http://www.mozilla.org/"
                xmlns:ext="xalan://io.github.hc1839.xbelmark.xslt.ext.DateTime"
                xmlns="http://www.w3.org/1999/xhtml"
                extension-element-prefixes="ext"
                exclude-result-prefixes="moz"
                version="1.0">

  <!-- Title of the bookmarks html. -->
  <xsl:param name="bookmarks.title" select="'Bookmarks'"/>

  <!-- Heading name of the bookmarks html. -->
  <xsl:param name="bookmarks.menu.name" select="'Bookmarks Menu'"/>

  <!-- Display name of a folder with no title. -->
  <xsl:param name="folder.title" select="'[Folder Name]'"/>

  <!-- Default folded status of a folder. -->
  <xsl:param name="folded.default" select="'yes'"/>

  <xsl:output method="html"/>

  <xsl:variable name="moz.ns" select="'http://www.mozilla.org/'"/>
  <xsl:variable name="vendor.id" select="system-property('xsl:vendor')"/>
  <xsl:variable name="vendor.libxslt.id" select="'libxslt'"/>
  <xsl:variable name="vendor.xalan.id" select="'Apache Software Foundation'"/>

  <xsl:template match="*|text()"/>

  <xsl:template match="/">
    <xsl:apply-templates select="xbel"/>
  </xsl:template>

  <xsl:template match="/xbel">
    <html lang="" xml:lang="">
      <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></meta>
        <title>
          <xsl:call-template name="title">
            <xsl:with-param name="title.element" select="title"/>
            <xsl:with-param name="default" select="$bookmarks.title"/>
          </xsl:call-template>
        </title>
        <style>
          .foldable {
            cursor: pointer;
          }
          .foldable.folded:before {
            content: "[+] ";
          }
          .foldable:before {
            content: "[\2013] ";
          }
          .hideable.hidden {
            max-height: 0;
            overflow: hidden;
          }
          .item-list.hidden {
            margin-bottom: 0px;
          }
          .item-list {
            margin-bottom: 25px;
          }
        </style>
      </head>
      <body>
        <h1>
          <xsl:value-of select="$bookmarks.menu.name"/>
        </h1>
        <dl>
          <xsl:apply-templates select="folder|separator|bookmark"/>
        </dl>
        <script>
          var foldables = document.getElementsByClassName("foldable");
          for (var i = 0; i != foldables.length; ++i) {
            foldables[i].onclick = function() {
              this.classList.toggle("folded");
              var hideable = this.parentElement.nextElementSibling;
              hideable.classList.toggle("hidden");
            }
          }
        </script>
      </body>
    </html>
  </xsl:template>

  <!-- Class of a foldable section depending on whether it is folded or not. -->
  <xsl:template name="folded-class">
    <xsl:param name="folded" select="$folded.default"/>
    <xsl:choose>
      <xsl:when test="$folded = 'yes'">
        <xsl:value-of select="'folded'"/>
      </xsl:when>
      <xsl:when test="$folded = 'no'">
        <xsl:value-of select="''"/>
      </xsl:when>
      <xsl:when test="not($folded)">
        <xsl:call-template name="folded-class">
          <xsl:with-param name="folded" select="$folded.default"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          <xsl:text>Invalid `folded` value: </xsl:text>
          <xsl:value-of select="$folded"/>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/xbel//folder">
    <xsl:variable name="folded.class">
      <xsl:call-template name="folded-class">
        <xsl:with-param name="folded" select="@folded"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="hidden.class">
      <xsl:choose>
        <xsl:when test="$folded.class">
          <xsl:value-of select="'hidden'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="''"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <dt>
      <h3 class="foldable {$folded.class}">
        <xsl:apply-templates select="@added"/>
        <xsl:apply-templates select="info/metadata/moz:*"/>
        <xsl:call-template name="title">
          <xsl:with-param name="title.element" select="title"/>
          <xsl:with-param name="default" select="$folder.title"/>
        </xsl:call-template>
      </h3>
    </dt>
    <dl class="item-list hideable {$hidden.class}">
      <xsl:if test="folder|separator|bookmark">
        <xsl:apply-templates select="folder|separator|bookmark"/>
      </xsl:if>
    </dl>
  </xsl:template>

  <xsl:template match="/xbel//separator">
    <hr></hr>
  </xsl:template>

  <xsl:template match="/xbel//bookmark">
    <dt>
      <a href="{@href}">
        <xsl:apply-templates select="@added|@modified"/>
        <xsl:apply-templates select="info/metadata/moz:*"/>
        <xsl:call-template name="title">
          <xsl:with-param name="title.element" select="title"/>
          <xsl:with-param name="default" select="@href"/>
        </xsl:call-template>
      </a>
    </dt>
  </xsl:template>

  <!-- Any XBEL metadata for Firefox. -->
  <xsl:template match="/xbel//metadata[@owner = 'http://www.mozilla.org/']/moz:*">
    <xsl:attribute name="{local-name(.)}">
      <xsl:value-of select="."/>
    </xsl:attribute>
  </xsl:template>

  <!-- Title of an XBEL entity. -->
  <xsl:template name="title">
    <xsl:param name="title.element"/>
    <xsl:param name="default" select="''"/>
    <xsl:choose>
      <xsl:when test="title">
        <xsl:value-of select="title"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$default"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- When the folder was added. -->
  <xsl:template match="/xbel//folder/@added">
    <xsl:if test="$vendor.id = $vendor.xalan.id or $vendor.id = $vendor.libxslt.id">
      <xsl:attribute name="add_date">
        <xsl:value-of select="ext:dateTimeToUnix(.)"/>
      </xsl:attribute>
      <xsl:attribute name="last_modified">
        <xsl:value-of select="ext:dateTimeToUnix(.)"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <!-- When the bookmark was added. -->
  <xsl:template match="/xbel//bookmark/@added">
    <xsl:if test="$vendor.id = $vendor.xalan.id or $vendor.id = $vendor.libxslt.id">
      <xsl:attribute name="add_date">
        <xsl:value-of select="ext:dateTimeToUnix(.)"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <!-- When the bookmark was modified. -->
  <xsl:template match="/xbel//bookmark/@modified">
    <xsl:if test="$vendor.id = $vendor.xalan.id or $vendor.id = $vendor.libxslt.id">
      <xsl:attribute name="last_modified">
        <xsl:value-of select="ext:dateTimeToUnix(.)"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
