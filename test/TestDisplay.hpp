/*
Copyright 2019 Anders Bo Rasmussen

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
#include "Display.hpp"

class TestDisplay:public DisplayI
{
  public:
    void pop_line() override
    {
      lines_.pop_back();
    }
    void add_line(const std::string &s) override
    {
      lines_.pop_back();
      lines_.push_back(s);
      lines_.push_back("");
    }
    void add_char(char ch) override
    {
      (lines_.back())+=ch;
    }

    std::vector<std::string> get_lines() const
    {
      return lines_;
    }

    void remove_last_char() override
    {
      (*lines_.rbegin()).pop_back();
    }


  private:
    std::vector<std::string> lines_=std::vector<std::string>(1,"");
};
  
