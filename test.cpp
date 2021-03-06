#if __cplusplus < 201103L
    #error "mcl requires C++11"
#endif

#include "mcl_basic.hpp"
#include "mcl_arithmetic.hpp"
#include "mcl_tabular.hpp"
#include "mcl_program_options.hpp"

#if __cplusplus >= 201703L
    #include "mcl_python_like.hpp"
#endif


#include <vector>
#include <array>
#include <list>
#include <iostream>
#include <utility>
#include <thread>

using namespace std::chrono_literals;

void test_mathematical()
{
    std::cout << "TEST MATHEMATICAL FUNCTIONS:" << std::endl;
    
    std::vector<double> vector = { -3, -2, -1, 0, 1, 2, 3, 4 };
    
    mc::print_container(vector, "original vector");
    std::cout << "average =            " << mc::average(vector) << std::endl;
    std::cout << "standard_deviation = " << mc::standard_deviation(vector) << std::endl;
    std::cout << "squared vector =     " << mc::stringify_container(mc::square_container(vector)) << std::endl;
    std::cout << "abs vector =         " << mc::stringify_container(mc::abs_container(vector)) << std::endl;
    
    std::cout << std::endl;
}  

void test_export()
{
    std::cout << "TEST VECTOR EXPORT:" << std::endl;
    
    std::vector<double> vectorA(5, 3.5);
    std::list<int> listB(5,  4);
    std::vector<std::string> vectorC(5, "test");

    mc::clear_file("three_vec.txt");
    mc::export_containers("three_vec.txt", { "A", "B", "C" }, vectorA, listB, vectorC);
    
    std::cout << "(check file three_vec.txt)" << std::endl;
    std::cout << std::endl;
}

void test_table()
{
    std::cout << "TEST TABULAR:" << std::endl;
    
    mc::table new_table;
    new_table.create()
    (mc::horizontal_line('-'))
    ("A", "BBBBccccB", "C", "D")
    (mc::horizontal_line('='))
    (4.5, 3.5, 2.5, 1.5)
    ('a', 'b', 'c', 'd')
    ("1",  10, 100, 1000)
    (mc::horizontal_line('-'));
    
    new_table.add_left_border();
    new_table.add_right_border();
    new_table.print();
    
    std::cout << std::endl;
}

void test_program_options(int argc, char ** argv)
{
    std::cout << "TEST PROGRAM OPTIONS:" << std::endl;
    
    if( mc::option_exists("--help", argc, argv) ) 
        std::cout << "'--help' exists" << std::endl;
    else 
        std::cout << "'--help' doesn't exists" << std::endl;
    
    auto int_vector = mc::option_get_values<int>("--vectori", argc, argv);
    if( int_vector && !int_vector->empty() ) std::cout << "found int vector, contains " << mc::stringify_container(*int_vector) << std::endl;
    
    auto single_double = mc::option_get_value<double>("--singled", argc, argv);
    if( single_double ) std::cout << "found single double " << *single_double << std::endl;
    
    std::cout << std::endl;
}

void test_time_measure()
{
    std::cout << "TEST TIME MEASURE:" << std::endl;
    std::cout << "execute function  [](){ std::this_thread::sleep_for(2ms); }  10 times and measure time (should be 20ms = 0.02s):" << std::endl;
    auto time = mc::measure_time([](){ std::this_thread::sleep_for(2ms); }, 10);
    std::cout << "result: " << time << "s" << std::endl;
    
    std::cout << std::endl;
}

void test_python_like()
{
    std::cout << "TEST PYTHON LIKE:" << std::endl;
#if __cplusplus >= 201703L
    std::vector<std::string> a = { "v1", "v2", "v3", "v4" };
    const std::list<std::string> b = { "l1", "l2", "l3", "l4" };
    
    std::cout << "enumerate:" << std::endl;
    
    for( auto [i, txt] : mc::enumerate(a) )
        std::cout << i << ": " << txt << std::endl;
    std::cout << std::endl;
    
    
    std::cout << "zip:" << std::endl;
    
    for( auto [str1, str2] : mc::zip(a,b) )
        std::cout << str1 << ", " << str2 << std::endl;
    std::cout << std::endl;
    
    
    std::cout << "zip_enumerate:" << std::endl;
    
    for( auto [i, str1, str2] : mc::zip_enumerate(a,b) )
        std::cout << i << ": " << str1 << ", " << str2 << std::endl;
    std::cout << std::endl;
    
#else
    std::cout << "python_like is not supported (requires C++17)" << std::endl;
    std::cout << std::endl;
#endif
}

int main(int argc, char ** argv)
{
    std::cout << "TEST MCL LIBRARY" << std::endl;
    std::cout << "================" << std::endl << std::endl;
    test_mathematical();
    test_export();
    test_table();
    test_program_options(argc, argv);
    test_time_measure();
    test_python_like();
}

