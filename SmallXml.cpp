#include "SmallXml.h"

namespace {

std::string get_indent(int indent) {
  std::string str = "";
  while (indent-- > 0)
    str += "  ";

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
    prev_(0), next_(0),
    first_child_(0),
    last_child_(0),
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
    prev_(0), next_(0),
    first_child_(0), last_child_(0),
    text_("") {
  // TODO - complete the switch cases.
  switch (type_) {
    case ELEMENT:
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
    prev_(0), next_(0),
    first_child_(0), last_child_(0),
    text_(""), tag_(""){
  switch(type_) {
    case ELEMENT:
      tag_ = value;
      break;
    case COMMENT:
      text_ = value;
      break;
  }
}

/*
  Destructor
   */
XmlNode::~XmlNode() {
}

void XmlNode::PushChild(XmlNode * child) {
  // TODO
}

void XmlNode::InsertChildBefore(XmlNode * child, XmlNode * target) {
  // TODO
}

void XmlNode::InsertChildAfter(XmlNode * child, XmlNode * target) {
  // TODO
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
std::string XmlNode::ToString(int indent = 0) const {
  std::string result;
  
  switch(type_) {
    case ELEMENT:
      result = ToStringAsElement(indent);
      break;
    case COMMENT:
      result = ToStringAsComment(indent);
      break;
    case DECLARATION:
      result = ToStringAsDeclaration(indent);
      break;
  }

  return result;
}

int XmlNode::type() const {
  return type_;
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

std::string & XmlNode::text() {
  return text_;
}

std::string XmlNode::tag() const {
  return tag_;
}

std::string & XmlNode::tag() {
  return tag_;
}

/////////////////////////////////////////////
// Private member functions

std::string XmlNode::ToStringAsElement(int indent) {

}
std::string XmlNode::ToStringAsComment(int indent) {

}
std::string XmlNode::ToStringAsDeclaration(int indent) {

}

}

#define TEST_SMALLXML
#ifdef TEST_SMALLXML

int main(int argc, char ** argv) {
  
  return 0;
}

#endif
