# SmallXml

This project is a C++ library for Xml DOM. The goal of the library is to support basic features of reading and writing Xml DOM.
It supports 5 Xml types:
+ Document
+ Element
+ Declaration
+ Comment
+ Text
In current version, SmallXml doesn't support namespace, CDATA section and encoding other than UTF-8.
This Project is a summer project led by [Dr. Fawcett](http://www.lcs.syr.edu/faculty/fawcett/handouts/webpages/FawcettHome.htm). It may also be used in CIS/CSE681 Objected-Oriented Design course.
## Usage
Include header file, `SmallXml.h` into your code.

``` cpp
#include "SmallXml.h"
```

## Constructor

### Document
Create a Document.

``` cpp
SmallXml::XmlNode doc_node(SmallXml::XmlNode::DOCUMENT);
```

### Element
Create an Element node with "DEFAULT" tag.

``` cpp
SmallXml::XmlNode ele_node(SmallXml::XmlNode::ELEMENT);
```
Otherwise, create an Element node with a given tag.

``` cpp
SmallXml::XmlNode ele_node(SamllXml::XmlNode::ELEMENT, "TagName");
```

### Declaration
Create a Declaration node with default encoding and version.
By default, encoding is "UTF-8", and version is "1.1".

``` cpp
SmallXml::XmlNode dec_node(SmallXml::XmlNode::DECLARATION);
```

### Comment
Create a Comment node with empty content.

``` cpp
SmallXml::XmlNode com_node(SmallXml::XmlNode::COMMENT);
```

Create a Comment node with given content

``` cpp
SmallXml::XmlNode com_node(SmallXml::XmlNode::Comment, "Some comment");
```

### TEXT
Create a empty Text node.

```cpp
SmallXml::XmlNode txt_node(SmallXml::XmlNode::TEXT);
```

Create a Text node with content.

```cpp
SmallXml::XmlNode txt_node(SmallXml::XmlNode::TEXT, "Some Content");
```

## Destructor

## Copy & Assignment

## Type

## ToString

## Child

## XPath

## Get Sibling

## Attributes

## Attributes of Declaration

## Clear

## Read

## Text & Tag

## Xml Special Character Encoding & Decoding

## Protected Text Processing Functions

