/*
SmallXml - Tiny and Simple Xml DOM

www.github.com/theliuy/SmallXml.git
Author: Yang Liu
        theliuy.com
*/

#ifndef SMALLXML_SMALLXML_H
#define SMALLXML_SMALLXML_H

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
  several functions to traverse DOM tree.
*/

class XmlNode {
 public:
  enum NodeType {
    ELEMENT,      // Element
    COMMENT,      // Comment
    TEXT,         // Text
    UNKNOWN,      // Not Used actually...
    DECLARATION,  // Declaration
    DOCUMENT      // Super node
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
      XmlNode();        // I am thinking to remove it
      XmlNode(XmlNode::ELEMENT); 
      // Construct with given tag
      XmlNode(XmlNode::ELEMENT, tag);
    -- XML Comment
      // Construct with empty comment
      XmlNode(XmlNode::COMMENT);
      // Construct with content
      XmlNode(XmlNode::COMMENT, content);
    -- XML DECLARATION
      // Construct a declaration, version="1.1" encoding="UTF-8"
      XmlNode(XmlNode::DECLARATION);
    -- TEXT
      // Construct an empty text
      XmlNode(XmlNode::TEXT);
      // Construct with a given text
      XmlNode(XmlNode::TEXT, text);
    -- DOCUMENT
      // Construct an document
      XmlNode(XmlNode::DOCUMENT);
      
    NOTE:
      When initialized with given string, it calls set_xxx functions.
      Thus, the value will be encoded with XML special characters.
  */
  XmlNode();
  XmlNode(NodeType type);
  XmlNode(NodeType type, const std::string & value);
  
  /*
    Copy Constructor
    They make a real copy
    
    // Copy a node
    XmlNode to_node(from_node);
    // Assign value
    XmlNode to_node = from_node;
  */
  XmlNode(const XmlNode & node);
  XmlNode & operator=(const XmlNode & node);
  
  /*
    Destructor
    
    The Destructor will release the node itself, and children.
  */
  ~XmlNode();

  // Public member functions
  /*
    Insert children
    These functions are only available to Element and Document.
    PushChild - Insert a child node at the end of list
    InsertChildBefore - Insert a child before a target node
    InsertChildAfter - Insert a child after a target node
    
    NOTE:
    When these functions called, the XmlNode object will be COPIED!
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
    
    NumOfChildren() returns the number of children. It is only effective
    to elements and document. It returns 0, if this object doesn't or is not
    allowed to have children.
    HasChild() returns true if and only if the node is element or document,
    and it has more than one child.
    
    // Get number of child
    int num_children = node.NumOfChildren();
    // Check if the node has child
    bool has_child = node.HasChild();
    
    NOTE:
      The number of children is what number of the first layer. It doesn't
      count recursively.
  */
  int NumOfChildren() const;
  bool HasChild() const;
  
  /*
    Attributes
    Attributes is only available for element, declaration.
    
    // Add Attribute
    node.SetAttribute(name, value);
    // Set attribute by a string
    std::string str = "name=\"value\" name0=\"value0\"";
    node.SetAttributes(str);
    // Get Attribute
    std::string value = node.GetAttribute(name);
    // Remove Attribute by name
    node.RemoveAttribute(name);
    
    NOTE:
      Attributes is managed by a map struct. Thus if more than one values
      are set to a consistant value, only the last one will be stored.
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
    
    NOTE:
      If you don't want indent, please set it to -1;
  */
  std::string ToString(int indent = 0) const;
  
  /*
    Clear - Clear all children node and make itself a default element node
  */
  void Clear();
  
  /*
    Read and Load
    Read - Read a string and generate a node
    
    // Read a node from a string
    node.Read(str);
    // Read a node from a string with the start
    // start will change into the index of last read char.
    node.Read(str, start);
    
    NOTE:
      Read functions will return a bool value, to indicate it success or is
      failed. If you want to track where cause the failure, check the index.
  */
  bool Read(const std::string & content);
  bool Read(const std::string & content, int & index);

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
    
    // Get Previous sibling
    XmlNode * prev = node.PreviousSibling();
    // Get Next Sibling
    XmlNode * next = node.NextSibling();
    // Get Previous sibling element by tag
    XmlNode * prev_elem = node.PreviousElement(tag);
    // Get Next sibling element by tag
    XmlNode * next_elem = node.NextElement(tag);
  */
  XmlNode * PreviousSibling() const;
  XmlNode * PreviousSibling();
  XmlNode * NextSibling() const;
  XmlNode * NextSibling();
  XmlNode * PreviousElement(const std::string & tag) const;
  XmlNode * NextElement(const std::string & tag) const;
  
  /*
    Children
    Get - Return first or last child
    
    // Get First Child
    XmlNode * first_child = node.FirstChild();
    // Get Last Child
    XmlNode * last_child = node.LastChild();
  */
  XmlNode * FirstChild() const;
  XmlNode * FirstChild();
  XmlNode * LastChild() const;
  XmlNode * LastChild();
  
  /*
    XPath!
    
    TODO
    
    It is coming soon!
  */
  //XmlNode * XPath(const std::string & path) const;
  //XmlNdoe * XPath(const std::string & path);

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
  std::string ToStringAsDOCUMENT(int indent) const;
  
  /*
    Parser functions
  */
  XmlNode * ParseNext(const std::string & content, // Content to Parse
                      int & start,                   // Parse index
                      NodeParseFlag & flag,        // flag of result
                      std::string & id);           // id for element parsing
                      
  void EatWhiteSpace(const std::string & content, int & start);
  bool ReadNode(std::string content, int & index);
  bool ReadDocument(std::string content, int & index);

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


  // TODO
  // I will explain these member variables soon!
  
  // text_ has different meaning to different type.
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
