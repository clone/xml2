# Examples

Common GNU tools (`wget`, `grep`, ...) are assumed.

## XML

### Use the Slashdot backend.

    % wget -q -O - http://slashdot.org/slashdot.xml | xml2
    
    /backslash/@xmlns:backslash=http://slashdot.org/backslash.dtd
    /backslash/story/title=More on Athlon Overclocking
    /backslash/story/url=http://slashdot.org/articles/00/03/04/1441248.shtml
    /backslash/story/time=2000-03-05 03:40:47
    /backslash/story/author=Hemos
    /backslash/story/department=better-faster-strong
    /backslash/story/topic=amd
    /backslash/story/comments=56
    /backslash/story/section=articles
    /backslash/story/image=topicamd.gif
    /backslash/story
    /backslash/story/title=New Atari Jaguar Game Running $1,225 on eBay
    /backslash/story/url=http://slashdot.org/articles/00/03/02/1430232.shtml
    ...

### Now, just the headlines.

    % wget -q -O - http://slashdot.org/slashdot.xml | xml2 | grep story/title= | cut -d= -f 2-
    
    More on Athlon Overclocking
    New Atari Jaguar Game Running $1,225 on eBay
    AT&T;'s Korn Shell Source Code Released
    TheBench.org: Community Cartooning
    OpenGL for Palm OS Environment
    Banner Ads on Your Cell Phone
    Burning Money on Open Source
    Embedded OpenBSD Running the Stallion ePipe
    Bezos Responds to Tim O'Reilly's Open Letter
    Update on 'Blame Canada' and the Oscars

### How big is the Red Hat 6.1 libxml RPM?

For variety, we use awk rather than grep and cut:

    % wget -q -O - http://rpmfind.net/linux/RDF/redhat/6.1/i386/libxml-1.4.0-1.i386.rdf | xml2 | awk -F= '/RPM:Size/ {print $2}'
    
    704399

### What is the melting point of silicon?

<sup>More awkitude. Don't let your CPU get hotter than this!</sup>

    % wget -q -O - http://metalab.unc.edu/xml/examples/periodic_table/allelements.xml |  xml2 | awk '/ATOM\/NAME=Silicon/,!/ATOM\//' |  awk -F\= '/MELTING_POINT/ {print $2}'
    
    Kelvin
    1683

<sup>(1683ºK is 2570ºF, by the way.)</sup>


## HTML


### Fetch the Slashdot news page.

You'll probably see some warnings. (Slashdot has some of the worst HTML I've ever seen...)

    % wget -q -O - http://slashdot.org/ | html2
    
    /html/head/title=Slashdot:News for Nerds. Stuff that Matters.
    /html/head=
    /html=
    /html/body/@bgcolor=#000000
    /html/body/@text=#000000
    /html/body/@link=#006666
    /html/body/@vlink=#000000
    /html/body=
    /html/body/center/a/@href=http://209.207.224.220/redir.pl?1789
    /html/body/center/a/@target=_top
    ...

### Find all the links.

If you find the warnings distracting, redirect the standard error of `html2` to `/dev/null`.

    % wget -q -O - http://slashdot.org/ | html2 | grep 'a/@href' | cut -d\= -f 2- | sort | uniq
    
    /about.shtml
    /advertising.shtml
    /article.pl?sid=99/03/31/0137221
    /article.pl?sid=99/04/25/1438249
    /article.pl?sid=99/04/27/0310247
    /article.pl?sid=99/04/29/0124247
    /article.pl?sid=99/08/24/1327256&mode;=thread
    /awards.shtml
    /cheesyportal.shtml
    /code.shtml
    ...

### Change some colors.

This pipeline uses both `html2` and `2html` to effect a round-trip. In the middle, `sed` applies a transformation, turning the background of every colored table on the page yellow. Yuck, huh?

    % wget -q -O - http://slashdot.org/ | html2 | sed 's|table/@bgcolor=\(.*\)$|table/@bgcolor=yellow|' | 2html > slashdot.html
    % netscape slashdot.html

### Strip JavaScript from a Geocities home page.

Geocities uses JavaScript to create an annoying little brand popup in the corner of their members' home pages. Let's delete it.

    % wget -q -O - http://www.geocities.com/SiliconValley/Peaks/5957/xml.html |  html2 | grep -vi '^[^=]*/script[/=]' | 2html > xml.html
    % netscape xml.html

---

Author: *Dan Egnor* (ofb.net/~egnor)<br/>
Converted manually by *Lorenzo L. Ancora*, from HTML to MarkDown. All legal rights remain with the original author and this documentation is distributed non-profit.
