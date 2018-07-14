# XML/Unix Processing Tools

## Introduction

These tools are used to convert XML and HTML to and from a line-oriented format more amenable to processing by classic Unix pipeline processing tools, like `grep`, `sed`, `awk`, `cut`, shell scripts, and so forth.

Documentation (reference.md) is available, and examples (examples.md) are illustrative.

### Installation

1. Fetch and install the `gnome-xml` library (`libxml`).<br/><br/>
I'm using version 1.8.6. Other versions might or might not work.
Make sure `xml-config` is on your path.

2. Fetch and unpack the source tarball for my tools from this repository.<br/><br/>
Look for a file named `xml2-version.tar.gz`.

3. Run `make`.<br/><br/>You should now have several binaries: `xml2`, `2xml`, `csv2`, `2csv`.<br/>
Symbolic links are used to offer alternative names: `html2` and `2html`.

3. Copy the binaries and links somewhere.

### Limitations

- Namespace support is absent.

- Whitespace isn't always preserved, and the rules for preserving and generating whitespace are complex.

- It's possible to preserve all whitespace, but the resulting flat files are big and ugly. In most cases, whitespace is meaningless, used only to make the XML human-readable. Even in HTML, whitespace is sometimes significant and sometimes not, with no easy way to tell which is which.

- XML is fundamentally hierarchical, not record-oriented.

- The usefulness of record-oriented Unix tools to this domain will always be limited to simple operations like basic search and replacement, no matter how many syntactic transformations we make. More complex processing requires XML-specific tools like XSLT.

- The transformation is complex.<br/><br/>
The syntax used by these tools is relatively intuitive, but difficult to describe precisely. (My own documentation relies only on examples.) This makes it difficult to formally reason about data, so subtle errors are easy to make.

---

Author: *Dan Egnor* (ofb.net/~egnor)<br/>
Converted manually by *Lorenzo L. Ancora*, from HTML to MarkDown. All legal rights remain with the original author and this documentation is distributed non-profit.
