<?xml version='1.0' encoding='UTF-8'?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/xhtml/chunk.xsl"/>

<xsl:param name="base.dir">html/</xsl:param>
<xsl:param name="html.stylesheet" select="'langref.css'"/>
<xsl:param name="chunk.section.depth" select="0"/>
<xsl:param name="section.autolabel" select="1"/>
<xsl:param name="section.label.includes.component.label" select="1"/>

<xsl:template match="productionset">
  <pre class="productionset">
    <xsl:apply-templates select="production"/>
  </pre>
</xsl:template>

<xsl:template match="production">
  <xsl:apply-templates select="lhs"/>
  <xsl:text>&#10;    = </xsl:text>
  <xsl:apply-templates select="rhs"/>
  <xsl:text> ;&#10;&#10;</xsl:text>
</xsl:template>

<xsl:template match="rhs">
  <xsl:for-each select="text() | *">
    <xsl:choose>
      <xsl:when test="name(current()) = 'sbr'">
        <xsl:text>&#10;    | </xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="normalize-space(current())"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:for-each>
</xsl:template>

</xsl:stylesheet>
