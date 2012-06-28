#include "SmallXml.h"
#include <algorithm>
#include <functional>

#include <cstdio>


namespace {

std::string showIndent(int indent) {
  std::string str = "";
  while (indent-- > 0)
    str += "  ";

  return str;
}

// trim from start
std::string lTrim(std::string str) {
  std::string::iterator it = 
    find_if(str.begin(), 
            str.end(), 
            std::not1(std::ptr_fun(std::isspace)));
  str.erase(str.begin(), it);
  return str;
}

// trim from end
std::string rTrim(std::string str) {
  std::string::reverse_iterator it = 
    find_if(str.rbegin(), 
            str.rend(),
            std::not1(std::ptr_fun(std::isspace)));
  str.erase(it.base(), str.end());
  return str;
}

// trim from both start and end
inline std::string trim(std::string str) {
  str = lTrim(rTrim(str));
  return str;
}

}

namespace SmallXml {

/*
  Constructor with no argument
  Create a node as element, and leave
  text_ empty, set tag as "DEFAULT".
   */
XmlNode::XmlNode()
  : type_(ELEMENT),
    parent_(NULL),
    prev_(NULL), next_(NULL),
    first_child_(NULL), last_child_(NULL),
    text_(""), tag_("DEFAULT") {
}

/*
  Create a node with a given type.
  In the cases of type,
  Element - set tag_ as "DEFAULT"
            set text_ as empty string
  Comment - set tag_ and text_ as empty
  Declaration - default version encoding
   */
XmlNode::XmlNode(NodeType type)
  : type_(type),
    parent_(NULL),
    prev_(NULL), next_(NULL),
    first_child_(NULL), last_child_(NULL),
    text_(""), tag_("") {
  
  switch (type_) {
    case ELEMENT:
      tag_ = "DEFAULT";
      break;
    case DECLARATION:
      break;
    case COMMENT:
      break;
  }
}

/*
  Constructor with ginven type and value
  In the cases of type,
  Element - value is the name of tag, tag_
  Comment - value is the the content, text_
  Declaration - same as the above one
   */
XmlNode::XmlNode(NodeType type, const std::string & value) 
  : type_(type),
    parent_(NULL),
    prev_(NULL), next_(NULL),
    first_child_(NULL), last_child_(NULL),
    text_(""), tag_("") {
  switch(type_) {
    case ELEMENT:
      set_tag(value);
      break;
    case COMMENT:
      set_text(value);
      break;
  }
}

/*
  Copy Constructor
*/
XmlNode::XmlNode(const XmlNode & node) 
  : type_(node.type_),
    parent_(node.parent_),
    prev_(node.prev_), next_(node.next_),
    first_child_(node.first_child_), last_child_(node.last_child_),
    text_(node.text_), tag_(node.tag_) {

}

/*
  Destructor
   */
XmlNode::~XmlNode() {
  XmlNode * node = first_child_;
  XmlNode * tmp = 0;
  
  while (node) {
    tmp = node;
    node = node->NextSibling();
    delete tmp;
  }
}

/*
  Insert a node as the last child of this node.
  Return 
    The pointer to its copy, if the operation successes
    Or NULL, if it is failed.
*/
XmlNode * XmlNode::PushChild(const XmlNode & node) {
  // Only elements have children
  if (ELEMENT != type_)
    return NULL;
    
  // Make a copy
  XmlNode * p_tmp_node = new XmlNode(node);
  if (NULL == p_tmp_node)
    return p_tmp_node;
  
  p_tmp_node->parent_ = this;
  
  p_tmp_node->prev_ = last_child_;
  p_tmp_node->next_ = NULL;
  
  if (NULL != last_child_)
    last_child_->next_ = p_tmp_node;
  else
    first_child_ = p_tmp_node;
  
  last_child_ = p_tmp_node;
  return p_tmp_node;
}

XmlNode * XmlNode::InsertChildBefore(const XmlNode & node, XmlNode * before_this) {
  // Check if this node has the target child
  if (NULL == before_this || this == before_this || this != before_this->parent_)
    return NULL;
    
  // Only elements have children
  if (ELEMENT != type_)
    return NULL;
    

  // make a copy
  XmlNode * p_tmp_node = new XmlNode(node);
  if (NULL == p_tmp_node)
    return p_tmp_node;
  
  p_tmp_node->parent_ = this;
  
  p_tmp_node->next_ = before_this;
  p_tmp_node->prev_ = before_this->prev_;
  if (NULL != before_this->prev_) {
    before_this->prev_->next_ = p_tmp_node;
  } else if (first_child_ == before_this) {
    first_child_ = p_tmp_node;
  }
  before_this->prev_ = p_tmp_node;
  
  return p_tmp_node;
}

XmlNode * XmlNode::InsertChildAfter(const XmlNode & child, XmlNode * after_this) {
  if (NULL == after_this || this != after_this->parent_)
    return 0;
    
  if (ELEMENT != type_)
    return 0;
    
  // Copy child
  XmlNode * p_tmp_node = new XmlNode(child);
  if (NULL == p_tmp_node)
    return p_tmp_node;
  
  p_tmp_node->parent_ = this;
  
  p_tmp_node->prev_ = after_this;
  p_tmp_node->next_ = after_this->next_;
  
  if (NULL != after_this->next_) {
    after_this->next_->prev_ = p_tmp_node;
  } else if (last_child_ == after_this) {
    last_child_ = p_tmp_node;
  }
  
  after_this->next_ = p_tmp_node;
  
  return p_tmp_node;
}

int XmlNode::NumOfChildren() const {
  int num = 0;
  XmlNode * p_scan = first_child_;
  while (NULL != p_scan) {
    ++num;
    p_scan = p_scan->next_;
  }
  
  return num;
}

bool XmlNode::HasChild() const {
  return NULL != first_child_;
}

/*
void XmlNode::SetAttribute(const std::string & name,
                           const std::string & value) {

}

std::string XmlNode::GetAttribute(const std::string & name) {

}
*/

/*
  Print function
  Check type and print with indent
   */
std::string XmlNode::ToString(int indent) const {
  
  switch(type_) {
    case ELEMENT:
      return ToStringAsElement(indent);
    case COMMENT:
      return ToStringAsComment(indent);
    case DECLARATION:
      return ToStringAsDeclaration(indent);
  }

  return "";
}

int XmlNode::type() const {
  return type_;
}

std::string XmlNode::typeAsString() const {
  switch (type_) {
    case ELEMENT:
      return "ELEMENT";
    case DECLARATION:
      return "DECLARATION";
    case COMMENT:
      return "COMMENT";
  }
  
  return "";
}

XmlNode * XmlNode::PreviousSibling() const {
  return prev_;
}

XmlNode * XmlNode::PreviousSibling() {
  return prev_;
}

XmlNode * XmlNode::NextSibling() const {
  return next_;
}

XmlNode * XmlNode::NextSibling() {
  return next_;
}

XmlNode * XmlNode::FirstChild() const {
  return first_child_;
}

XmlNode * XmlNode::FirstChild() {
  return first_child_;
}

XmlNode * XmlNode::LastChild() const {
  return last_child_;
}

XmlNode * XmlNode::LastChild() {
  return last_child_;
}

std::string XmlNode::text() const {
  return text_;
}

void XmlNode::set_text(const std::string & text) {
  if (DECLARATION == type_)
    return;
  
  text_ = trim(text);
}

std::string XmlNode::tag() const {
  return tag_;
}

void XmlNode::set_tag(const std::string & tag) {
  if (ELEMENT != type_)
    return;
    
  tag_ = trim(tag);
}

/////////////////////////////////////////////
// Private member functions

std::string XmlNode::ToStringAsElement(int indent) const {
  // Init result with open tag
  // TODO - Process Attributes
  std::string result = showIndent(indent) + "<" + tag_ + ">\n";
  
  // Append text
  if (!text_.empty())
    result += showIndent(indent + 1) + text_ + "\n";
  
  // Append children
  XmlNode * p_child;
  for (p_child = this->FirstChild(); 
       NULL != p_child; 
       p_child = p_child->NextSibling()) {
    result += p_child->ToString(indent + 1);
  }

  // Append close tag
  result += showIndent(indent) + "</" + tag_ + ">\n";

  return result;
}

std::string XmlNode::ToStringAsComment(int indent) const {

  // In XML 1.1, '--' is not allowed in text.
  // While, it should be check when init a comment.
  std::string result = showIndent(indent) + "<!-- " + text_ + " -->";
  
  return result;
}
std::string XmlNode::ToStringAsDeclaration(int indent) const {
  // About indent, I am still thinking Should declaration start 
  // with indent or not.
  
  // TODO
  // Sample Declaration. After I implement attribute, I will change it.
  return "<?xml version=\"1.1\"?>\n";
}

}

#define TEST_SMALLXML
#ifdef TEST_SMALLXML

#include <iostream>

using namespace std;
using namespace SmallXml;

void test_ToString() {
  cout << "\n----- Test ToString()s -----\n";
  cout << "Declaration\n";
  XmlNode elem1(XmlNode::DECLARATION);
  cout << elem1.ToString();
  
  cout << "Element\n";
  XmlNode elem2(XmlNode::ELEMENT, "  TagName");
  elem2.set_text("   Some Content ");
  cout << elem2.ToString();
}

void test_inserts() {
  cout << "\n----- Test Insert()s -----\n";
  cout << "PushChild()\n";
  XmlNode elem0(XmlNode::ELEMENT, "Parent");
  XmlNode elem1(XmlNode::ELEMENT, "Child1");
  XmlNode elem2(XmlNode::ELEMENT, "Child2");
  XmlNode elem3(XmlNode::ELEMENT, "Child3");
  XmlNode elem4(XmlNode::ELEMENT, "Child4");
  elem2.set_text("Content of child2");
  cout << "--Before Insertion\n";
  cout << elem0.ToString(1);
  cout << "--After Insertion\n";
  XmlNode * p_new_elem1 = elem0.PushChild(elem1);
  XmlNode * p_new_elem2 = elem0.PushChild(elem2);
  cout << elem0.ToString(1);
  
  cout << "InsertChildBefore(child3 , child1)\n";
  elem0.InsertChildBefore(elem3, p_new_elem1);
  cout << elem0.ToString(1);
  
  cout << "InsertChildAfter(child4 , child1)\n";
  elem0.InsertChildAfter(elem4, p_new_elem1);
  cout << elem0.ToString(1);
  cout << "\nNow parent has " << elem0.NumOfChildren() << " children.\n";
}

int main(int argc, char ** argv) {
  //test_ToString();

  test_inserts();

  return 0;
}



#endif
