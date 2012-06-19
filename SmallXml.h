/*
SmallXml - Tiny and Simple Xml DOM

www.github.com/theliuy/SmallXml.git
Author: Yang Liu
        theliuy.com
*/

#ifndef SMALL_XML_H
#define SMALL_XML_H

#include <string>

namespace SmallXml {

/*
  A class for everything in the Document Object
  Model. It might be Element, Comment, Declaration.
  Attributes is not included, since attributes 
  are not in the DOM tree.
  Nodes have sibling and chilren. This class contains
  several functions to traverse DOM tree, and IO
  functions.
*/

class XmlNode {
 public:
  enum NodeType {
    ELEMENT,
    COMMENT,
    UNKNOWN,
    DECLARATION
  };

  /*
    Constructors of XmlNode
    -- XML Element:
      // Construct with default tag - <DEFAULT>
      XmlNode(ELEMENT); 
      // Construct with given tag
      XmlNode(ELEMENT, tag);
    -- XML Comment
      // Construct with empty comment
      XmlNode(COMMENT);
      // Construct with content
      XmlNode(COMMENT, content);
    -- XML DECLARATION
      // Construct a declaration
      XmlNode(DECLARATION);
    Default to create an element
  */
  XmlNode();
  XmlNode(NodeType type);
  XmlNode(NodeType type, const std::string & value);
  
  /*
    Destructor
  */
  ~XmlNode();

  // Public member functions
  /*
    Insert children
    These functions are only available to Element
    PushChild - Insert a child node at the end of list
    InsertChildBefore - Insert a child before a target node
    InsertChildAfter - Insert a child after a target node
    
    Sample Usage
    XmlNode parent, child1, child2, child3;
    // Add child1 as parent's last Child
    parent.PushChild(&child1);
    // Add child2 before child1
    parent.InsertChildBefore(&child2, &child1);
    // Add child3 after child1
    parent.InsertChildAfter(&child3, &child1);
  */
  void PushChild(XmlNode * child);
  void InsertChildBefore(XmlNode * child, XmlNode * target);
  void InsertChildAfter(XmlNode * child, XmlNode * target);
  
  /*
    Attributes
    Sample usage
    // Add Attribute
    node.SetAttribute(name, value);
    // Get Attribute
    std::string value = node.GetAttribute(name);
  */
  // void SetAttribute(const std::string & name, const std::string & value);
  // std::string GetAttribute(const std::string & name);

  /*
    ToString
    Return a string in Xml format
  */
  std::string ToString(int indent = 0) const;

  // Get and set
  /*
    Type
    Get - return a copy of type_.
    Set - Not allowed.
  */
  int type() const;
  
  /*
    Sibling
    Get - Return prev or next silbing.
  */
  XmlNode * PreviousSibling() const;
  XmlNode * PreviousSibling();
  XmlNode * NextSibling() const;
  XmlNode * NextSibling();

  /*
    Children
    Get - Return first or last child
  */
  XmlNode * FirstChild() const;
  XmlNode * FirstChild();
  XmlNode * LastChild() const;
  XmlNode * LastChild();

  /*
    Value
    Get - Return
      Content of Comment
      Plain Text of Element
    Set - Set the value above
  */
  std::string text() const;
  std::string & text();
  std::string tag() const;
  std::string & tag();

 private:
  /*
    ToString as type
  */
  std::string ToStringAsElement(int indent);
  std::string ToStringAsComment(int indent);
  std::string ToStringAsDeclaration(int indent);

  // Type of this node
  NodeType type_;

  // Next and Previous sibling
  XmlNode * prev_;
  XmlNode * next_;

  // Fist and Last sibling
  // Except for element, they are NULL.
  XmlNode * first_child_;
  XmlNode * last_child_;

  // text_ has different meaning to different type.
  // For Element, text_ is the plain text
  // For Comment, text_ is the comment.
  // For Declaration and Unkown, it is empty.
  std::string text_;
  // tag_ is only used by element
  // it is the name of tag
  std::string tag_;
};

}

#endif
