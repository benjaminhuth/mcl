#include "mcl_basic.hpp"
#include "mcl_tabular.hpp"
#include "mcl_program_options.hpp"

#include <vector>
#include <array>
#include <iostream>
#include <utility>

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
    
    std::vector<double> vectorA(10, 3.5), vectorB(10,  4.5);
    std::vector<std::string> vectorC(10, "testvec");

    mc::clear_file("three_vec.txt");
    mc::export_containers("three_vec.txt", { "A", "B", "C" }, vectorA, vectorB, vectorC);
    
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
    
    if( mc::option_exists("--help", argc, argv) ) std::cout << "--help exists" << std::endl;
    else std::cout << "--help doesn't exists" << std::endl;
    
    auto int_vector = mc::option_get_values<int>("--vectori", argc, argv).value_or(std::vector<int>());
    auto double_vec = mc::option_get_values<double>("--vectord", argc, argv).value_or(std::vector<double>());
    
    if( !int_vector.empty() ) std::cout << "found int vector, contains " << mc::stringify_container(int_vector) << std::endl;
    if( !int_vector.empty() ) std::cout << "found double vector, contains " << mc::stringify_container(double_vec) << std::endl;
    
    std::cout << std::endl;
}

int main(int argc, char ** argv)
{
    std::cout << "TEST MCL LIBRARY" << std::endl;
    std::cout << "================" << std::endl << std::endl;
    test_mathematical();
    test_export();
    test_table();
    test_program_options(argc, argv);
}

