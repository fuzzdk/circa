#ifndef rpn_hpp
#define rpn_hpp

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include "Display.hpp"

class ValueStack
{
  public:
    ValueStack() {}
    double pop() {if (values_.empty()) throw int(); double val=values_.back(); values_.pop_back(); return val;}
    void push(const std::string &s)
    {
      double val=std::stod(s);
      values_.push_back(val);
    }
    void push(double d) {values_.push_back(d);}
    int size() const {return values_.size();}
    bool empty() const {return values_.empty();}
    double last() const {return *(values_.rbegin());}
    const std::vector<double>& get_values() const {return values_;}

  private:
    std::vector<double> values_;
};

class RPN
{
  DisplayI &d;
  ValueStack stack;
  std::string entry;

  void enter(bool implicit);
  void two_input_operator(const std::function<double(double,double) > &lambda);
  void one_input_operator(const std::function<double(double) > &lambda);
  void drop();
  

  public:
  const std::vector<double>& get_stack() const {return stack.get_values();}
  RPN(DisplayI &d) : d(d) {}
  void process_input(char ch);
};

#endif
