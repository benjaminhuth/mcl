# mcl
C++ header-only library to support common actions with standard library

## Module 'python-like'

Implements two list operations known from python: `enumerate` and `zip`. Requires C++17!

```c++
std::vector<std::string> a = { "v1", "v2", "v3", "v4" };
const std::list<std::string> b = { "l1", "l2", "l3", "l4" };

for( auto [i, str] : mc::enumerate(a) )
    std::cout << i << " " << str << std::endl;

for( auto [str1, str2] : mc::zip(a,b) )
    std::cout << str << " " << num << std::endl;
```

C-style arrays (e.g. `double[4]`) are not supported at the moment. The implementations are inspired by [this implementation](http://reedbeta.com/blog/python-like-enumerate-in-cpp17).

## Module 'tabular'

Offers a simple possibility to create tables. It uses `<iostream>` and `std::cout` under the hood.

```c++
#include <mcl/mcl_tabular.hpp>

mc::table new_table; // table object

// this creates a proxy object, which consumes ()-operators
new_table.create()
(mc::horizontal_line('-'))
("A", "B", "C", "D")
(mc::horizontal_line('='))
(4.5, 3.5, 2.5, 1.5)
("1",  10, 100, 1000)
(mc::horizontal_line('-'));

// a few (very few) options for customization
new_table.add_left_border();
new_table.add_right_border();

// print the table
new_table.print();
```

The commandline output looks like this:

```
--------------------------
| A   | B   | C   | D    |
==========================
| 4.5 | 3.5 | 2.5 | 1.5  |
| a   | b   | c   | d    |
| 1   | 10  | 100 | 1000 |
--------------------------
```

## Module 'program options'

Offers a simple interface for evaluating program options. Not to powerful, but enough for simple applications. 

```c++
#include <mcl/mcl_program_options.hpp>

int main(int argc, char ** argv)
{
    if( mc::option_exists("--help", argc, argv) ) 
        std::cout << "help message" << std::endl;
        
    // provides std::optional<std::vector<int>> (caution: can be empty)
    auto int_vector = mc::option_get_values<int>("--vectori", argc, argv); 
    if( int_vector && !int_vector->empty() ) 
        std::cout << "vector<int> = " << mc::stringify_container(*int_vector) << std::endl;
    
    // provides std::optional<double>
    auto single_number = mc::option_get_value<double>("--singled", argc, argv); 
    if( single_double ) 
        std::cout << "number = " << *single_double << std::endl;
}
```

The functions `mc::option_get_value` and `mc::option_get_values` scan the arguments until the end of the list or until they find an argument which starts with `-` or `--` (by default). The above program can be used like this:

```
./a.out --vectori 1 2 3 --singled 3.14 --help
```

## Other functionality (Module 'basic' and 'arithmetic')

### Container export

Simple container-to-file export:

```c++
std::vector<double> vectorA(5, 3.5)
std::list<int> listB(5,  4);
std::vector<std::string> vectorC(5, "test");

mc::clear_file("three_vec.txt");
mc::export_containers("three_vec.txt", { "A", "B", "C" }, vectorA, listB, vectorC);
```

Output:
```
cat three_vec.txt
A       B       C
3.5     4       test
3.5     4       test
3.5     4       test
3.5     4       test
3.5     4       test
```

### Arithmetic operations

Some arithmetic operations for containers of integral type:

```c++
std::vector<double> vector;

auto average = mc::average(vector);
auto stddev  = mc::standard_deviation(vector);
auto sqrt    = mc::square_container(vector);
auto abs     = mc::abs_container(vector);
```
