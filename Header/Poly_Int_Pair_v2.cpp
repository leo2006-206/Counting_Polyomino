#include "Poly_Int_Pair_v2.hpp"
#include <array>


namespace int_pair_v2{

    //Point8 method Start
    bool Point8::operator ==(const Point8::self_t other) const{
        // if(_x == other._x && _y == other._y){
        //     return true;
        // }
        // return false;

        return (_x == other._x && _y == other._y);
    }
    bool Point8::operator !=(const self_t other)const{
        // if(_x == other._x && _y == other._y){
        //     return false;
        // }
        // return true;
        return !(_x == other._x && _y == other._y);
    }
    bool Point8::operator <(const self_t other)const{
        if(_y < other._y){
            return true;
        }
        else if(_y == other._y && _x < other._x){
            return true;
        }
        else{
            return false;
        }
    }
    
    Point8::self_t Point8::operator +(const self_t other) const{
        return self_t(_x + other._x, _y + other._y);
    }
    Point8::self_t Point8::operator -(const self_t other) const{
        return self_t(_x - other._x, _y - other._y);
    }
    Point8::self_t Point8::operator *(const self_t other) const{
        return self_t(_x * other._x, _y * other._y);
    }
    Point8::self_t Point8::operator /(const self_t other) const{
        return self_t(_x / other._x, _y / other._y);
    }

    Point8::self_t Point8::get_up() const{
        return self_t(_x, _y + 1);
    }
    Point8::self_t Point8::get_right() const{
        return self_t(_x + 1, _y);
    }
    Point8::self_t Point8::get_down() const{
        return self_t(_x, _y - 1);
    }
    Point8::self_t Point8::get_left() const{
        return self_t(_x - 1, _y);
    }
    std::array<Point8::self_t, 4> Point8::get_adjacent_points() const{
        return {
            get_up(),
            get_right(),
            get_down(),
            get_left()
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
    //Point8 method End

    //memo_Polyomino method Start
    memo_Polyomino::point_t memo_Polyomino::get_min_cell() const{
        return point_t(*_non_empty_cells.begin());
    }

    memo_Polyomino::point_t memo_Polyomino::shift_to_origin(){
        point_t offset = *_non_empty_cells.begin();
        if(offset._x == 0 && offset._y == 0){
            return offset;
        }
        for(auto i = _non_empty_cells.begin(); i != _non_empty_cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        for(auto i = _empty_cells.begin(); i != _empty_cells.end(); ++i){
            i->_x -= offset._x;
            i->_y -= offset._y;
        }
        return offset;
    }

    void memo_Polyomino::generate_adj_empty_cells(){
        constexpr bool EMPTY_CELL = false;
        constexpr bool NON_EMPTY_CELL = true;
        
        std::unordered_map<point_t::hash_t, bool> table;
        table.reserve(_non_empty_cells.size() * 4);

        for(const point_t p : _non_empty_cells){
            auto adj_array = p.get_adjacent_points();

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

    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_90() const{
        //assume all the point are unique
        //create two temp vector
        //index iterating to copy data into vector
        //sort the vector and insert inorderly into result flat_set
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_90();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_90();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;

        // Need O(N log N) binary search, O(N) shift with complex duplication check
        // Super bad compare with above solution

        // self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);
        // for(point_t p : _non_empty_cells){
        //     result._non_empty_cells.emplace(p.get_rotate_clockwise_90());
        // }
        // for(point_t p : _empty_cells){
        //     result._empty_cells.emplace(p.get_rotate_clockwise_90());
        // }
        // return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_180() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_180();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_180();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_rotate_clockwise_270() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_rotate_clockwise_270();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_rotate_clockwise_270();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_0() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_0();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_0();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_45() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_45();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_45();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_90() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_90();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_90();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }
    memo_Polyomino::self_t memo_Polyomino::get_reflect_line_135() const{
        self_t result(_non_empty_cells.capacity(), _empty_cells.capacity(), _parent_id, _num_child);

        std::size_t ne_size = _non_empty_cells.size();
        std::size_t e_size = _empty_cells.size();
        
        std::vector<point_t> temp_ne;
        std::vector<point_t> temp_e;

        temp_ne.resize(ne_size);
        temp_e.resize(e_size);

        auto ne_it = _non_empty_cells.begin();
        auto e_it = _empty_cells.begin();

        for (std::size_t i = 0; i < ne_size; ++i){
            temp_ne[i] = ne_it[i].get_reflect_line_135();
        }
        for (std::size_t i = 0; i < e_size; ++i){
            temp_e[i] = e_it[i].get_reflect_line_135();
        }

        sort_and_insert_set(result._non_empty_cells, temp_ne);
        sort_and_insert_set(result._empty_cells, temp_e);

        return result;
    }

    std::vector<memo_Polyomino::self_t> memo_Polyomino::generate_duplicated_child() const{
        std::vector<self_t> result;
        result.reserve(_non_empty_cells.size());

        for(const point_t p : _empty_cells){
            self_t copy(*this);
            std::array<point_t, 4> adjacents = p.get_adjacent_points();

            copy._empty_cells.erase(p);
            copy._non_empty_cells.emplace(p);

            for(const point_t new_empty_p : adjacents){
                if(!copy._non_empty_cells.contains(new_empty_p)){
                    copy._empty_cells.emplace(new_empty_p);
                }
            }
            result.emplace_back(copy);
        }

        return result;
    }

    std::vector<memo_Polyomino::self_t> memo_Polyomino::generate_unique_child() const{
        std::vector<self_t> result;

        symmetry_Poly mask;

        set childs = mask.generate_unique_child(*this);
        result.reserve(childs.size());
        for(const point_t p : childs){
            result.emplace_back(generate_new_child(p));
        }

        return result;
    }

    memo_Polyomino::self_t memo_Polyomino::generate_new_child(point_t p) const{
        if(!_empty_cells.contains(p)){
            return self_t();
        }
        self_t result;
        result._non_empty_cells = this->_non_empty_cells;
        result._empty_cells = this->_empty_cells;

        result._empty_cells.erase(p);
        result._non_empty_cells.emplace(p);

        auto adj_array = p.get_adjacent_points();
        for(const point_t adj_p : adj_array){
            if(!result._non_empty_cells.contains(adj_p)){
                result._empty_cells.emplace(adj_p);
            }
        }
        result.shift_to_origin();

        return result;
    }
    //memo_Polyomino method End

    //struct symmetry_Poly
    void symmetry_Poly::set_symmetry(const symmetry_Poly::poly_t& poly){
        std::array<poly_t, 7> transfrom = {
            poly.get_rotate_clockwise_90(),
            poly.get_rotate_clockwise_180(),
            poly.get_rotate_clockwise_270(),
            poly.get_reflect_line_0(),
            poly.get_reflect_line_45(),
            poly.get_reflect_line_90(),
            poly.get_reflect_line_135()
        };

        for(poly_t& p : transfrom){
            p.shift_to_origin();
        }

        _mask |= sym_mask::r0;

        for(int i = 0; i < 7; ++i){
            if(compare_translation(poly, transfrom[i])){
                // std::cout << "\n" << static_cast<int>(sym_mask::sym_action[i]);
                _mask |= sym_mask::sym_action[i];
            }
        }
    }

    symmetry_Poly::poly_t::set symmetry_Poly::generate_unique_child(const poly_t& poly){
        set_symmetry(poly);

        poly_t::set result = poly._empty_cells;
        mask_t temp = _mask;

        switch(temp){
            case group_mask::trivial_g8:
                break;

            case group_mask::c2_1_g4:{
                point_t shift = find_transformed_min(poly._non_empty_cells)[3];
                for(auto it = result.begin(); it != result.end();){
                    symmetry_Poly::point_t transfromed(it->get_reflect_line_0() - shift);
                    
                    if(*it != transfromed){
                            result.erase(transfromed);
                    }
                    it++;
                }
                break;
            }
                

            case group_mask::c2_2_g4:{
                point_t shift = find_transformed_min(poly._non_empty_cells)[4];
                for(auto it = result.begin(); it != result.end();){
                    symmetry_Poly::point_t transfromed(it->get_reflect_line_45() - shift);
                    
                    if(*it != transfromed){
                            result.erase(transfromed);
                    }
                    it++;
                }
                break;
            }
                

            case group_mask::c2_3_g4:{
                point_t shift = find_transformed_min(poly._non_empty_cells)[5];

                for(auto it = result.begin(); it != result.end();){
                    symmetry_Poly::point_t transfromed(it->get_reflect_line_90() - shift);
                    
                    if(*it != transfromed){
                            result.erase(transfromed);
                    }
                    it++;
                }
                break;
            }
                
            case group_mask::c2_4_g4:{
                point_t shift = find_transformed_min(poly._non_empty_cells)[6];
                for(auto it = result.begin(); it != result.end();){
                    symmetry_Poly::point_t transfromed(it->get_reflect_line_135() - shift);
                    
                    if(*it != transfromed){
                            result.erase(transfromed);
                    }
                    it++;
                }
                break;
            }
                
            case group_mask::f2_g4:{
                point_t shift = find_transformed_min(poly._non_empty_cells)[1];
                for(auto it = result.begin(); it != result.end();){
                    symmetry_Poly::point_t transfromed(it->get_rotate_clockwise_180() - shift);
                    
                    if(*it != transfromed){
                            result.erase(transfromed);
                    }
                    it++;
                }
                break;
            }
                
            
            case group_mask::d2_1_g2:{
                auto shift_array = find_transformed_min(poly._non_empty_cells);
                for(auto it = result.begin(); it != result.end(); ){
                    point_t trans_array[] = {
                        it->get_rotate_clockwise_180() - shift_array[1],
                        it->get_reflect_line_0() - shift_array[3],
                        it->get_reflect_line_90() - shift_array[5]
                    };

                    for(auto p : trans_array){                        
                        if(*it != p){
                            result.erase(p);
                        }
                    }

                    it++;
                }
                break;
            }
                
            case group_mask::d2_2_g2:{
                auto shift_array = find_transformed_min(poly._non_empty_cells);
                for(auto it = result.begin(); it != result.end(); ){
                    point_t trans_array[] = {
                        it->get_rotate_clockwise_180() - shift_array[1],
                        it->get_reflect_line_45() - shift_array[4],
                        it->get_reflect_line_135() - shift_array[6]
                    };

                    for(auto p : trans_array){                        
                        if(*it != p){
                            result.erase(p);
                        }
                    }
                    
                    it++;
                }
                break;
            }
                
            case group_mask::c4_g2:{
                auto shift_array = find_transformed_min(poly._non_empty_cells);
                for(auto it = result.begin(); it != result.end(); ){
                    point_t trans_array[] = {
                        it->get_rotate_clockwise_90() - shift_array[0],
                        it->get_rotate_clockwise_180() - shift_array[1],
                        it->get_rotate_clockwise_270() - shift_array[2]
                    };

                    for(auto p : trans_array){                        
                        if(*it != p){
                            result.erase(p);
                        }
                    }
                    
                    it++;
                }
                break;
            }
            
            case group_mask::d4_g1:{
                auto shift_array = find_transformed_min(poly._non_empty_cells);
                for(auto it = result.begin(); it != result.end(); ){
                    point_t trans_array[] = {
                        it->get_rotate_clockwise_90() - shift_array[0],
                        it->get_rotate_clockwise_180() - shift_array[1],
                        it->get_rotate_clockwise_270() - shift_array[2],
                        it->get_reflect_line_0() - shift_array[3],
                        it->get_reflect_line_45() - shift_array[4],
                        it->get_reflect_line_90() - shift_array[5],
                        it->get_reflect_line_135() - shift_array[6]
                    };

                    for(auto p : trans_array){                        
                        if(*it != p){
                            result.erase(p);
                        }
                    }
                    
                    it++;
                }
                break;
            }

            default:
                break;
        }

        return result;
    }

    symmetry_Poly::poly_t::set symmetry_Poly::generate_unique_child2(const poly_t& poly){
        constexpr bool UNIQUE_CELL = true;
        constexpr bool DUPLICATE_CELL = false;
        using hash_table = boost::unordered::unordered_flat_map<point_t::hash_t, bool>;

        set_symmetry(poly);

        poly_t::set result;
        result.reserve(poly._empty_cells.size());

        switch (_mask) {
            case group_mask::trivial_g8:
                // keep all
                break;

            case group_mask::c2_1_g4: {
                point_t shift = find_transformed_min(poly._non_empty_cells)[3];
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t t = p.get_reflect_line_0() - shift;

                    auto search = table.find(t.point_to_hash());
                    if(search != table.end()){
                        if(p != t){
                            search->second = DUPLICATE_CELL;
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::c2_2_g4: {
                point_t shift = find_transformed_min(poly._non_empty_cells)[4];
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t t = p.get_reflect_line_45() - shift;

                    auto search = table.find(t.point_to_hash());
                    if(search != table.end()){
                        if(p != t){
                            search->second = DUPLICATE_CELL;
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::c2_3_g4: {
                point_t shift = find_transformed_min(poly._non_empty_cells)[5];
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t t = p.get_reflect_line_90() - shift;

                    auto search = table.find(t.point_to_hash());
                    if(search != table.end()){
                        if(p != t){
                            search->second = DUPLICATE_CELL;
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::c2_4_g4: {
                point_t shift = find_transformed_min(poly._non_empty_cells)[6];
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t t = p.get_reflect_line_135() - shift;

                    auto search = table.find(t.point_to_hash());
                    if(search != table.end()){
                        if(p != t){
                            search->second = DUPLICATE_CELL;
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::f2_g4: {
                point_t shift = find_transformed_min(poly._non_empty_cells)[1];
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t t = p.get_rotate_clockwise_180() - shift;

                    auto search = table.find(t.point_to_hash());
                    if(search != table.end()){
                        if(p != t){
                            search->second = DUPLICATE_CELL;
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::d2_1_g2: {
                const auto shift_array = find_transformed_min(poly._non_empty_cells);
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t trans_array[] = {
                        p.get_rotate_clockwise_180() - shift_array[1],
                        p.get_reflect_line_0()       - shift_array[3],
                        p.get_reflect_line_90()      - shift_array[5]
                    };

                    for(point_t t : trans_array){
                        auto search = table.find(t.point_to_hash());
                        if(search != table.end()){
                            if(p != t){
                                search->second = DUPLICATE_CELL;
                            }
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::d2_2_g2: {
                const auto shift_array = find_transformed_min(poly._non_empty_cells);
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t trans_array[] = {
                        p.get_rotate_clockwise_180() - shift_array[1],
                        p.get_reflect_line_45()       - shift_array[4],
                        p.get_reflect_line_135()      - shift_array[6]
                    };

                    for(point_t t : trans_array){
                        auto search = table.find(t.point_to_hash());
                        if(search != table.end()){
                            if(p != t){
                                search->second = DUPLICATE_CELL;
                            }
                        }
                    }
                }
                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::c4_g2: {
                const auto shift_array = find_transformed_min(poly._non_empty_cells);
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t trans_array[] = {
                        p.get_rotate_clockwise_90()     - shift_array[0],
                        p.get_rotate_clockwise_180()    - shift_array[1],
                        p.get_rotate_clockwise_270()    - shift_array[2]
                    };

                    for(point_t t : trans_array){
                        auto search = table.find(t.point_to_hash());
                        if(search != table.end()){
                            if(p != t){
                                search->second = DUPLICATE_CELL;
                            }
                        }
                    }
                }

                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            case group_mask::d4_g1: {
                const auto shift_array = find_transformed_min(poly._non_empty_cells);
                hash_table table;
                table.reserve(poly._empty_cells.size() * 2);
                for(const point_t p : poly._empty_cells){
                    table.insert({p.point_to_hash(), UNIQUE_CELL});
                }

                for (auto it = table.begin(); it != table.end(); ++it) {
                    if(it->second == DUPLICATE_CELL){
                        continue;
                    }
                    point_t p(it->first);
                    point_t trans_array[] = {
                        p.get_rotate_clockwise_90()  - shift_array[0],
                        p.get_rotate_clockwise_180() - shift_array[1],
                        p.get_rotate_clockwise_270() - shift_array[2],
                        p.get_reflect_line_0()       - shift_array[3],
                        p.get_reflect_line_45()      - shift_array[4],
                        p.get_reflect_line_90()      - shift_array[5],
                        p.get_reflect_line_135()     - shift_array[6]
                    };

                    for(point_t t : trans_array){
                        auto search = table.find(t.point_to_hash());
                        if(search != table.end()){
                            if(p != t){
                                search->second = DUPLICATE_CELL;
                            }
                        }
                    }
                }

                for (auto& [k, v] : table) {
                    if(v == UNIQUE_CELL){
                    result.emplace(point_t(k)); 
                    }   
                }
                break;
            }

            default:
                break;
        }
        return result;
    }


    //struct symmetry_Poly

    std::array<memo_Polyomino::point_t, 7> find_transformed_min(const memo_Polyomino::set& flat_set){
        memo_Polyomino::point_t max(INT8_MAX, INT8_MAX);
        std::array<memo_Polyomino::point_t, 7> result = {
            max, max, max, max, max, max, max
        };

        for(const memo_Polyomino::point_t p : flat_set){
            std::array<memo_Polyomino::point_t, 7> cmp = {
                p.get_rotate_clockwise_90(),
                p.get_rotate_clockwise_180(),
                p.get_rotate_clockwise_270(),
                p.get_reflect_line_0(),
                p.get_reflect_line_45(),
                p.get_reflect_line_90(),
                p.get_reflect_line_135()
            };

            for(auto i = 0; i < 7; ++i){
                if(cmp[i] < result[i]){
                    result[i] = cmp[i];
                }
            }
        }

        return result;
    }

    void sort_and_insert_set(memo_Polyomino::set& flat_set, std::vector<memo_Polyomino::point_t>& vector){
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
        if(first._non_empty_cells.size() != second._non_empty_cells.size()){
            return false;
        }

        std::size_t n = first._non_empty_cells.size() * sizeof(memo_Polyomino::point_t);
        return std::memcmp(
            &(*first._non_empty_cells.cbegin()),
            &(*second._non_empty_cells.cbegin()),
            n) == 0;
    }

    std::array<memo_Polyomino::hash_t, 8> hash_free_poly(memo_Polyomino& poly){
        std::array<memo_Polyomino, 8> trans_array = {
            poly,
            poly.get_rotate_clockwise_90(),
            poly.get_rotate_clockwise_180(),
            poly.get_rotate_clockwise_270(),
            poly.get_reflect_line_0(),
            poly.get_reflect_line_45(),
            poly.get_reflect_line_90(),
            poly.get_reflect_line_135(),
        };

        std::array<memo_Polyomino::hash_t, 8> result;

        for(int i = 0 ; i < 8; ++i){
            trans_array[i].shift_to_origin();
            result[i] = trans_array[i].poly_to_hash();
        }

        return result;
    }

    std::uint64_t distance_of_point_set(const memo_Polyomino::set& point_set){
        std::uint64_t result = 0;
        for(const memo_Polyomino::point_t p : point_set){
            auto diff = p._x - p._y;
            result += std::abs(diff * diff);
        }
        return result;
    }
    std::pair<std::uint64_t, std::uint64_t> norms_of_point_set(const memo_Polyomino::set& point_set){
        std::uint64_t x_result = 0;
        std::uint64_t y_result = 0;
        for(const memo_Polyomino::point_t p : point_set){
            x_result += std::abs(p._x * p._x);
            y_result += std::abs(p._y * p._y);
        }
        return {x_result, y_result};
    }


    Polyomino_class::Polyomino_class(std::size_t num_cell){
        hash_container.resize(num_cell);
        poly_container.resize(num_cell);

        poly_container[0].emplace_back(poly_t::base_case());
        hash_container[0].insert(
            {poly_container[0].back().poly_to_hash(), 0}
        );

        for(std::size_t n_cell = 1; n_cell < num_cell; ++n_cell){
            auto& prev_poly_v = poly_container[n_cell -1];
            auto& curr_poly_v = poly_container[n_cell];
            auto& curr_hash_table = hash_container[n_cell];

            // std::cout << "\n n_cell = " << n_cell << "\t prev v size = " << prev_poly_v.size();

            for(std::size_t prev_poly_index = 0; prev_poly_index < prev_poly_v.size(); ++prev_poly_index){
                std::vector<poly_t> childs = prev_poly_v[prev_poly_index].generate_unique_child();
                prev_poly_v[prev_poly_index]._num_child = childs.size();

                // prev_poly_v[prev_poly_index].print("prev poly");
                //std::cout << "\nprev poly index = " << prev_poly_index;
                for(poly_t& child : childs){
                    bool is_new[8] = {0};
                    std::array<hash_t, 8> hash_array = hash_free_poly(child);
                    
                    for(int hash_index = 0; hash_index < 8; ++hash_index){
                        is_new[hash_index] = !curr_hash_table.contains(hash_array[hash_index]);
                        
                        if(!is_new[hash_index]){
                            break;
                        }
                        if (hash_index == 7) {
                            child._parent_id = prev_poly_index;
                            curr_poly_v.push_back(std::move(child));
                            std::size_t child_index = curr_poly_v.size() - 1;

                            // curr_poly_v.back().print("\tNew poly");

                            for(int hash_index = 0; hash_index < 8; ++hash_index){
                                if(is_new[hash_index]){
                                    curr_hash_table.insert({hash_array[hash_index], child_index});    
                                    // std :: cout << "\nhash = " << hash_array[hash_index] << "\tindex" << child_index;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void Polyomino_class::print_poly() const{
        std::size_t n = 1;
        for(auto& sub_v : poly_container){
            std::cout << "\nNum cell = " << n << "\t# Poly = " << sub_v.size();
            n++;

            std::size_t idx = 1;
            for(auto p : sub_v){
                std::cout << "\n\tid = " << idx;
                p.print();
                std::cout	<< "\n\t\tdis non_empty = " << distance_of_point_set(p._non_empty_cells)
                            << "\t dis empty = " << distance_of_point_set(p._empty_cells);
                idx++;
            }
        }
    }
}