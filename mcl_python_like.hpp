#ifndef MCL_PYTHON_LIKE_HPP
#define MCL_PYTHON_LIKE_HPP

#if __cplusplus < 201703L
#error "error: 'mcl_python_like.hpp' requires C++17!"
#endif

#include <tuple>

namespace mc
{
    // Thanks to: http://reedbeta.com/blog/python-like-enumerate-in-cpp17
    
    template<typename iterable_t,
             typename iter_t = decltype(std::begin(std::declval<iterable_t>())),
             typename = decltype(std::end(std::declval<iterable_t>()))>
    constexpr auto enumerate( iterable_t && iterable)
    {
        struct iterator
        {
            std::size_t i;
            iter_t iter;
            bool operator != (const iterator & other) const { return iter != other.iter; }
            void operator ++ () { ++i; ++iter; }
            auto operator *  () const { return std::tie(i, *iter); }
        };
        
        struct iterable_wrapper
        {
            iterable_t iterable;
            auto begin() { return iterator{ 0, std::begin(iterable) }; }
            auto end() { return iterator{ 0, std::end(iterable) }; }
        };
        
        return iterable_wrapper{ std::forward<iterable_t>(iterable) };
    }
    
    // TODO const qualifiers on operator* ??
    
    // zip (inspired by above)
    template<class ... iterable_types>
    constexpr auto zip( iterable_types && ... types )
    {
        struct zip_iterator
        {
            using it_tuple_t = decltype(std::make_tuple(std::begin(types)...));
            using val_tuple_t = decltype(std::make_tuple((*std::begin(types))...));
            it_tuple_t iterator_tuple; 
            
            bool operator != (const zip_iterator & other) const 
            { 
                return iterator_tuple != other.iterator_tuple;
            }
            
            void operator ++ () 
            { 
                std::apply([&](auto & ... it){ std::make_tuple(++it...); }, iterator_tuple); 
            }
            
            auto operator *  ()
            {
                return std::apply([&](auto ... it){ return  std::make_tuple(*it...); }, iterator_tuple);
            }
        };
        
        struct iterable_wrapper
        {
            decltype(std::make_tuple(types...)) iterable_tuple;
            
            auto begin() 
            {                
                return zip_iterator{ std::apply([](auto & ... args){ return std::make_tuple(std::begin(args)...); }, iterable_tuple) };
            }
            
            auto end()
            {
                return zip_iterator{ std::apply([](auto & ... args){ return std::make_tuple(std::end(args)...); }, iterable_tuple) };
            }
        };
        
        return iterable_wrapper{ std::forward_as_tuple(types...) };
    }
        
    // Somehow code copy... not good, but easiest solution
        
    template<class ... iterable_types>
    constexpr auto zip_enumerate( iterable_types && ... types )
    {
        struct zip_en_iterator
        {
            std::size_t i;
            using it_tuple_t = decltype(std::make_tuple(std::begin(types)...));
            using val_tuple_t = decltype(std::make_tuple((*std::begin(types))...));
            it_tuple_t iterator_tuple; 
            
            bool operator != (const zip_en_iterator & other) const 
            { 
                return iterator_tuple != other.iterator_tuple;
            }
            
            void operator ++ () 
            { 
                ++i;
                std::apply([&](auto & ... it){ std::make_tuple(++it...); }, iterator_tuple); 
            }
            
            auto operator *  ()
            {
                auto val_tup = std::apply([&](auto ... it){ return  std::make_tuple(*it...); }, iterator_tuple);
                auto i_tup = std::make_tuple(i);
                return std::tuple_cat(i_tup, val_tup);
            }
        };
        
        struct iterable_wrapper
        {
            decltype(std::make_tuple(types...)) iterable_tuple;
            
            auto begin() 
            {                
                return zip_en_iterator{ 0, std::apply([](auto & ... args){ return std::make_tuple(std::begin(args)...); }, iterable_tuple) };
            }
            
            auto end()
            {
                return zip_en_iterator{ 0, std::apply([](auto & ... args){ return std::make_tuple(std::end(args)...); }, iterable_tuple) };
            }
        };
        
        return iterable_wrapper{ std::forward_as_tuple(types...) };
    }
}



#endif
