#include "Poly_Int_Pair_v3.hpp"

namespace int_pair_v3{

    //Point8 start
    // __m256i Point8::get_vector_mask() const{
    //     //vector[255 : 0] = 32 char
    //     //(0 to 31) char i -> ei
    //     //ever char = _x
    //     //odd  char = _y
    //     constexpr std::size_t SIZE = 32;
    //     _int_t mask[SIZE];
    //     for(std::size_t i = 0; i < SIZE; ++i){
    //         mask[i] = (i % 2 == 0) ? _x : _y;
    //     }
    //     return _mm256_loadu_si256(reinterpret_cast<__m256i*>(mask));
    // }

    std::array<Point8::self_t, 4> Point8::adjacent_points() const{
        return {
            self_t(_x, _y+1),
            self_t(_x+1, _y),
            self_t(_x, _y-1),
            self_t(_x-1, _y)
        };
    }

    std::array<Point8::self_t, 8> Point8::transformed_points() const{
        return {
            self_t(_x, _y),
            get_rotate_clockwise_90(),
            get_rotate_clockwise_180(),
            get_rotate_clockwise_270(),
            get_reflect_line_0(),
            get_reflect_line_45(),
            get_reflect_line_90(),
            get_reflect_line_135()
        };
    }

    Point8::self_t Point8::get_rotate_clockwise_90() const{
        return self_t(_y, - _x);
    }
    Point8::self_t Point8::get_rotate_clockwise_180() const{
        return self_t(- _x, - _y);
    }
    Point8::self_t Point8::get_rotate_clockwise_270() const{
        return self_t(- _y, _x);
    }
    
    Point8::self_t Point8::get_reflect_line_0() const{
        return self_t(_x, - _y);
    }
    Point8::self_t Point8::get_reflect_line_45() const{
        return self_t(_y, _x);
    }
    Point8::self_t Point8::get_reflect_line_90() const{
        return self_t(- _x, _y);
    }
    Point8::self_t Point8::get_reflect_line_135() const{
        return self_t(- _y, - _x);
    }

    Point8::hash_t Point8::point_to_hash() const{
        hash_t a = static_cast<hash_t>(static_cast<_uint_t>(_x));
        hash_t b = static_cast<hash_t>(static_cast<_uint_t>(_y));
        return (a << HASH_SHIFT) | b;
    }
    std::string Point8::str() const{
        std::string s;
        s.reserve(16);
        s.push_back('(');
        s += std::to_string(_x);
        s.push_back(',');
        s += std::to_string(_y);
        s.push_back(')');
        s.push_back(' ');
        return s;
    }

    bool Point8::is_equal(const self_t a, const self_t b){
        return (a._x == b._x && a._y == b._y);
    }
    bool Point8::is_not_equal(const self_t a, const self_t b){
        return !is_equal(a, b);
    }
    bool Point8::is_less_than(const self_t a, const self_t b){
        if(a._y < b._y){
            return true;
        }
        else if(a._y == b._y && a._x < b._x){
            return true;
        }
        else{
            return false;
        }
    }
    bool Point8::is_great_than(const self_t a, const self_t b){
        return !is_less_than(a, b);
    }
    //Point8 end

    //fixed_poly

    void fixed_Polyomino::grow_with_child(point_t child){
        if(!_empty_cells.contains(child)){
            std::cout<<"\nNot contain "<<child.str();
            return;
        }

        _empty_cells.erase(child);
        _cells.emplace(child);

        auto adj_array = child.adjacent_points();
        for(const point_t adj : adj_array){
            //emplace only success if _cells not contain adj
            if(!_cells.contains(adj)){
                _empty_cells.emplace(adj);
            }
        }
    }

    fixed_Polyomino::point_t fixed_Polyomino::shift_to_origin(){
        // ready auto vectorised
        point_t offset = *_cells.begin();
        if(offset._x == 0 && offset._y == 0){
            return offset;
        }

        for(auto i = _cells.begin(); i != _cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        for(auto i = _empty_cells.begin(); i != _empty_cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        return offset;
    }

    fixed_Polyomino::point_t fixed_Polyomino::grow_and_shift(point_t child){
        grow_with_child(child);
        return shift_to_origin();
    }

    fixed_Polyomino::hash_t fixed_Polyomino::poly_to_hash() const{
        using My_Hash = Hash_Function_64;

        hash_t result = My_Hash::mix_hash(
            My_Hash::normal_hash(_cells.size()),
            My_Hash::normal_hash(_empty_cells.size())
        );

        auto* cell_ptr = &*_cells.begin();
        std::size_t cell_n = _cells.size() * sizeof(point_t);

        result = My_Hash::mix_hash(
            result,
            My_Hash::xxhash(cell_ptr, cell_n)
        );

        auto* empty_ptr = &*_empty_cells.begin();
        std::size_t empty_n = _empty_cells.size() * sizeof(point_t);

        result = My_Hash::mix_hash(
            result,
            My_Hash::xxhash(empty_ptr, empty_n)
        );

        return result;
    }

    void fixed_Polyomino::base_case(){
        point_t base(0, 0);

        _cells.emplace(base);
        
        for(const point_t p : base.adjacent_points()){
            _empty_cells.emplace(p);
        }
    }

    void fixed_Polyomino::print_size() const{
        std::cout   << "\n"
                    << "\t# Cells = " << _cells.size()
                    << "\t# Empty_Cells = " << _empty_cells.size();
    }
    void fixed_Polyomino::print(const char* text) const{
        print_size();
        std::cout<<	"\n\t"<<text<<"\t# Cells: "<<
            _cells.size()<<"\t";
        int count = 5;
        for(point_t p : _cells){
            if(count == 5){
                std::cout<<"\n\t\t";
                count = 0;
            }
            count++;
            std::cout << p.str() << "\t";
        }

        std::cout<<	"\n\t"<<text<<"\t# Empty_Cells: "<<
            _empty_cells.size()<<"\t";
        count = 5;
        for(point_t p : _empty_cells){
            if(count == 5){
                std::cout<<"\n\t\t";
                count = 0;
            }
            count++;
            std::cout << p.str() << "\t";
        }
    }
    void fixed_Polyomino::print_py_image() const{
        std::cout<<"\nnon_empty = [";
        for(point_t p : _cells){
            std::cout<<"\n"<<p.str()<<",";
        }
        std::cout<<"\n]";

        std::cout<<"\nempty = [";
        for(point_t p : _empty_cells){
            std::cout<<"\n"<<p.str()<<",";
        }
        std::cout<<"\n]\n";
    }

    //fixed_poly

    //free_poly

    std::array<free_Polyomino::hash_t, 8> free_Polyomino::fixed_class_hash() const{
        return {
            _fixed_class[0].poly_to_hash(),
            _fixed_class[1].poly_to_hash(),
            _fixed_class[2].poly_to_hash(),
            _fixed_class[3].poly_to_hash(),
            _fixed_class[4].poly_to_hash(),
            _fixed_class[5].poly_to_hash(),
            _fixed_class[6].poly_to_hash(),
            _fixed_class[7].poly_to_hash(),
        };
    }

    void free_Polyomino::print() const{
        std::cout   << "\nFree_Poly [parent id, child num, sym_mask] = [ "
                    << _parent_id << ", " << static_cast<int>(_num_child) << ", " << static_cast<int>(_sym_mask) << " ]";
        for(std::size_t i = 0; i < 8; i++){
            std::cout << "\n\tFixed_Poly offset = " << _offset[i].str();
            _fixed_class[i].print();
            std::cout << "\n";
        }
        
    }

    void free_Polyomino::grow_and_shift(point_t child){
        auto trans_array = (child + _offset[0]).transformed_points();
        // for(int i = 0; i < 8; i++){
        //     point_t s = (_offset[0] - _offset[i]);
        //     point_t p = trans_array[i] + (_offset[0] - _offset[i]);
        //     std::cout << "\nold t = " << trans_array[i].str() << "\ts = " << s.str() << "\tp = " << p.str();
        // }
        for(int i = 0; i < 8; i++){
            point_t p = trans_array[i] - _offset[i];
            // std::cout <<    "\nT(p + s_a) = " << trans_array[i].str() <<
            //                 "\tshift b = " << _offset[i].str()  <<
            //                 "\tT(p + s_a) - s_b = " << p.str();
            _offset[i] = _offset[i] + _fixed_class[i].grow_and_shift(p);
        }
    }

    //free_poly


    //memo_poly start

    void memo_Polyomino::generate_adj_empty_cells(){
        constexpr bool EMPTY_CELL = false;
        constexpr bool NON_EMPTY_CELL = true;
        
        boost::unordered::unordered_node_map<point_t::hash_t, bool> table;
        table.reserve(_cells.size() * 4);

        for(const point_t p : _cells){
            auto adj_array = p.adjacent_points();

            auto search = table.find(p.point_to_hash());
            if(search != table.end()){
                if(search->second == EMPTY_CELL){
                    search->second = NON_EMPTY_CELL;
                }
            }
            else{
                table.insert({p.point_to_hash(), NON_EMPTY_CELL});
            }

            for(const point_t adj : adj_array){
                auto search = table.find(adj.point_to_hash());
                if(search == table.end()){
                    table.insert({adj.point_to_hash(), EMPTY_CELL});
                }
            }
        }

        for(const auto& item : table){
            if(item.second == EMPTY_CELL){
                _empty_cells.emplace(point_t(item.first));
            }
        }
    }

    memo_Polyomino::point_t memo_Polyomino::shift_to_origin(){
        // ready auto vectorised
        point_t offset = *_cells.begin();
        if(offset._x == 0 && offset._y == 0){
            return offset;
        }

        for(auto i = _cells.begin(); i != _cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        for(auto i = _empty_cells.begin(); i != _empty_cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        return offset;
    }

    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_90() const{
        //assume all the point are unique
        //create two temp vector
        //index iterating to copy data into vector
        //sort the vector and insert inorderly into result flat_set
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_90();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_90();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;

        // Need O(N log N) binary search, O(N) shift with complex duplication check
        // Super bad compare with above solution

        // self_t result(_cells.capacity(), _empty_cells.capacity());
        // for(point_t p : _cells){
        //     result._cells.emplace(p.get_rotate_clockwise_90());
        // }
        // for(point_t p : _empty_cells){
        //     result._empty_cells.emplace(p.get_rotate_clockwise_90());
        // }
        // return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_180() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_180();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_180();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_270() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_270();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_270();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_0() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_0();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_0();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_45() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_45();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_45();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_90() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_90();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_90();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_135() const{
        self_t result(_cells.capacity(), _empty_cells.capacity());

        std::size_t ne_size = _cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_135();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_135();
        }

        sort_and_insert_set(result._cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }

    memo_Polyomino::hash_t memo_Polyomino::poly_to_hash() const{
        auto mix_hash = [](hash_t x){
            x += 0x9e3779b97f4a7c15ull;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
            x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
            return x ^ (x >> 31);
        };

        hash_t result = mix_hash(_cells.size() + _empty_cells.size());
        hash_t offset = 1;
        for(point_t p : _cells){
            // std::cout << "\n hash = " << result;
            result = mix_hash(result + p.point_to_hash() + offset);
            ++offset;
        }
        for(point_t p : _empty_cells){
            // std::cout << "\n hash = " << result;
            result = mix_hash(result + p.point_to_hash() + offset);
            ++offset;
        }
        return result;
    }

    void memo_Polyomino::print_size() const{
        std::cout   << "\n\t{parent, # child} = " 
                    << _parent_id << ", " << _num_child
                    << "\t# Cells = " << _cells.size()
                    << "\t# Empty_Cells = " << _empty_cells.size();
    }
    void memo_Polyomino::print(const char* text) const{
        print_size();
        std::cout<<	"\n\t"<<text<<"\t# Cells: "<<
            _cells.size()<<"\t";
        int count = 5;
        for(point_t p : _cells){
            if(count == 5){
                std::cout<<"\n\t\t";
                count = 0;
            }
            count++;
            std::cout << p.str() << "\t";
        }

        std::cout<<	"\n\t"<<text<<"\t# Empty_Cells: "<<
            _empty_cells.size()<<"\t";
        count = 5;
        for(point_t p : _empty_cells){
            if(count == 5){
                std::cout<<"\n\t\t";
                count = 0;
            }
            count++;
            std::cout << p.str() << "\t";
        }
    }
    void memo_Polyomino::print_py_image() const{
        std::cout<<"\nnon_empty = [";
        for(point_t p : _cells){
            std::cout<<"\n"<<p.str();
        }
        std::cout<<"\n]";

        std::cout<<"\nempty = [";
        for(point_t p : _empty_cells){
            std::cout<<"\n"<<p.str();
        }
        std::cout<<"\n]\n";
    }

    memo_Polyomino::self_t memo_Polyomino::base_case(){
        memo_Polyomino result;

        point_t base(0, 0);
        result._cells.emplace(0, 0);

        for(point_t p : base.adjacent_points()){
            result._empty_cells.emplace(p);
        }

        return result;
    }
    //memo_poly end

    //sym_mask

    symmetry_mask::mask_t symmetry_mask::set_sym(const poly_t& target){
        mask_t result = 0 | sym_action::mask_r0;

        for(std::size_t i = 1; i < 8; ++i){
            if(compare_fixed_poly(target._fixed_class[0], target._fixed_class[i])){
                result |= sym_action::mask_array[i-1];
            }
        }

        return result;
    }

    symmetry_mask::set_t symmetry_mask::get_unique_child(const poly_t &target){
        set_t result = target._fixed_class[0]._empty_cells;
        mask_t temp = target._sym_mask;

        auto uniquify = [&](std::span<const index_t> mapping){
            // for(auto p : result){
            //     std::cout << "\n\t" << p.str();
            // }

            for(auto it = result.begin(); it != result.end(); ){
                auto trans_array = (*it).transformed_points();
                
                for(const index_t index : mapping){
                    // std::cout << "\nit = "<<it->str();

                    point_t product = trans_array[index];
                    product._x -= target._offset[index]._x;
                    product._y -= target._offset[index]._y;

                    if(point_t::is_not_equal(*it, product)){

                        // std::cout <<"\ttran = "<<product.str();
                        result.erase(product);
                    }
                    
                }
                it++;
            }
        };


        switch(temp){
            case sym_group::mask_trivial_g8:
                break;
            
            case sym_group::mask_c2_1_g4:{
                uniquify(sym_group_transformed_mapping::mapping_c2_1_g4);
                break;
            }

            case sym_group::mask_c2_2_g4:{
                uniquify(sym_group_transformed_mapping::mapping_c2_2_g4);
                break;
            }

            case sym_group::mask_c2_3_g4:{
                uniquify(sym_group_transformed_mapping::mapping_c2_3_g4);
                break;
            }

            case sym_group::mask_c2_4_g4:{
                uniquify(sym_group_transformed_mapping::mapping_c2_4_g4);
                break;
            }

            case sym_group::mask_f2_g4:{
                uniquify(sym_group_transformed_mapping::mapping_f2_g4);
                break;
            }

            case sym_group::mask_d2_1_g2:{
                uniquify(sym_group_transformed_mapping::mapping_d2_1_g2);
                break;
            }

            case sym_group::mask_d2_2_g2:{
                uniquify(sym_group_transformed_mapping::mapping_d2_2_g2);
                break;
            }

            case sym_group::mask_c4_g2:{
                uniquify(sym_group_transformed_mapping::mapping_c4_g2);
                break;
            }

            case sym_group::mask_d4_g1:{
                uniquify(sym_group_transformed_mapping::mapping_d4_g1);
                break;
            }

            default:
                break;
        }

        return result;
    }

    //sym_mask

    void sort_and_insert_set(memo_Polyomino::_set_t& flat_set, std::vector<memo_Polyomino::point_t>& vector){
        std::sort(vector.begin(), vector.end(), 	
            [](const Point8 a, const Point8 b){
                if(a._y < b._y){return true;}
                else if(a._y == b._y && a._x < b._x){return true;}
                else{return false;}
            }
        );
        flat_set.insert(boost::container::ordered_unique_range, vector.begin(), vector.end());
    }

    bool compare_translation(const memo_Polyomino& first, const memo_Polyomino& second){
        if(first._cells.size() != second._cells.size()){
            return false;
        }

        std::size_t n = first._cells.size() * sizeof(memo_Polyomino::point_t);
        return std::memcmp(
            &(*first._cells.cbegin()),
            &(*second._cells.cbegin()),
            n) == 0;
    }

    bool compare_fixed_poly(const fixed_Polyomino &first, const fixed_Polyomino &second){
        if(first._cells.size() != second._cells.size()){
            return false;
        }

        std::size_t n = first._cells.size() * sizeof(fixed_Polyomino::point_t);
        return std::memcmp(
            &(*first._cells.cbegin()),
            &(*second._cells.cbegin()),
            n) == 0;
    }
}