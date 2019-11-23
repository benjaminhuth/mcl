#ifndef MCL_ARITHMETIC_HPP
#define MCL_ARITHMETIC_HPP

#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

namespace mc
{
    // Mathematical container functions
    // ================================
    
    // Average
    template<class iterator_t, class number_t = typename std::iterator_traits<iterator_t>::value_type>
    inline number_t average(const iterator_t begin, const iterator_t end)
    {
        static_assert( std::is_arithmetic<number_t>::value, "Container must store arithmetic type");
        
        auto sum = std::accumulate(begin, end, static_cast<number_t>(0));
        return sum / (end - begin);
    }
    
    template<class container_t, class number_t = typename container_t::value_type>
    inline number_t average(const container_t &data)
    {
        return average(data.begin(), data.end());
    }
    
    // Standard deviation
    template<class iterator_t, class number_t = typename std::iterator_traits<iterator_t>::value_type>
    inline number_t standard_deviation(const iterator_t begin, const iterator_t end)
    {
        static_assert( std::is_arithmetic<number_t>::value, "Container must store arithmetic type");
        
        auto data_average = average(begin, end);
        number_t sum = 0;
        
        for(auto it = begin; it != end; ++it) 
            sum += std::pow(*it - data_average, 2);
        
        return std::sqrt( sum / (end - begin));
    }
    
    template<class container_t, class number_t = typename container_t::value_type>
    inline number_t standard_deviation(const container_t &data)
    {
        return standard_deviation(data.begin(), data.end());
    }
    
    // Vector operations
    
    template<class iterator_t, 
             class container_t = std::vector<typename std::iterator_traits<iterator_t>::value_type>, 
             class number_t = typename std::iterator_traits<iterator_t>::value_type>
    inline container_t square_container(const iterator_t begin, const iterator_t end)
    {
        static_assert( std::is_arithmetic<number_t>::value, "Container must store arithmetic type");
        container_t squared_container(end - begin);
        
        std::transform(begin, end, squared_container.begin(), [](const number_t &el){ return el*el; });
        
        return squared_container;
    }
    
    template<class container_t , class number_t = typename container_t::value_type>
    inline container_t square_container(const container_t &data)
    {
        return square_container<typename container_t::const_iterator, container_t>(data.begin(), data.end());
    }
    
    template<class iterator_t, 
             class container_t = std::vector<typename std::iterator_traits<iterator_t>::value_type>, 
             class number_t = typename std::iterator_traits<iterator_t>::value_type>
    inline container_t abs_container(const iterator_t begin, const iterator_t end)
    {
        static_assert( std::is_arithmetic<number_t>::value, "Container must store arithmetic type");
        container_t retcont(end - begin);
        
        std::transform(begin, end, retcont.begin(), [](const number_t &el){ return std::abs(el); });
        
        return retcont;
    }

    template<class container_t , class number_t = typename container_t::value_type>
    inline container_t abs_container(const container_t &data)
    {
        return abs_container<typename container_t::const_iterator, container_t>(data.begin(), data.end());
    }
}

#endif
