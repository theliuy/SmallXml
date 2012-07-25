/*
SmallXml - Tiny and Simple Xml DOM

www.github.com/theliuy/SmallXml.git
Author: Yang Liu
        theliuy.com
*/

#ifndef SMALL_XML_H
#define SMALL_XML_H

#include <string>
#include <map>
#include <vector>

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
    TEXT,
    UNKNOWN,
    DECLARATION
  };
  
  enum NodeParseFlag {
    OPEN_TAG,
    CLOSE_TAG,
    SELF_CLOSE_TAG,
    UNDEFINE
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
    Copy Constructor
  */
  XmlNode(const XmlNode & node);
  XmlNode & operator=(const XmlNode & node);
  
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
    
    NOTE:
    When these functions called, the XmlNode object will be copied.
    If insertion successes, a pointer points to the new object in the
    DOM. If failed, they return null pointers. The newly allocated 
    memory will be released when the parent object is destroyed.
    
    Sample Usage
    XmlNode parent, child1, child2, child3;
    // Add child1 as parent's last Child
    XmlNode * p_child1 = parent.PushChild(child1);
    // Add child2 before child1
    XmlNode * p_child2 = parent.InsertChildBefore(child2, p_child1);
    // Add child3 after child1
    XmlNode * p_child3 = parent.InsertChildAfter(child3, p_child1);
  */
  XmlNode * PushChild(const XmlNode & node);
  XmlNode * InsertChildBefore(const XmlNode & node, XmlNode * before_this);
  XmlNode * InsertChildAfter(const XmlNode & node, XmlNode * after_this);
  
  /*
    Number of children
  */
  int NumOfChildren() const;
  bool HasChild() const;
  
  /*
    Attributes
    Sample usage
    // Add Attribute
    node.SetAttribute(name, value);
    // Get Attribute
    std::string value = node.GetAttribute(name);
  */
  void SetAttribute(const std::string & name, const std::string & value);
  void SetAttributes(const std::string & content);
  void RemoveAttribute(const std::string & name);
  std::string GetAttribute(const std::string & name) const;
  std::vector<std::pair<std::string, std::string> > GetAttributes() const;
  
  /*
    Declaration attributes
    Declaration has two attributes version and encoding
    These setting and getting functions is only available for DECLARATION type
  */
  void SetVersion(const std::string & version);
  void SetEncoding(const std::string & encoding);
  std::string GetVersion() const;
  std::string GetEncoding() const;

  /*
    ToString
    Return a string in Xml format
  */
  std::string ToString(int indent = 0) const;
  
  /*
    Clear - Clear all children node and make itself a default element node
  */
  void Clear();
  
  /*
    Read and Load
    Read - Read a string and generate a node
    Load - Read a file and generate a node
  */
  bool Read(std::string content);
  bool Load(const std::string filename);

  // Get and set
  /*
    Type
    Get - return a copy of type_.
    Set - Not allowed.
  */
  int type() const;
  std::string typeAsString() const;
  
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
  void set_text(const std::string & text);
  std::string tag() const;
  void set_tag(const std::string & tag);
  
  /*
    XmlSpecial characters encoding and decoding
  */
  static std::string XmlSpecialCharEncode(std::string origin);
  static std::string XmlSpecialCharDecode(std::string origin);

 private:
  /*
    ToString by type
  */
  std::string ToStringAsElement(int indent) const;
  std::string ToStringAsComment(int indent) const;
  std::string ToStringAsDeclaration(int indent) const;
  std::string ToStringAsUnknown(int indent) const;
  std::string ToStringAsText(int indent) const;
  
  /*
    Parser functions
  */
  XmlNode * ParseNext(const std::string & content, // Content to Parse
                      int & start,                   // Parse index
                      NodeParseFlag & flag,        // flag of result
                      std::string & id);           // id for element parsing
                      
  void EatWhiteSpace(const std::string & content, int & start);

  // Type of this node
  NodeType type_;
  
  // Parent of this node
  XmlNode * parent_;

  // Next and Previous siblings
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
  
  // Attribute map
  // According to Xml 1.1, value of attributes should be
  // string. Thus, a map in map<string, string> is used to
  // present attributes.
  std::map<std::string, std::string> attributes_;
  
};

}

#endif
