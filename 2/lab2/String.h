#include <iostream>
#include <algorithm>
#include <memory>
#include "MyString.h"

using namespace std;

class String {
  using string = emcpp::MyString;
//    using string = std::string;
  std::shared_ptr<string> data;

public:
  // creates an empty string
  String(): data(std::make_shared<string>()) {
  }

  // copy C-string
  String(const char* str): data(std::make_shared<string>(str)) {
  }

  // no copy
  String(const String& str): data(str.data) {
  }

  // no copy
  String operator=(const String& s) {
    data = s.data;
    return *this;
  }

  /// makes a copy of a string if it has more than one reference.
  void set(int index, char ch) {
    //note: not concurrent
    if(data.use_count() > 1) {
      data = std::make_shared<string>(*data);
    }
    (*data)[index] = ch;
  }

  /// no copy
  char get(int index) const {
    return (*data)[index];
  }

  /// creates a new string only if both strings are non empty
  friend String operator+(String a, String b) {
    if(a.data->length() != 0 && b.data->length() == 0) {
      return a;
    } else if(a.data->length() == 0 && b.data->length() != 0) {
      return b;
    } else {
      String result(a.data->c_str());
      *result.data += b.data->c_str();
      return result;
    }
  }

  friend std::ostream & operator<< (std::ostream& out, String s) {
    out << *s.data;
    return out; 
  }
};
