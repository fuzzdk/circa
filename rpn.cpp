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
#include "rpn.hpp"
#include "Display.hpp"
#include <cmath>

void RPN::enter(bool implicit)
{
  if (entry.empty())
  {
    if (stack.empty() || implicit)
      return; //TODO: error?
    double v=stack.last();
    stack.push(v);
    d.add_line(std::to_string(v));
    Change change({v},{});
    history_.add_change(change);
    return;
  }
  double v=stack.push(entry);
  d.add_line(std::to_string(stack.last()));

  Change change({v},{});
  history_.add_change(change);
  entry="";
}

void RPN::two_input_operator(const std::function<double(double,double) > &lambda)
{
  enter(true);
  if (stack.size()<2)
    return;
  d.pop_line();
  d.pop_line();
  const double elem2=stack.pop();
  const double elem1=stack.pop();
  const double res=lambda(elem1,elem2);
  Change change({res},{elem1,elem2});
  history_.add_change(change);
  d.add_line(std::to_string(res));
  stack.push(res);
}

void RPN::one_input_operator(const std::function<double(double) > &lambda)
{
  enter(true);
  if (stack.size()<1)
    return;
  d.pop_line();
  const double elem=stack.pop();
  double res=lambda(elem);
  Change change({res},{elem});
  history_.add_change(change);
  d.add_line(std::to_string(res));
  stack.push(res);
}

void RPN::undo()
{
  enter(true);
  if (history_.empty())
    return;
  Change change=history_.pop_change();
  for(int i=0;i<change.get_push().size();++i)
  {
    d.pop_line();
    d.clear_line();
    stack.pop();
  }
  for(double push : change.get_pop())
  {
    d.add_line(std::to_string(push));
    stack.push(push);
  }
}

void RPN::drop()
{
  if (!entry.empty())
  {
    d.clear_line();
    entry.clear();
    return;
  }
  if (stack.size()<1)
    return;
  d.pop_line(); //line we are editing
  d.clear_line(); //element
  stack.pop(); 
}



void RPN::process_input(char ch)
{
  switch (ch)
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      if (ch=='.' && entry.find(".")!=std::string::npos)
        return;
      entry+=ch;
      d.add_char(ch);
      break;
    case 10: //enter
      enter(false);
      break;
    case '+':
      two_input_operator([](double a,double b){return a+b;});
      break;
    case '-':
      two_input_operator([](double a,double b){return a-b;});
      break;
    case '*':
      two_input_operator([](double a,double b){return a*b;});
      break;
    case '/':
      two_input_operator([](double a,double b){return a/b;});
      break;
    case '^':
      two_input_operator([](double a,double b){return std::pow(a,b);});
      break;
    case 'n':
      one_input_operator([](double a){return -a;});
      break;
    case 'u':
      undo();
      break;
    case '\\':
      drop();
      break;
 
    case 4: //ctrl-d
    case 27: //esc
      exit(0);
    case 8: //backspace
    case 127:
      if (entry.empty())
        return;
      entry.pop_back();
      d.remove_last_char();
  }
}

