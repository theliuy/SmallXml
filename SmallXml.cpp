#include "SmallXml.h"
#include <algorithm>
#include <functional>
#include <stack>

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

// Replace all occurrence in the given string
std::string replaceAll(std::string origin, // the Origin string
                       const std::string & from, // old part
                       const std::string & to // new part
                       ) {
  int from_size = from.size();
  int to_size = to.size();
  
  if (0 == from_size)
    return origin;
    
  int i = 0;
  // It's funny. I should cast unsigned int to int
  while (i < static_cast<int>(origin.size()) - from_size) {
    if (0 == origin.compare(i, from_size, from)) {
      origin.replace(i, from_size, to);
      i += to_size;
    } else {
      ++i;
    }
  }
  
  return origin;                       
}

bool isWhiteSpace(const char c) {
  return (' '  == c || 
          '\n' == c ||
          '\r' == c || 
          '\t' == c || 
          '\v' == c ||
          '\f' == c);
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
    text_(""), tag_("DEFAULT"),
    attributes_(std::map<std::string, std::string>()) {
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
    text_(""), tag_(""),
    attributes_(std::map<std::string, std::string>()) {
  
  switch (type_) {
    case ELEMENT:
      tag_ = "DEFAULT";
      break;
    case DECLARATION:
      SetVersion("1.1");
      SetEncoding("UTF-8");
      break;
    case COMMENT:
      break;
    case UNKNOWN:
      break;
    case TEXT:
      break;
    case DOCUMENT:
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
    case TEXT:
      set_text(value);
      break;
    case UNKNOWN:
      set_text(value);
      break;
    case DOCUMENT:
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
    first_child_(0), last_child_(0),
    text_(node.text_), tag_(node.tag_),
    attributes_(node.attributes_) {
  XmlNode * scan = node.first_child_;
  
  while (NULL != scan) {
    PushChild(*scan);
    scan = scan->next_;
  }
}

XmlNode & XmlNode::operator=(const XmlNode & node) {
  if (this == &node)
    return *this;
  
  type_ = node.type_;
  parent_ = node.parent_;
  prev_ = node.prev_;
  next_ = node.next_;
  first_child_ = 0;
  last_child_ = 0;
  text_ = node.text_;
  tag_ = node.tag_;
  attributes_ = node.attributes_;
  
  XmlNode * scan = node.first_child_;
  while (NULL != scan) {
    PushChild(*scan);
    scan = scan->next_;
  }
  
  return *this;
}

/*
  Destructor
*/
XmlNode::~XmlNode() {
  XmlNode * node = first_child_;
  XmlNode * tmp = NULL;
  
  while (node) {
    tmp = node;
    node = node->NextSibling();
    delete tmp;
  }
  
  first_child_ = NULL;
  last_child_ = NULL;
}

/*
  Insert a node as the last child of this node.
  Return 
    The pointer to its copy, if the operation successes
    Or NULL, if it is failed.
*/
XmlNode * XmlNode::PushChild(const XmlNode & node) {
  // Only elements have children
  if (ELEMENT != type_ && DOCUMENT != type_)
    return NULL;
    
  if (DOCUMENT == node.type_)
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
  if (ELEMENT != type_ && DOCUMENT != type_)
    return NULL;
    
  if (DOCUMENT == node.type_)
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
    
  if (ELEMENT != type_ && DOCUMENT != type_)
    return 0;
    
  if (DOCUMENT == child.type_)
    return NULL;
    
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
  // Only Element has children
  // Return 0 if type_ is not ELEMENT
  if (ELEMENT != type_ || DOCUMENT != type_)
    return 0;

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

void XmlNode::SetAttribute(const std::string & name,
                           const std::string & value) {
  
  // Only Element & Declaration have children
  if (ELEMENT != type_ && DECLARATION != type_)
    return;
  attributes_[XmlSpecialCharEncode(name)] = XmlSpecialCharEncode(value);
}

void XmlNode::SetAttributes(const std::string & content) {
  if (ELEMENT != type_ && DECLARATION != type_)
    return;
    
  int index = 0;
  
  int content_size = content.size();
  while (index < content_size) {
    EatWhiteSpace(content, index);
    // parse name
    std::string name = "";
    while (index < content_size &&
           '=' != content[index]) {
      name += content[index++];
    }
    
    // attribute without value
    if (!(++index < content_size &&
          ('\"' == content[index] || '\'' == content[index])))
      continue;
      
    ++index;
    std::string value = "";
    while (index < content_size &&
           '\"' != content[index] &&
           '\'' != content[index]) {
      value += content[index++];       
    }

    if (!name.empty() && !value.empty())
      SetAttribute(name, value);
    
    ++index;
  }
}

void XmlNode::RemoveAttribute(const std::string & name) {
  if (ELEMENT != type_)
    return;
    
  attributes_.erase(name);
}

std::string XmlNode::GetAttribute(const std::string & name) const {
  // Only Element & Declaration have children
  if (ELEMENT != type_ && DECLARATION != type_)
    return "";

  std::string encoded_name = XmlSpecialCharEncode(name);
  
  std::map<std::string, std::string>::const_iterator it = attributes_.find(encoded_name);
  if (it != attributes_.end()) {
    return XmlSpecialCharDecode(it->second);
  }
  
  return "";
}

std::vector<std::pair<std::string, std::string> > XmlNode::GetAttributes() const {

  std::vector<std::pair<std::string, std::string> > result;
  
  std::map<std::string, std::string>::const_iterator it = attributes_.begin();
  for (; it != attributes_.end(); ++it) {
    result.push_back(std::pair<std::string, std::string>(it->first, it->second));
  }
  
  return result;
}

void XmlNode::SetVersion(const std::string & version) {
  if (DECLARATION != type_)
    return;
    
  SetAttribute("version", version);
}

void XmlNode::SetEncoding(const std::string & encoding) {
  if (DECLARATION != type_)
    return;
    
  SetAttribute("encoding", encoding);
}

std::string XmlNode::GetVersion() const {
  if (DECLARATION != type_)
    return "";
    
  return GetAttribute("version");
}

std::string XmlNode::GetEncoding() const {
  if (DECLARATION != type_)
    return "";
    
  return GetAttribute("Encoding");
}

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
    case TEXT:
      return ToStringAsText(indent);
    case UNKNOWN:
      return ToStringAsUnknown(indent);
    case DOCUMENT:
      return ToStringAsDOCUMENT(indent);
  }

  return "";
}

void XmlNode::Clear() {
  
  // Set as a Default element
  type_ = TEXT;
  tag_ = "";
  text_ = "";
  
  // Clear Attributes
  attributes_.clear();
  
  // Release Children
  XmlNode * node = first_child_;
  XmlNode * tmp = NULL;
  
  while (node) {
    tmp = node;
    node = node->NextSibling();
    delete tmp;
  }
  
  first_child_ = NULL;
  last_child_ = NULL;
}

bool XmlNode::Read(const std::string & content) {
  int index = 0;
  return Read(content, index);
}

bool XmlNode::Read(const std::string & content, int & index) {
  if (DOCUMENT == type_)
    return ReadDocument(content, index);
  else
    return ReadNode(content, index);
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

XmlNode * XmlNode::PreviousElement(const std::string & tag) const {
  XmlNode * scan = prev_;
  while(NULL != scan) {
    if (ELEMENT == scan->type_ && 
        tag_ == tag) {
      return scan;    
    }
    
    scan = scan->prev_;
  }
  
  return NULL;
}

XmlNode * XmlNode::NextElement(const std::string & tag) const {
  XmlNode * scan = next_;
  while (NULL != scan) {
    if (ELEMENT == scan->type_ &&
        scan->tag_ == tag) {
      return scan;    
    }
    
    scan = scan->next_;
  }
  
  return NULL;
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
  
  // For element, text is a child of parent
  if (ELEMENT == type_) {
    PushChild(XmlNode(TEXT, XmlSpecialCharEncode(trim(text))));
    return;
  }
  
  text_ = XmlSpecialCharEncode(trim(text));
}

std::string XmlNode::tag() const {
  return tag_;
}

void XmlNode::set_tag(const std::string & tag) {
  if (ELEMENT != type_)
    return;
    
  tag_ = XmlSpecialCharEncode(trim(tag));
}


// General string to xml string
std::string XmlNode::XmlSpecialCharEncode(std::string str) {

  str = replaceAll(str, "&" , "&amp;");
  str = replaceAll(str, "<" , "&lt;");
  str = replaceAll(str, ">" , "&gt;");
  str = replaceAll(str, "\'", "&apos;");
  str = replaceAll(str, "\"", "&quot;");

  return str;
}

// Xml string to General string
std::string XmlNode::XmlSpecialCharDecode(std::string str) {
  str = replaceAll(str, "&lt;"  , "<");
  str = replaceAll(str, "&gt;"  , ">");
  str = replaceAll(str, "&apos;", "\'");
  str = replaceAll(str, "&quot;", "\"");
  str = replaceAll(str, "&amp;" , "&");

  return str;
}

/////////////////////////////////////////////
// Private member functions

std::string XmlNode::ToStringAsElement(int indent) const {
  std::string new_line_sep = (-1 == indent) ? "" : "\n";

  // Init result with open tag
  std::string result = showIndent(indent) + "<" + tag_;
  
  // Traverse Attributes
  for (std::map<std::string, std::string>::const_iterator it = attributes_.begin();
       it != attributes_.end();
       ++it) {
    result += " " + it->first + "=\"" + it->second + "\"";
  }
  
  result += ">" + new_line_sep;
  
  /*
   In the latest design, I push the text as a child of element,
   thus, these code is not needed any more.
  // Append text
  if (!text_.empty())
    result += showIndent(indent + 1) + text_ + "\n";
  */
  
  // Append children
  XmlNode * p_child;
  int child_indent = (-1 == indent) ? -1 : indent + 1;
  for (p_child = this->FirstChild(); 
       NULL != p_child; 
       p_child = p_child->NextSibling()) {
    result += p_child->ToString(child_indent);
  }

  // Append close tag
  result += showIndent(indent) + "</" + tag_ + ">" + new_line_sep;

  return result;
}

std::string XmlNode::ToStringAsComment(int indent) const {
  std::string new_line_sep = (-1 == indent) ? "" : "\n";

  // In XML 1.1, '--' is not allowed in text.
  // While, it should be check when init a comment.
  std::string result = showIndent(indent) + "<!-- " + text_ + " -->" + new_line_sep;
  
  return result;
}

std::string XmlNode::ToStringAsDeclaration(int indent) const {
  std::string new_line_sep = (-1 == indent) ? "" : "\n";

  // About indent, I am still thinking Should declaration start 
  // with indent or not.
  std::string version_str = GetAttribute("version");
  version_str = version_str.empty() ? "" : " version=\"" + version_str + "\"";
  std::string encoding_str = GetAttribute("encoding");
  encoding_str = encoding_str.empty() ? "" : " encoding=\"" + encoding_str + "\"";
  
  std::string result = "<?xml" + version_str + encoding_str + "?>" + new_line_sep;
  return result;
}

std::string XmlNode::ToStringAsUnknown(int indent) const {
  std::string result = (text_.empty()) ? "" : showIndent(indent) + text_;
  result += "\n";
  
  return result;
}

std::string XmlNode::ToStringAsText(int indent) const {
  std::string new_line_sep = (-1 == indent) ? "" : "\n";

  std::string result = (text_.empty()) ? "" : showIndent(indent) + text_;
  result += new_line_sep;
  
  return result;
}

std::string XmlNode::ToStringAsDOCUMENT(int indent) const {
  std::string result = "";
  
  XmlNode * scan = first_child_;
  while (NULL != scan) {
    result += scan->ToString(indent);
    scan = scan->next_;
  }
  
  return result;
}

XmlNode * XmlNode::ParseNext(const std::string & content, // Content to Parse
                             int & start,                   // Parse index
                             NodeParseFlag & flag,        // flag of result
                             std::string & id) {          // id for element parsing
  EatWhiteSpace(content, start);
  
  if (0 > start || start >= content.size()) {
    return NULL;
  }
  
  XmlNode * result_node = NULL;
  
  // TEXT
  if ('<' != content[start]) {
    std::string text = "";
    do {
      text += content[start];
    } while (++start < content.size() && '<' != content[start]);
    
    result_node = new XmlNode(TEXT, text);
    flag = SELF_CLOSE_TAG;
    id = "";
    return result_node;
  }
  
  // Comment
  if (start + 3 < content.size() && 
      '!' == content[start + 1] &&
      '-' == content[start + 2] &&
      '-' == content[start + 3]) {
    std::string text = "";
    start += 4;
    do {
      text += content[start++];
    } while (start + 2 < content.size() && 
             !('-' == content[start] && '-' == content[start + 1] &&  '>' == content[start + 2]));
    
    // Check close tag "--!>"
    if (start + 2 >= content.size())
      return NULL;
    
    result_node = new XmlNode(COMMENT, text);
    flag = SELF_CLOSE_TAG;
    id = "";
    start += 3;
    return result_node;
  }
  
  // Declaration
  if (start + 4 < content.size() && 
      '?' == content[start + 1]  &&
      'x' == content[start + 2]  &&
      'm' == content[start + 3]  &&
      'l' == content[start + 4]) {
    std::string text = "";
    start += 4;

    while (++start < content.size() && 
           !('?' == content[start] && '>' == content[start + 1])) {
      text += content[start]; 
    }
    
    if (start >= content.size())
      return NULL;
    
    result_node = new XmlNode(DECLARATION);
    result_node->SetAttributes(text);
    flag = SELF_CLOSE_TAG;
    id = "";
    start += 3;
    return result_node; 
  }
  
  // ELEMENT
  // Close tag
  if (start + 1 < content.size() &&
      '/' == content[start + 1]) {
    ++start;
    id = "";
    while (++start < content.size() &&
           '>' != content[start]) {
      id += content[start]; 
    }
    if (start < content.size() && !id.empty())
      flag = CLOSE_TAG;
    start += 1;
    id = XmlSpecialCharEncode(trim(id));
    return result_node;
  }
  
  // OPEN TAG
  std::string tag_str = "";
  // Parse Tag;
  while (++start < content.size()      &&
         !isWhiteSpace(content[start]) &&
         '>' != content[start]) {
    tag_str += content[start];
  }
  
  // Parse Attribute
  std::string attribute_str = "";
  if ('>' != content[start]) {
    while (++start < content.size() &&
           '>' != content[start]) {
      attribute_str += content[start];       
    }
  }
  if (start < content.size() && !tag_str.empty()) {
    result_node = new XmlNode(ELEMENT, tag_str);
    result_node->SetAttributes(attribute_str);
    flag = OPEN_TAG;
    id = "";
  }
  ++start;
  
  return result_node;
}

bool XmlNode::ReadNode(std::string content, int & index) {
    
  std::stack<XmlNode *> parse_stack;
  
  NodeParseFlag flag = UNDEFINE;
  std::string id_str = "";
  int content_size = content.size();
  
  XmlNode * tmp_node_ptr = ParseNext(content, index, flag, id_str);
  
  // Init this node as empty text;
  Clear();
  
  if (UNDEFINE == flag)
    return false;
  
  // Copy the first parsed node to this.
  if (NULL != tmp_node_ptr) {
    *this = *tmp_node_ptr;
    delete tmp_node_ptr;
    tmp_node_ptr = NULL;
  }
  
  if (SELF_CLOSE_TAG == flag) { // It doesn't has children
    return true;
  }
  
  if (OPEN_TAG == flag){
    parse_stack.push(this);
  }
  
  while (0 != parse_stack.size() && index < content.size()) {
    tmp_node_ptr = NULL;
    flag = UNDEFINE;
    tmp_node_ptr = ParseNext(content, index, flag, id_str);
    
    // A self closed tag, a child of top element
    if (SELF_CLOSE_TAG == flag) {
      XmlNode * node_ptr = parse_stack.top();
      if (NULL != node_ptr) {
        node_ptr->PushChild(*tmp_node_ptr);
        delete tmp_node_ptr;
        tmp_node_ptr = NULL;
        continue;
      } else {
        delete tmp_node_ptr;
        return false;
      }
    }
    
    // A Open tag
    if (OPEN_TAG == flag) {
      parse_stack.push(tmp_node_ptr);
      continue;
    }
    
    // Close tag of an element
    if (CLOSE_TAG == flag) {
      XmlNode * node_ptr = parse_stack.top();
      if (id_str != node_ptr->tag_) {
        // Clean the stack, release memory
        while (this != parse_stack.top()) {
          XmlNode * ptr = parse_stack.top();
          parse_stack.pop();
          delete ptr;
        }
        return false;
      }
      
      if (node_ptr == this) {
        return true;
      }
      
      // id matchs, it is not this pointer
      parse_stack.pop();
      if (NULL != parse_stack.top()) {
        parse_stack.top()->PushChild(*node_ptr);
        delete node_ptr;
      }
    }

  }
  
  return true;
}

bool XmlNode::ReadDocument(std::string content, int & index) {
  int content_size = content.size();
  
  while (index < content_size) {
    XmlNode tmp_node;
    if (tmp_node.Read(content, index)) {
      PushChild(tmp_node);
    }
  }
  
  return true;
}

void XmlNode::EatWhiteSpace(const std::string & content, int & start) {
  if (0 > start)
    start = 0;

  while (start < content.size()) {
   char c = content[start];
   if (isWhiteSpace(c)) {
      ++start;  
    }
    else {
      break;
    }
  }
}

}

#define TEST_SMALLXML
#ifdef TEST_SMALLXML

#include <iostream>

using namespace std;
using namespace SmallXml;

// Test ToString functions
void test_ToString();
// Test Insertion
void test_inserts();
// Test Parser and Read
void test_parser();
// Test Find and XPath
void test_find();


int main(int argc, char ** argv) {
  test_ToString();
  
  test_inserts();
  
  test_parser();
  
  test_find();

  return 0;
}

void test_ToString() {
  cout << "\n----- Test ToString()s -----\n";
  cout << "Declaration\n";
  XmlNode elem1(XmlNode::DECLARATION);
  cout << elem1.ToString();
  
  cout << "Element\n";
  XmlNode elem2(XmlNode::ELEMENT, "  TagName");
  elem2.set_text("   Some Content ");
  elem2.SetAttribute("hello", "world");
  elem2.SetAttribute("syracuse", "syracuse ny");
  cout << elem2.ToString();
  
  cout << "Comment\n";
  XmlNode elem3(XmlNode::COMMENT, "Some Comment");
  cout << elem3.ToString();
  
  cout << "Unknown\n";
  XmlNode elem4(XmlNode::UNKNOWN, "Some Text");
  cout << elem4.ToString();
  
  cout << "\n----- Test GetAttributes() -----\n";
  vector<pair<string, string> > attributes = elem2.GetAttributes();
  for (int i = 0; i < attributes.size(); ++i) {
    cout << attributes[i].first << " => " << attributes[i].second << "\n";
  }
}

void test_inserts() {
  cout << "\n----- Test Insert()s -----\n";
  cout << "PushChild()\n";
  XmlNode elem0(XmlNode::ELEMENT, "Parent");
  elem0.SetAttribute("href", "some url");
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
  cout << "\nParent has attribute \"href\" => \"" << elem0.GetAttribute("href") << "\"\n";
}

void test_parser() {
  cout << "\n----- Test Parser -----\n";
  
  int index = 0;
  
  cout << "\nText\n";
  cout << "Origin\n";
  XmlNode text_node(XmlNode::TEXT, "hello world");
  string text = text_node.ToString(1);
  cout << text;
  cout << "Parsed\n";
  XmlNode result_text_node;
  result_text_node.Read(text, index);
  cout << result_text_node.ToString(1);
  cout << "size = " << text.size() << "\n";
  cout << "index = " << index << "\n";
  
  cout << "\nComment\n";
  cout << "Origin\n";
  XmlNode comment_node(XmlNode::COMMENT, "This is a comment");
  string comment = comment_node.ToString(1);
  cout << comment;
  cout << "Parsed\n";
  XmlNode result_comment_node;
  index = 0;
  result_comment_node.Read(comment, index);
  cout << result_comment_node.ToString(1);
  cout << "size = " << comment.size() << "\n";
  cout << "index = " << index << "\n";
  
  cout << "\nDeclaration\n";
  cout << "Origin\n";
  XmlNode dec_node(XmlNode::DECLARATION);
  string dec = dec_node.ToString(1);
  cout << dec;
  cout << "Parsed\n";
  XmlNode result_dec_node;
  index = 0;
  result_dec_node.Read(dec, index);
  cout << result_dec_node.ToString(1);
  cout << "size = " << dec.size() << "\n";
  cout << "index = " << index << "\n";
  
  
  cout << "\nELEMENT\n";
  cout << "Origin\n";
  XmlNode ele_node1(XmlNode::ELEMENT, "Test");
  ele_node1.SetAttribute("hello", "world");
  ele_node1.SetAttribute("New key", "new value");
  XmlNode ele_node2(XmlNode::COMMENT, "Some Comment");
  XmlNode ele_node3(XmlNode::ELEMENT, "Child");
  ele_node1.PushChild(ele_node2);
  ele_node1.PushChild(ele_node3);
  string ele = ele_node1.ToString(-1);
  cout << ele_node1.ToString(-1) << "\n";
  cout << "Parsed\n";
  XmlNode result_ele_node;
  index = 0;
  result_ele_node.Read(ele, index);
  cout << result_ele_node.ToString(-1) << "\n";
  cout << "size = " << ele.size() << "\n";
  cout << "index = " << index << "\n";
  
  cout << "\nAn Document\n";
  cout << "Origin\n";
  string conj = dec_node.ToString(-1) + " " + ele_node1.ToString(-1) + " " + comment_node.ToString(-1);
  cout << conj << "\n";
  cout << "Parsed\n";
  XmlNode result_conj_node(XmlNode::DOCUMENT);
  index = 0;
  result_conj_node.Read(conj, index);
  cout << result_conj_node.ToString(-1) << "\n";
  cout << "next char = '" << conj[index] << "'\n";
  cout << "size = " << conj.size() << "\n";
  cout << "index = " << index << "\n";
  
  return;
}

void test_find() {
  XmlNode node_0_0(XmlNode::ELEMENT, "SU");
  
  XmlNode * p_node_1_0 = node_0_0.PushChild(XmlNode(XmlNode::ELEMENT, "LCSmith"));
  cout << p_node_1_0;
  p_node_1_0->set_text("The 1st LCSmith");
  XmlNode * p_node_1_1 = node_0_0.PushChild(XmlNode(XmlNode::ELEMENT, "Whitman"));
  XmlNode * p_node_1_2 = node_0_0.PushChild(XmlNode(XmlNode::ELEMENT, "Maxwell"));
  XmlNode * p_node_1_3 = node_0_0.PushChild(XmlNode(XmlNode::ELEMENT, "LCSmith"));
  p_node_1_3->set_text("Another LCSmith");
  
  cout << "\nA huge node\n";
  cout << node_0_0.ToString();
  
  XmlNode * scan = node_0_0.FirstChild();
  cout << "\nFirst Child\n";
  if (NULL == scan)
    cout << "NULL\n";
  else
    cout << scan->ToString();
    
  scan = scan->NextSibling();
  cout << "\nNext Child\n";
  if (NULL == scan)
    cout << "NULL\n";
  else
    cout << scan->ToString();
    
  scan = scan->PreviousSibling();
  cout << "\nPrevious Child\n";
  if (NULL == scan)
    cout << "NULL\n";
  else
    cout << scan->ToString();
    
  scan = scan->NextElement("LCSmith");
  cout << "\nNext LCSmith\n";
  if (NULL == scan)
    cout << "NULL\n";
  else
    cout << scan->ToString();
    
  scan = scan->NextElement("LCSmith");
  cout << "\nNext LCSmith\n";
  if (NULL == scan)
    cout << "Not Found\n";
  else
    cout << scan->ToString();
}

#endif
