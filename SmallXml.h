/*
SmallXml - Tiny and Simple Xml DOM

www.github.com/theliuy/SmallXml.git
Author: Yang Liu
        theliuy.com
*/

#ifndef SMALL_XML_H
#define SMALL_XML_H

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
    ToString
    Return a string in Xml format
  */
  std::string ToString() const; 

  // Get and set
  /*
    Type
    Get - return a copy of type_.
    Set - Not allowed.
  */
  NodeType type() const;
  
  /*
    Sibling
    Get - Return prev or next silbing.
    Set - Not allowed
  */
  XmlNode * prev();
  XmlNode * next();

  /*
    Value
    Get - Return
      Content of Comment
      Plain Text of Element
    Set - Set the value above
  */
  std::string content() const;
  std::string & content();
  std::string tag();
  std::string & tag();

 private:
  // Type of this node
  NodeType type_;

  // Next and Previous sibling
  XmlNode * prev_;
  XmlNode * next_;

  // Fist and Last sibling
  // Except for element, they are NULL.
  XmlNode * first_child_;
  XmlNode * last_child_;

  // Value has different meaning to different type.
  // For Element, value_ is the plain text
  // For Comment, value_ is the comment.
  // For Declaration and Unkown, it is empty.
  std::string text_;
  std::string tag_;
}

}

#endif
