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

```cpp
SmallXml::XmlNode doc_node(SmallXml::XmlNode::DOCUMENT);
```

### Element
Create an Element node with "DEFAULT" tag.

```cpp
SmallXml::XmlNode ele_node(SmallXml::XmlNode::ELEMENT);
```
Otherwise, create an Element node with a given tag.

```cpp
SmallXml::XmlNode ele_node(SamllXml::XmlNode::ELEMENT, "TagName");
```

### Declaration
Create a Declaration node with default encoding and version.

By default, encoding is "UTF-8", and version is "1.1".

```cpp
SmallXml::XmlNode dec_node(SmallXml::XmlNode::DECLARATION);
```

### Comment
Create a Comment node with empty content.

```cpp
SmallXml::XmlNode com_node(SmallXml::XmlNode::COMMENT);
```

Create a Comment node with given content

```cpp
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

When destucting a XmlNode object, all children nodes will be freed. Take care of the pointers, it might be invalid after a dectruction.

## Copy & Assignment

XmlNode supports copy construction and override assignment operator, which can be used explicitly or implicilitly. 

When do this operation, the children objects are copied into XmlNode, rather than copy the pointers. Thus, the newly copied children nodes are different objects from the old ones.

Copy constructor can be call in the way below,

```cpp
SmallXml::XmlNode newnode(oldnode);
```

Or,

```cpp
SmallXml::XmlNode newnode = oldnode;
```

Assignment operator can also be used to copy objects

```cpp
newnode = oldnode;
```

## Type

Get type as enumerator type XmlNode::NodeType.

```cpp
int node_type = node.type();
```

Or, get type as string. 

```cpp
std::string type = node.typeAsString();
```

The return values are shown below.
<table>
<tr><td>type</td><td>string</td></tr>
<tr><td>SmallXml::ELEMENT</td><td>ELEMENT</td></tr>
<tr><td>SmallXml::DECLARATION</td><td>DECLARATION</td></tr>
<tr><td>SmallXml::COMMENT</td><td>COMMENT</td></tr>
<tr><td>SmallXml::TEXT</td><td>TEXT</td></tr>
<tr><td>SmallXml::DOCUMENT</td><td>DOCUMENT</td></tr>
</table>

## ToString

Convert the XmlNode to a string. This function is often used as "printing" function. ToString functions are based on type.

ToString functions also accept a parameter as indence. One indence stands for two contigurous white spaces. By the way, a negative value indicates no indence. The default value is 0.

For comment and text node, negative value is the same as 0. Declaration ignores this aagument. Element and Document nodes will pass this argument to their children to make a pretty display.

### Usage

```cpp
node.Tostring(1);
```

### Result

#### Element

The string includes a open tag with attributes and close tag. By the way, it will traverse the children and call their ToString funtions.

```xml
  <TAG_NAME name="value">
    Chilren
  </TAGNAME>
```

#### Comment

The content will be quoted by comment syntax.

```xml
  <!-- Content of Comment -->
```

#### Text

The string will be a plain text.

```xml
  Some text
```

#### Declaration

The sample declaration is shown as below.

```xml
  <?xml version="1.1" encoding="UTF-8"?>
```

#### Document

When invoking ToString of a document node, it will traverse all the children nodes and call their ToString functions.

## Child

SmallXml provides functions for child operation. For the reason that only element and document nodes have child, nodes with other types will do "nothing" when called these functions.

### Getting
Return first or last child. If calling these functions to a non-child node, NULL will be returned.
    
```cpp
// Get First Child
XmlNode * first_child = node.FirstChild();

// Get Last Child
XmlNode * last_child = node.LastChild();
```

#### NOTE:
All these function are overloaded. There is one which returns a const pointer, while another returns a non-restricted pointer.

### Insertion

These functions are only available to Element and Document. Or, nothing happens.

`PushChild` - Insert a child node at the end of list

`InsertChildBefore` - Insert a child before a target node

`InsertChildAfter` - Insert a child after a target node

Usage is shown as below.

```cpp
// Add child1 as parent's last Child
XmlNode * p_child1 = parent.PushChild(child1);
    
// Add child2 before child1
XmlNode * p_child2 = parent.InsertChildBefore(child2, p_child1);
    
// Add child3 after child1
XmlNode * p_child3 = parent.InsertChildAfter(child3, p_child1);
```

#### NOTE:
When these functions called, the XmlNode object will be COPIED!

If insertion successes, a pointer points to the new object in the DOM. If failed, they return null pointers. The newly allocated memory will be released when the parent object is destroyed.

### Status

Here are two functions to get the status of one's child.

`NumOfChildren()` returns the number of children. It is only effective to elements and document. It returns 0, if this object doesn't or is not allowed to have children.

`HasChild()` returns true if and only if the node is element or document, and it has more than one child.
  
```cpp
// Get number of child
int num_children = node.NumOfChildren();

// Check if the node has child
bool has_child = node.HasChild();
```

#### NOTE:
The number of children is what number of the first layer. It doesn't count recursively.

## XPath

Supported after version 0.2.

XPath is used to locates a node in an element or document node. Comment, declaration and text nodes don't have children, thus, `XPath` returns null. The usage is shown as below.

```cpp
// Get the first matched node
XmlNode * p_found = node_0_0.XPath(/path0/path1");

// Get all matched nodes
std::vector <XmlNode *> xpaths_0 = node_0_0.XPaths("/path0/path1");
std::vector <const XmlNode *> xpaths_1 = node_0_0.XPaths_c(/path0/path1");  
```

`XPath` is implemented with depth first search to find the first match node, or returns null.
`XPaths` and `XPaths_c` are implemented with breadth first search to find all matched nodes, or return a empty vectore. The only difference between them is the `XPaths_c` returns a vector of const pointers to XmlNode object, while `XPaths` returns non-constant pointers.

## Get Sibling

Get siblings. It return the next or previous nodes the current node. If it doesn't have siblings, these functions return NULL.
Usage is shown below,

```cpp
// Get Previous sibling
XmlNode * prev = node.PreviousSibling();

// Get Next Sibling
XmlNode * next = node.NextSibling();
```

Or, here are functions to get a sibling element with a given tag name.

```cpp
// Get Previous sibling element by tag
XmlNode * prev_elem = node.PreviousElement(tag);
    
// Get Next sibling element by tag
XmlNode * next_elem = node.NextElement(tag);
```

#### Note:
All these function are overloaded. There is one which returns a const pointer, while another returns a non-restricted pointer.

## Attributes
Attributes is only available for element, declaration.
    
### Setting
This function is only available for element, declaration. Or, nothing happens.

```cpp
// Set Attribute
node.SetAttribute(name, value);

// Set attribute by a string
std::string str = "name=\"value\" name0=\"value0\"";
node.SetAttributes(str);
```

### Getting
This function is only available for element, declaration. Or, it returns an empty string.

```cpp
// Get Attribute
std::string value = node.GetAttribute(name);

// Get all attributes
 std::vector<std::pair<std::string, std::string> > attributes = element.GetAttributes();
```

### Removing
This function is only available for element, declaration. Or, nothing happens.

```cpp
// Remove Attribute by name
node.RemoveAttribute(name);
```    

#### NOTE:
Attributes is managed by a map struct. Thus if more than one values are set to a consistant value, only the last one will be stored.

### Attributes of Declaration
In SmallXml version 0.1, only two attributes are supported by declaration node, version and encoding.

By default, version will be "1.1" and encoding will be "UTF-8"

Here are functions for these attributes. If they are called to a non-declaration node, setting won't occur, while getting returns an empty string.

```cpp
// Setting
declaration.SetVersion("1.2");
declaration.SetEncoding("GBK");

// Getting
std::string version = declaration.GetVersion();
std::string encoding = declaration.GetEncoding();
```

## Clear
Clear all children node and make itself a default element node

```cpp
node.Clear();
```

## Read

SmallXml supports reading a Xml in plain text into a XmlNode object. Usage is shown as below.

```cpp
// A xml in plan string
std::string text = "<tag>Some Content</tag>"

// Sample 0
XmlNode node;
node.Read(text);

// Sample 1
// Or,
int index = 0;
node.Read(text, &index);
```

In the sample 0, if the given text is a valid xml, the node becomes a object, or to be a default element node.

In the sample 1, the argument "index" is the location of where the parsing starts. By the way, the index will be modified as the index of the last valid character, which can be used to track parsing procedure.

## Text & Tag

text_ and tag_ are two private members of XmlNode object. Several public functions are provided to access them.

```cpp
// Get tag
std::string tag = node.tag();
// Get decoded tag
std::string decoded_tag = node.GetDecodedTag();
// Set tag
node.set_tag(given_tag);

// Get text
std::string text = node.text();
// Get decoded text
std::string decoded_text = node.GetDecodedText();
// Set text
node.set_text(given_text);
```

Both `set_tag` and `set_text` will encode the given string. And both `tag` and `text` will return decoded characters.

`GetDecodedTag` and `GetDecodedText` return the decoded text_ and tag_.

While, tag_ and text_ have deferent meaning to nodes with deferent types.

<table>
<tr><td>type</td><td>tag_</td><td></td></tr>
<tr><td>ELEMENT</td><td>The tag of this node</td><td>Not used after version 0.2</td></tr>
<tr><td>DECLARATION</td><td>Not used</td><td>Not used</td></tr>
<tr><td>COMMENT</td><td>Not used</td><td>The content of this comment</td></tr>
<tr><td>TEXT</td><td>Not used</td><td>The content of text node</td></tr>
<tr><td>DOCUMENT</td><td>Not used</td><td>Not used</td></tr>
</table>

## Xml Special Character Encoding & Decoding

```cpp
std::string decoded_string = XmlNode::XmlSpecialCharDecode(encoded);

std::string enocded_string = XmlNode::XmlSpecialCharEncode(decoded);
```

XmlNode also provides static functions to encode and decode string. `XmlSpecialCharDecode` converses xml special characters into plain text, while `XmlSpecialCharEncode` encodes the given plain text into a string with xml special characters.
