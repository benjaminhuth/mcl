#ifndef MCL_PROGRAM_OPTIONS_HPP_INCLUDED
#define MCL_PROGRAM_OPTIONS_HPP_INCLUDED

#if __cplusplus < 201103L
    #error "mcl requires C++11"
#endif

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <type_traits>

#if __cplusplus >= 201703L
#include <optional>
template<typename T> using optional = std::optional<T>;
#else
#include "deps/optional/optional.hpp"
template<typename T> using optional = std::experimental::optional<T>;
#endif

#include "mcl_basic.hpp"

namespace mc
{
    inline bool option_exists(const std::string &option, int argc, char ** argv)
    {
        std::vector<std::string> args;
        args.assign(argv+1, argv+argc);
        
        return std::find(args.begin(), args.end(), option) != args.end();
    }
    
    template<typename T>
    inline optional<std::vector<T>> option_get_values(std::string option, int argc, char ** argv,
                                                      std::vector<std::string> option_markers = {"-", "--"})
    {        
        // check if an argument starts with one of the option markers
        auto has_marker = [&](std::string arg)
        {
            for(auto marker : option_markers)
            {
                if( arg.substr(0, marker.size()) == marker )
                    return true;
            }
            return false;
        };
        
        // fill options
        std::vector<std::string> args;
        args.assign(argv+1, argv+argc);
        
        auto found = std::find(args.begin(), args.end(), option);
        
        if( found == args.end() ) return optional<std::vector<T>>();
        
        // fill map
        std::vector<T> values;
        
        for(auto it=found+1; it != args.end() && !has_marker(*it); ++it)
        {
            values.push_back(convert<T>(*it));
        }
        
        return optional<std::vector<T>>(values);
    }
    
    template<typename T>
    inline optional<T> option_get_value(std::string option, int argc, char ** argv,
                                        std::vector<std::string> option_markers = {"-", "--"})
    {
        auto vec = option_get_values<T>(option, argc, argv, option_markers);
        
        if( vec && !vec->empty() )
            return vec->at(0);
        else
            return optional<T>();
    }
}   

#endif // MCL_PROGRAM_OPTIONS_HPP_INCLUDED
