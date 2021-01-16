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
#include "catch.hpp"
#include "rpn.hpp"
#include "test/TestDisplay.hpp"

using std::vector;
using std::string;

struct Output
{
  vector<double> stack;
  vector<string> display;
};

Output process_input(const std::string &input)
{
  TestDisplay d;
  RPN r(d);
  for(char ch : input)
    r.process_input(ch);
  return Output{r.get_stack(),d.get_lines()};
}

TEST_CASE( "Display", "[display]" ) {
  REQUIRE(process_input("").display == vector<string>{""});
  REQUIRE(process_input("123").display == vector<string>{"123"});
  REQUIRE(process_input("123\n").display == vector<string>{"123.000000",""});
  REQUIRE(process_input("123\n45").display == vector<string>{"123.000000","45"});
  REQUIRE(process_input("2\n").display == vector<string>{"2.000000",""});
  REQUIRE(process_input("2\n\n").display == vector<string>{"2.000000","2.000000",""});
  REQUIRE(process_input("2\n2+").display == vector<string>{"4.000000",""});
}
TEST_CASE( "Input", "[input numbers]" ) {
  REQUIRE(process_input(".23").display == vector<string>{".23"});
  REQUIRE(process_input("1.2.3").display == vector<string>{"1.23"});
}

TEST_CASE( "Backspace", "[backspace]" ) {
  REQUIRE(process_input("1\b2\n").stack == vector<double>{2.0});
  REQUIRE(process_input("1\b2").display == vector<string>{"2"});
  REQUIRE(process_input("1\b\b\b").display == vector<string>{""});
  REQUIRE(process_input("1\b\b\b2\n3*").stack == vector<double>{6.0});
}


TEST_CASE( "Addition", "[addition]" ) {
  REQUIRE(process_input("2\n3+").stack == vector<double>{5.0});
  REQUIRE(process_input("2\n3++").stack == vector<double>{5.0});
}

TEST_CASE( "Subtraction", "[subtraction]" ) {
  REQUIRE(process_input("2\n3-").stack == vector<double>{-1.0});
}

TEST_CASE( "Division", "[division]" ) {
  REQUIRE(process_input("3\n2/").stack == vector<double>{1.5});
  REQUIRE(process_input("2\n0/").stack == vector<double>{std::numeric_limits<double>::infinity()});
  REQUIRE(process_input("2\n4-0/").stack == vector<double>{-std::numeric_limits<double>::infinity()});
}

TEST_CASE( "Multiplication", "[multiplication]" ) {
  REQUIRE(process_input("3\n2*").stack == vector<double>{6.0});
}

TEST_CASE( "Duplication", "[duplication]" ) {
  REQUIRE(process_input("2\n\n").stack == vector<double>{2.0,2.0});
}

TEST_CASE( "Power", "[power]" ) {
  REQUIRE(process_input("2\n8^").stack == vector<double>{256.0});
  REQUIRE(process_input("2\n0^").stack == vector<double>{1.0});
  REQUIRE(process_input("0\n1000^").stack == vector<double>{0.0});
}

TEST_CASE( "Negation", "[negation]" ) 
{
  REQUIRE(process_input("2n").stack == vector<double>{-2.0});
}


TEST_CASE( "Drop", "[drop element]" ) 
{
  REQUIRE(process_input("\\").stack == vector<double>{});
  REQUIRE(process_input("2\\").stack == vector<double>{});
  REQUIRE(process_input("2\\").display == vector<string>{""});
  REQUIRE(process_input("2\n\\").stack == vector<double>{});
  REQUIRE(process_input("2\n\\").display == vector<string>{""});
  REQUIRE(process_input("2\n3\\").stack == vector<double>{2});
  REQUIRE(process_input("2\n3\\").display == vector<string>{"2.000000",""});
}

TEST_CASE( "Undo 0", "[undo literal input]" ) {
REQUIRE(process_input("u").stack == vector<double>{});
REQUIRE(process_input("2\nu").stack == vector<double>{});
REQUIRE(process_input("2\n3\nu").stack == vector<double>{2.0});
}

TEST_CASE( "Undo 1", "[undo 1 input functions]" ) {
REQUIRE(process_input("1nu").stack == vector<double>{1.0});
REQUIRE(process_input("2\n\nu").stack == vector<double>{2.0});
REQUIRE(process_input("2\n\nuu").stack == vector<double>{});
}

TEST_CASE( "Undo 2", "[undo 2 input functions]" ) {
REQUIRE(process_input("2\n3+u").stack == vector<double>{2.0,3.0});
REQUIRE(process_input("1\n2\n3+u").stack == vector<double>{1.0,2.0,3.0});
}


TEST_CASE( "Undo multi level", "[undo multiple times]" ) {
REQUIRE(process_input("1nuu").stack == vector<double>{});
REQUIRE(process_input("1uu").stack == vector<double>{});
}
