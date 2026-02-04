#include "../Header/Poly_Int_Pair_v1.hpp"
#include "../Header/Poly_Int_Pair_v2.hpp"
#include "../Header/Poly_Int_Pair_v3.hpp"
#include "../Header/Timing.hpp"


#define DATA_FILE "../CSV_Data/Data.csv"
void test1(){
    using namespace int_pair_v3;

    // static constexpr std::array<std::pair<int,int>, 7> non_empty = {{
    //     // {1, 0},
    //     {0, 0},
    //     {0, -1},
    //     {0, -2},
    //     {0, -3},
    //     {0, -4},
    //     {0, -5},
    //     {0, -6}
    // }};

    std::array<std::pair<int, int>, 2> non_empty = {{
        {0, 0},
        {0, 1}
    }};

    memo_Polyomino base1;

    for(auto [x, y] : non_empty){
        base1._cells.emplace(x, y);
    }

    base1.generate_adj_empty_cells();
    std::cout << "\nshift = " << base1.shift_to_origin().str();

    free_Polyomino f(base1);
    std :: cout << "\n" << (int)symmetry_mask::set_sym(f);

    std::cout << "\n\n\n";

    f._sym_mask = symmetry_mask::set_sym(f);

    f.print();

    for(int i = 0; i < 100; ++i){
        auto array = symmetry_mask::get_unique_child(f);

        // for(auto& p : array){
        //     std :: cout << "\n" << p.str();
        // }

        // std::cout << "\nwant to map point "<<(*array.begin()).str();

        f.grow_and_shift(*array.begin());

        // std::cout << "\n\n\n";

        // f.print();
        // for(auto h : f.fixed_class_hash()){
        //     std::cout << "\n" << h;
        // }
    }

    f.print();

}

void test2(){
    using namespace int_pair_v3;

    memo_Polyomino base;

    auto last = base._cells.capacity();

    constexpr std::size_t size = 1 << 8;
    for(std::size_t i = 0; i < size; ++i){
        for(std::size_t j = 0; j < size; ++j){
            // memo_Polyomino::grow_flat_set(base._cells);
            base._cells.emplace(i, j);
            if(last != base._cells.capacity()){
                auto temp = last;
                last = base._cells.capacity();
                std::cout << "\n\ncapacity = " << last << "\tprevious = " << temp;
                std::cout << "\nrate = " << (double)temp / (double)last << "\ti = " << i * size + j;
            }
        }
    }
}

void test3(){
    using namespace int_pair_v3;

    std :: cout << "\n" << sizeof(free_Polyomino);
}

void generate(const int num_cells){
    using namespace int_pair_v2;

    Polyomino_class all(num_cells);
    //generate all free polyominoes from 1 to num_cells

    all.print_num();
    //print the number of free polyominoes for each number of cell
    //and print the number of hash for each number of cell

    //all.print();
    //print all the free polyominoes for each cumber of cell
    //and print all the hash value for each cumber of cell
}

int main(void){

    constexpr int num_cells = 13;

    timing::measure(generate, num_cells);
    //also print the time used to generate free poly
}