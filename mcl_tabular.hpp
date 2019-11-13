#ifndef MCL_TABULAR_HPP
#define MCL_TABULAR_HPP

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <numeric>
#include <vector>
#include <list>

namespace mc
{    
    typedef std::vector<std::string> row_t;
    
    struct horizontal_line
    {
        horizontal_line(char c = '-') : m_char(c) { }
        char m_char;
        std::size_t position;
    };
    
    struct _table_creator
    {
        _table_creator(std::list<row_t> &rows, std::vector<horizontal_line> &hor_lines) : m_rows(rows), m_hor_lines(hor_lines) { }
        
        template<class type_t>
        _table_creator &operator()(const type_t &value)
        {
            std::stringstream ss;
            ss << value;
            m_current_row.push_back(ss.str());
            
            m_rows.push_back(m_current_row);
            m_current_row.clear();
            
            return *this;
        }
        
        template<class type_t, class ... types_t>
        _table_creator operator()(const type_t &value, const types_t& ... remaining)
        {
            std::stringstream ss;
            ss << value;
            m_current_row.push_back(ss.str());
            
            this->operator()(remaining ...);
            return *this;
        }
        
        _table_creator operator()(horizontal_line hor_line)
        {
            hor_line.position = m_rows.size();
            m_hor_lines.push_back(hor_line);
            
            return *this;
        }
    
        std::vector<std::string> m_current_row;
        std::list<row_t> &m_rows;
        std::vector<horizontal_line> &m_hor_lines;
    };
    
    class table
    {
    public:
        
        table() : m_creator(m_rows, m_hor_lines) { }
        
        _table_creator &create()
        {
            m_rows.clear();
            return m_creator;
        }
        
        void print()
        {
            prepare_table();
            
            std::size_t i=0;
            for( auto row = m_rows.begin(); row != m_rows.end(); ++row, ++i )
            {
                // Does horizontal line exist?
                auto hor_line_it = std::find_if(m_hor_lines.begin(), m_hor_lines.end(),
                                                [&](const horizontal_line &l){ return i == l.position; });
                
                if( hor_line_it != m_hor_lines.end() )
                    std::cout << std::string(m_row_length, hor_line_it->m_char) << std::endl;
                
                // Print line
                if( m_has_left_delim ) std::cout << m_vert_delim;
                
                for( auto cell_it = row->begin(); cell_it != row->end(); ++cell_it )
                {
                    std::cout << m_cell_padding << *cell_it << m_cell_padding;
                    
                    if( cell_it != row->end()-1 )
                        std::cout << m_vert_delim;
                }
                
                if( m_has_right_delim ) std::cout << m_vert_delim;
                
                std::cout << std::endl;
            }
            
            // Does bottom line exist?
            auto hor_line_it = std::find_if(m_hor_lines.begin(), m_hor_lines.end(), 
                                            [&](const horizontal_line &l){ return i == l.position; });
                
            if( hor_line_it != m_hor_lines.end() )
                std::cout << std::string(m_row_length, hor_line_it->m_char) << std::endl;
        }
        
        void set_vertical_delimiter(std::string delim) { m_vert_delim = delim; }
        void set_padding(int size) { m_cell_padding = size; }
        void add_left_border() { m_has_left_delim = true; }
        void add_right_border() { m_has_right_delim = true; }
        void global_cell_width() { m_has_gobal_cell_width = true; }
        
    private:
        void prepare_table()
        {
            // Calculate different collum widths
            auto collums = 
                std::max_element(m_rows.begin(), m_rows.end(), [](row_t &a, row_t &b){ return a.size() < b.size(); })->size();
            
            std::vector<std::size_t> collumn_widths(collums, 0);
            
            for( auto &row : m_rows )
            {
                // adjust row lenghts
                row.resize(collums);
            
                // search max collumn widht
                for(std::size_t i=0; i<collums; ++i)
                {
                    collumn_widths[i] = std::max(collumn_widths[i], row[i].size());
                }
            }
            
            auto global_cell_width_max = *std::max_element(collumn_widths.begin(), collumn_widths.end());
            
            for( auto &row : m_rows )
            {
                for(std::size_t i=0; i<collums; ++i)
                {
                    if( m_has_gobal_cell_width == true )
                        row[i].resize( global_cell_width_max, ' ');
                    else
                        row[i].resize( collumn_widths[i], ' ');
                }
            }
            
            // Calculate global rowlength
            if( m_has_gobal_cell_width == false )
                m_row_length = std::accumulate(collumn_widths.begin(), collumn_widths.end(), 0ul);
            else
                m_row_length = collums * global_cell_width_max;
            
            // add padding
            m_row_length += 2 * collums * m_cell_padding.size();
            
            // add central delimiter
            m_row_length += (collums - 1) * m_vert_delim.size();
            
            // add optional left and right delimiters
            if( m_has_left_delim )  m_row_length += m_vert_delim.size();
            if( m_has_right_delim ) m_row_length += m_vert_delim.size();
        }
        
        std::list<row_t> m_rows;
        std::vector<horizontal_line> m_hor_lines;
        _table_creator m_creator;
        
        std::size_t m_row_length;
        std::string m_vert_delim{"|"};
        std::string m_cell_padding{" "};
        
        // options
        bool m_has_gobal_cell_width = false;
        bool m_has_left_delim = false;
        bool m_has_right_delim = false;
    };
}

#endif
