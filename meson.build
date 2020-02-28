#Copyright 2019 Anders Bo Rasmussen

#This file is part of circa.

    #circa is free software: you can redistribute it and/or modify
    #it under the terms of the GNU General Public License as published by
    #the Free Software Foundation, either version 3 of the License, or
    #(at your option) any later version.

    #circa is distributed in the hope that it will be useful,
    #but WITHOUT ANY WARRANTY; without even the implied warranty of
    #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    #GNU General Public License for more details.

    #You should have received a copy of the GNU General Public License
    #along with circa.  If not, see <https://www.gnu.org/licenses/>.

project('circa', 'cpp', default_options : ['cpp_std=c++17'])

#boost_dep = dependency('boost')

add_global_arguments('-Wno-sign-compare', language : 'cpp')

common = static_library('common','rpn.cpp','Display.cpp')

executable('circa', 'main.cpp', link_with : common, install: true)
executable('mtest', 'test/main.cpp','test/tests.cpp', link_with : common)

