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

```c
#include "SmallXml.h"
```

### Document
#### Create
Create a Document

```c
SmallXml::XmlNode doc_node();
```

### Element
### Declaration
### Comment
### Text
