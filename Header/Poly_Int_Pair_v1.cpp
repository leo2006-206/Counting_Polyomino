#include "Poly_Int_Pair_v1.hpp"

namespace int_pair_v1{

    bool find_cell(const uint64_t hash, const std::vector<uint64_t>& body){
        for(size_t i = 0; i < body.size(); ++i){
            if(body[i] == hash){
                return true;
            }
        }
        return false;
    }

    std::vector<int_pair_v1::vector_Polyomino> generate_vector(const vector_Polyomino& root){
        std::vector<vector_Polyomino> result;
        std::vector<uint64_t> used_cells;
        Point32 adjacent_cell[4];
        
        // std::cout<<"\n\n\n";
        result.reserve(root._cells.capacity());
        for(size_t i = 0; i < root._cells.size(); ++i){
            // std::cout<<"\n("<<root._cells[i]._x<<", "<<root._cells[i]._y<<")  "<<root._cells[i].pair_to_int();
            used_cells.push_back(root._cells[i].pair_to_int());
        }
        for(size_t i = 0; i < root._cells.size(); ++i){
            adjacent_cell[0] = root._cells[i].move_up();
            adjacent_cell[1] = root._cells[i].move_right();
            adjacent_cell[2] = root._cells[i].move_down();
            adjacent_cell[3] = root._cells[i].move_left();

            for(int j = 0; j < 4; ++j){

                if(find_cell(adjacent_cell[j].pair_to_int(), used_cells)){
                    // std::cout<<"\n found ("<<adjacent_cell[j]._x<<", "<<adjacent_cell[j]._y<<")  "<<adjacent_cell[j].pair_to_int();
                }
                else{
                    // std::cout<<"\n unused ("<<adjacent_cell[j]._x<<", "<<adjacent_cell[j]._y<<")  "<<adjacent_cell[j].pair_to_int();
                    vector_Polyomino new_vector_polyomino(root._cells.size() + 1);
                    new_vector_polyomino._cells = root._cells;
                    new_vector_polyomino._cells.push_back(adjacent_cell[j]);
                    // new_vector_polyomino.sort();
                    result.push_back(new_vector_polyomino);
                    used_cells.push_back(adjacent_cell[j].pair_to_int());
                }
            }
        }
        // std::cout<<"\n\n\n";
        return result;
    }

    std::vector<int_pair_v1::vector_Polyomino> generate_hash_table(const vector_Polyomino& root){
        std::vector<vector_Polyomino> result;
        std::unordered_set<uint64_t> used_cells;
        Point32 adjacent_cell[4];

        result.reserve(root._cells.capacity());

        for(size_t i = 0; i < root._cells.size(); ++i){
            used_cells.insert(root._cells[i].pair_to_int());
        }
        for(size_t i = 0; i < root._cells.size(); ++i){
            adjacent_cell[0] = root._cells[i].move_up();
            adjacent_cell[1] = root._cells[i].move_right();
            adjacent_cell[2] = root._cells[i].move_down();
            adjacent_cell[3] = root._cells[i].move_left();

            for(int j = 0; j < 4; ++j){

                auto search = used_cells.find(adjacent_cell[j].pair_to_int());
                if(search == used_cells.end()){
                    vector_Polyomino new_vector_polyomino(root._cells.capacity());
                    new_vector_polyomino._cells = root._cells;
                    new_vector_polyomino._cells.push_back(adjacent_cell[j]);
                    result.push_back(new_vector_polyomino);
                    used_cells.insert(adjacent_cell[j].pair_to_int());
                }
            }
        }
        return result;
    }

    std::vector<vector_Polyomino> remove_duplication(std::vector<vector_Polyomino>& body){
        for(auto i = body.begin(); i != body.end(); ++i){
            for(auto j = std::next(i); j != body.end(); ){
                if(compare_rotation(*i, *j) || compare_reflection(*i, *j)){
                    j = body.erase(j);
                }
                else{
                    ++j;
                }
            }
        }
        return body;
    }

    int compare_translation(const vector_Polyomino& first, const vector_Polyomino& second){
        vector_Polyomino temp1 = first;
        vector_Polyomino temp2 = second;
        temp1.sort();
        temp2.sort();
        Point32 mapping = temp2._cells[0] - temp1._cells[0];
        for(size_t i = 1; i < temp1._cells.size(); ++i){
            if(mapping != (temp2._cells[i] - temp1._cells[i])){
                return 0;
            }
        }
        if(temp1._cells[0] < temp2._cells[0]){
            return 1;
        }
        else{
            return 2;
        }
    }

    bool compare_rotation(const vector_Polyomino& first, const vector_Polyomino& second){
        vector_Polyomino r90  = first.rotate_clockwise_90();
        vector_Polyomino r180 = first.rotate_clockwise_180();
        vector_Polyomino r270 = first.rotate_clockwise_270();

        return (first == second || r90 == second || r180 == second || r270 == second);   
    }

    bool compare_reflection(const vector_Polyomino& first, const vector_Polyomino& second){
        vector_Polyomino r0  = first.reflect_x_axis();
        vector_Polyomino r45 = first.reflect_line_45();
        vector_Polyomino r90 = first.reflect_y_axis();
        vector_Polyomino r135= first.reflect_line_135();

        return(r0 == second || r45 == second || r90 == second || r135 == second);
    }

    vector_Group::vector_Group(size_t size){
        _array.reserve(size);
        for(size_t i = 0; i < size; ++i){
            _array.push_back(std::vector<vector_Polyomino>());
        }
        _array[0].push_back(base_case());
        for(size_t i=1; i<size; ++i){
            for(auto j = _array[i-1].cbegin(); j != _array[i-1].cend(); ++j){
                std::vector<vector_Polyomino> temp = generate_hash_table(*j);
                for(auto k = temp.cbegin(); k != temp.cend(); ++k){
                    _array[i].emplace_back(*k);
                    // k->print("\t\tinsert");
                }
            }
            _array[i] = remove_duplication(_array[i]);
        }
    }

    // BST_Polyomino version

    std::vector<BST_Polyomino> generate_hash_table(const BST_Polyomino& root, const std::uint32_t parent_ID){
        std::vector<BST_Polyomino> result;
        std::unordered_set<uint64_t> used_cells;
        Point32 adjacent_cell[4];

        for(auto i = root._cells.cbegin(); i != root._cells.cend(); ++i){
            used_cells.insert(i->pair_to_int());
        }
        for(auto i = root._cells.cbegin(); i != root._cells.cend(); ++i){
            adjacent_cell[0] = i->move_up();
            adjacent_cell[1] = i->move_right();
            adjacent_cell[2] = i->move_down();
            adjacent_cell[3] = i->move_left();

            for(int j = 0; j < 4; ++j){
                auto search = used_cells.find(adjacent_cell[j].pair_to_int());
                if(search == used_cells.end()){
                    BST_Polyomino child(parent_ID);
                    child._cells.insert(root._cells.cbegin(), root._cells.cend());
                    child._cells.insert(adjacent_cell[j]);
                    result.emplace_back(child);
                    used_cells.insert(adjacent_cell[j].pair_to_int());
                }
            }
        }
        return result;
    }

    std::vector<BST_Polyomino> remove_duplication(std::vector<BST_Polyomino>& body){
        for(auto i = body.begin(); i != body.end(); ++i){
            for(auto j = std::next(i); j != body.end(); ){
                if(compare_rotation(*i, *j) || compare_reflection(*i, *j)){
                    j = body.erase(j);
                }
                else{
                    ++j;
                }
            }
        }
        return body;
    }

    int compare_translation(const BST_Polyomino& first, const BST_Polyomino& second){
        Point32 mapping = *second._cells.cbegin() - *first._cells.cbegin();
        auto it_first = ++first._cells.cbegin();
        auto it_second = ++second._cells.cbegin();
        while(
            it_first != first._cells.cend() && 
            it_second != second._cells.cend()){
            if(mapping != (*it_second - *it_first)){
                return 0;
            }
            ++it_first;
            ++it_second;
        }
        if(*second._cells.cbegin() < *first._cells.cbegin()){
            return 1;
        }
        else{
            return 2;
        }
    }

    bool compare_rotation(const BST_Polyomino& first, const BST_Polyomino& second){
        BST_Polyomino rotation[3] = {   first.rotate_clockwise_90(),
                                        first.rotate_clockwise_180(),
                                        first.rotate_clockwise_270()};

        return (
            compare_translation(first, second) ||
            compare_translation(rotation[0], second) || 
            compare_translation(rotation[1], second) ||
            compare_translation(rotation[2], second)
        );
    }

    bool compare_reflection(const BST_Polyomino& first, const BST_Polyomino& second){
        BST_Polyomino reflection[4] = {
            first.reflect_x_axis(),
            first.reflect_line_45(),
            first.reflect_y_axis(),
            first.reflect_line_135()
        };
        return (
            compare_translation(reflection[0], second) ||
            compare_translation(reflection[1], second) ||
            compare_translation(reflection[2], second) ||
            compare_translation(reflection[3], second)
        );
    }

    BST_Group::BST_Group(size_t num_cell){

        _array.reserve(num_cell);
        for(size_t i = 0; i < num_cell; ++i){
            _array.push_back(std::vector<BST_Polyomino>());
        }
        _array[0].push_back(BST_Polyomino::base_case());
        for(size_t i = 1; i < num_cell; ++i){
            // for(auto j = _array[i-1].cbegin(); j != _array[i-1].cend(); ++j){
            //     std::vector<BST_Polyomino> temp = generate_hash_table(*j, static_cast<uint32_t>());
            //     std::cout<<"\n"<<temp.size();
            //     for(auto k = temp.cbegin(); k != temp.cend(); ++k){
            //         _array[i].emplace_back(*k);
            //         // k->print("\t\tinsert");
            //     }
            // }
            for(size_t j = 0; j < _array[i-1].size(); ++j){
                std::vector<BST_Polyomino> temp = generate_hash_table(_array[i-1][j], static_cast<uint32_t>(j));
                _array[i-1][j]._child = temp.size();
                // remove_duplication(temp);
                for(auto k = temp.cbegin(); k != temp.cend(); ++k){
                    _array[i].emplace_back(*k);
                    // k->print("\t\tinsert");
                }
            }
            remove_duplication(_array[i]);
        }
        for(auto i = _array[num_cell-1].begin(); i != _array[num_cell-1].end(); ++i){
            std::vector<BST_Polyomino> temp = generate_hash_table(*i, static_cast<uint32_t>(MY_UINT32_MAX));
            i->_child = temp.size();
        }
    }

}