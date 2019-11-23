#ifndef MCL_BASIC_HPP
#define MCL_BASIC_HPP


#include <stdexcept>
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
    

namespace mc
{
    // Container export functions
    // ==========================

    template<class container_t>
    inline std::string stringify_container(const container_t &c)
    {
        std::stringstream stream;
        
        stream << "[ ";
        for(auto it = c.begin(); it != c.end(); it++)
        {
            stream << *it << " ";
        }
        stream << "]";
        
        return stream.str();
    }

    template<class container_t>
    inline void print_container(const container_t &c, std::string name = "")
    {
        std::string name_str = name.empty() ? "" : name + " = ";
        std::cout << name_str << stringify_container(c) << std::endl;
    }
    
    inline void export_containers(std::string filename, std::vector<std::string> headers) 
    {
        if( headers.size() != 0 )
            throw std::runtime_error("number of headers must match number of containers");
    }
        
    template<class container_t, class ... containers_t>
    inline void export_containers(std::string filename, std::vector<std::string> headers, 
                                  const container_t &container, const containers_t& ... remaining)
    {
        std::ifstream old_file(filename, std::ios::in);
        std::stringstream file_buffer;
        
        // file is empty
        if(old_file.peek() == std::ifstream::traits_type::eof() )
        {
            // write first header and erase afterwards
            file_buffer << headers.front() << '\n';
            headers.erase(headers.begin());
            
            // write data of first container
            for(auto &el : container)
            {
                file_buffer << el << '\n';
            }
        }
        else
        {
            bool has_header = false;
            auto it = container.begin();
            
            for(char next = old_file.get(); !old_file.eof(); old_file.get(next))
            {
                if( next == '\n' )
                {
                    if( !has_header )
                    {
                        if( !headers.empty() )
                            file_buffer << '\t' << headers.front();
                        else
                            throw std::runtime_error("number of headers must match number of containers");
                        
                        headers.erase(headers.begin());
                        has_header = true;
                    }
                    else
                    {
                        if( it != container.end() )
                            file_buffer << '\t' << *it++;
                        else
                            throw std::runtime_error("vector sizes do not match!");
                    }
                }
                file_buffer << next;
            }
        }
        
        old_file.close();
        
        std::ofstream new_file(filename, std::ios::out  | std::ios::trunc);
        new_file << file_buffer.str();
        new_file.close();
        
        export_containers(filename, headers, remaining...);
    }
    
    inline void clear_file(std::string filename)
    {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);
    }
    
    // Generate ranges
    // ===============
    
    template<typename T>
    inline std::vector<T> range(T start, T step, int steps)
    {
        std::vector<T> range;
        
        for(int i{0}; i != steps; ++i)
        {
            range.push_back(start + i * step);
        }
        
        return range;
    }
    
    // Templated string conversion functions
    // =====================================
    
    template<typename T> inline T convert(const std::string &str){ static_assert("type not implemented for conversion"); }
    template<> inline float convert<float>(const std::string &str) { return static_cast<float>(std::atof(str.data())); }
    template<> inline double convert<double>(const std::string &str) { return std::atof(str.data()); }
    template<> inline int convert<int>(const std::string &str) { return std::atoi(str.data()); }
    template<> inline std::string convert(const std::string &str) { return str; }
    template<> inline bool convert(const std::string &str)
    {
        if( str == "true" || str == "True" || str == "TRUE" )
            return true;
        else if( str == "false" || str == "False" || str == "FALSE" )
            return false;
        else
            return static_cast<bool>(std::atoi(str.data()));
    }
    
    // Function time measuring
    // =======================
    
    template< typename function_object_t>
    double measure_time(function_object_t func, std::size_t iterations)
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for(std::size_t i=0; i<iterations; ++i)
            func();
        
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> elapsed = end - start;
        
        return elapsed.count() / iterations;
    }
    
}

#endif // AVERAGE_HPP
