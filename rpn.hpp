/*
Copyright 2019-2021 Anders Bo Rasmussen

This file is part of circa.

    circa is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    circa is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with circa.  If not, see <https://www.gnu.org/licenses/>.
*/
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
    double push(const std::string &s)
    {
      double val=std::stod(s);
      values_.push_back(val);
      return val;
    }
    void push(double d) {values_.push_back(d);}
    int size() const {return values_.size();}
    bool empty() const {return values_.empty();}
    double last() const {return *(values_.rbegin());}
    const std::vector<double>& get_values() const {return values_;}

  private:
    std::vector<double> values_;
};

class Change
{
  public:
    Change(const std::vector<double> &push,  const std::vector<double> &pop): push_(push), pop_(pop) {}
    const std::vector<double>& get_push() const {return push_;}
    const std::vector<double>& get_pop() const {return pop_;}

  private:
    std::vector<double> push_;
    std::vector<double> pop_;
};

class History
{
  public:
    void add_change(const Change &change) {changes_.push_back(change);}
    bool empty() const {return changes_.empty();}
    Change pop_change() {if (changes_.empty()) throw int(); Change val=changes_.back(); changes_.pop_back(); return val;}
    int size() const {return changes_.size();}

  private:
    std::vector<Change> changes_;
};

class RPN
{
  DisplayI &d;
  ValueStack stack;
  std::string entry;
  History history_;

  void enter(bool implicit);
  void two_input_operator(const std::function<double(double,double) > &lambda);
  void one_input_operator(const std::function<double(double) > &lambda);
  void drop();
  void undo();
  

  public:
  const std::vector<double>& get_stack() const {return stack.get_values();}
  RPN(DisplayI &d) : d(d) {}
  void process_input(char ch);
};

#endif
