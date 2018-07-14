# XML/Unix Processing Tools Documentation

## Usage

There are six tools. They are all simple filters, reading information from standard input in one format and writing the same information to standard output in a different format.

| Tool name | Input        | Output |
|-----------|--------------|--------|
| xml2 	    | XML          | Flat   |
| html2     | HTML 	       | Flat   |
| csv2 	    | CSV          | Flat   |
| 2xml 	    | Flat 	       | XML    |
| 2html     | Flat 	       | HTML   |
| 2csv 	    | Flat 	       | CSV    |

The "Flat" format is specific to these tools. It is a syntax for representing structured markup in a way that makes it easy to process with line-oriented tools. The same format is used for HTML, XML, and CSV; in fact, you can think of `html2` as converting HTML to XHTML and running `xml2` on the result; likewise `2html` and `2xml`.

CSV (comma-separated value) files are less expressive than XML or HTML (CSV has no hierarchy), so `xml2` | `2csv` is a lossy conversion.

## File Format

To use these tools effectively, it's important to understand the "Flat" format. Unfortunately, I'm lazy and sloppy; rather than provide a precise definition of the relationship between XML and "Flat", I will simply give you a pile of examples and hope you can generalize correctly.

(Good luck!)

<table>
<tr><th>XML</th><th>Flat equivalent</th>

<tr>
<td>&lt;thing/&gt;</td>
<td>/thing</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>&lt;thing&gt;&lt;subthing/&gt;&lt;/thing&gt;</td>
<td>/thing/subthing</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>&lt;thing&gt;stuff&lt;/thing&gt;</td>
<td>/thing=stuff</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;thing&gt;<br>
<span class="items">
  &lt;subthing&gt;substuff&lt;/subthing&gt;<br>
  stuff<br>
</span>
  &lt;/thing&gt;<br>
</td>
<td>
  /thing/subthing=substuff<br>
  /thing=stuff<br>
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;person&gt;<br>
<span class="items">
  &lt;name&gt;Juan Do&eacute;&lt;/name&gt;<br>
  &lt;occupation&gt;Zillionaire&lt;/occupation&gt;<br>
  &lt;pet&gt;Dogcow&lt;/pet&gt;<br>
  &lt;address&gt;<br>
<span class="items">
    123 Camino Real<br>
    &lt;city&gt;El Dorado&lt;/city&gt;<br>
    &lt;state&gt;AZ&lt;/state&gt;<br>
    &lt;zip&gt;12345&lt;/zip&gt;<br>
</span>
  &lt;/address&gt;<br>
  &lt;important/&gt;<br>
</span>
  &lt;/person&gt;<br>
</td>
<td>
  /person/name=Juan Do&eacute;<br>
  /person/occupation=Zillionaire<br>
  /person/pet=Dogcow<br>
  /person/address=123 Camino Real<br>
  /person/address/city=El Dorado<br>
  /person/address/state=AZ<br>
  /person/address/zip=12345<br>
  /person/important
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;collection&gt;<br>
<span class="items">
    &lt;group&gt;<br>
<span class="items">
      &lt;thing&gt;stuff&lt;/thing&gt;<br>
      &lt;thing&gt;stuff&lt;/thing&gt;<br>
</span>
    &lt;/group&gt;<br>
</span>
  &lt;/collection&gt;<br>
</td>
<td>
  /collection/group/thing=stuff<br>
  /collection/group/thing<br>
  /collection/group/thing=stuff<br>
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;collection&gt;<br>
<span class="items">
    &lt;group&gt;<br>
<span class="items">
      &lt;thing&gt;stuff&lt;/thing&gt;<br>
</span>
    &lt;/group&gt;<br>
    &lt;group&gt;<br>
<span class="items">
      &lt;thing&gt;stuff&lt;/thing&gt;<br>
</span>
    &lt;/group&gt;<br>
</span>
  &lt;/collection&gt;<br>
</td>
<td>
  /collection/group/thing=stuff<br>
  /collection/group<br>
  /collection/group/thing=stuff<br>
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;thing&gt;<br>
<span class="items">
    stuff<br>
    <br>
    more stuff<br>
    &amp;lt;other stuff&amp;gt;
</span>
  &lt;/thing&gt;<br>
</td>
<td>
  /thing=stuff<br>
  /thing=<br>
  /thing=more stuff<br>
  /thing=&lt;other stuff&gt;<br>
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>&lt;thing flag="value"&gt;stuff&lt;/thing&gt;</td>
<td>
  /thing/@flag=value<br>
  /thing=stuff<br>
</td>

<tr><td colspan="2"><hr></td>
<tr>
<td>
  &lt;?processing instruction?&gt;<br>
  &lt;thing/&gt;
<td>
  /?processing=instruction<br>
  /thing<br>
</td>

</table>

---

Author: *Dan Egnor* (ofb.net/~egnor)<br/>
Converted manually by *Lorenzo L. Ancora*, from HTML to MarkDown. All legal rights remain with the original author and this documentation is distributed non-profit.
