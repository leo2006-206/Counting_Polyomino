#include "PolyominoAnalyse.hpp"
#include <fstream>
#include <string>
#include <cassert>

#define DATA_FILE "../CSV_Data/Data.csv"
#define ANALYSIS_FILE "../CSV_Data/Analysis.csv"

#define DISTANCE_FILE "../CSV_Data/Distance.csv"
#define DIS_CHILD_FILE "../CSV_Data/Distribution_Child.csv"
#define DIS_BRIDGE_FILE "../CSV_Data/Distribution_Bridge.csv"
#define DIS_SF2_FILE "../CSV_Data/Distribution_SF2.csv"
#define DIS_SF3_FILE "../CSV_Data/Distribution_SF3.csv"
#define DIS_SF4_FILE "../CSV_Data/Distribution_SF4.csv"


#define MAX_N 9

void read_testing(){
    using namespace int_pair;
    std::fstream reader(DATA_FILE, std::fstream::in);
    std::string line;
    std::vector<std::string> result;

    assert(reader.is_open() && "\nReader file access failed\n");


    std::getline(reader, line);

    while(std::getline(reader, line)){
        result = split(line, ',');
        // std::cout<<*result.rbegin()<<"\n";
    }

    auto v = extract_point_from_string(*result.rbegin(), 10);
    for(Point p : v){
        std::cout<<p._x<<" "<<p._y<<"\n";
    }
    reader.close();
}

void read_write_testing(){
    using namespace int_pair;
    
    std::fstream writer(ANALYSIS_FILE, std::fstream::out);
    std::fstream reader(DATA_FILE, std::fstream::in);
    std::string line;
    std::vector<std::string> tokens;
    std::array<unsigned, 6> analysis;
    //[0] -> # Child from Analysis -> 4N - 2b - (1 * sf_3 + 2 * sf_3 + 3 * sf_4)
    //[1] -> 4N, [2] -> bridge, [3] -> sf_2, [4] -> sf_3, [5] -> sf_4

    assert(writer.is_open() && "\nWriter file access failed\n");
    assert(reader.is_open() && "\nReader file access failed\n");


    analysis_first_line(writer);
    std::getline(reader, line);

    while(std::getline(reader, line)){
        tokens = split(line, ',');
        analysis.fill(0);
        analyse_Polyomino(
            extract_point_from_string(
                *tokens.rbegin(),
                std::stoi(tokens[0])
            ),analysis);
        writer<<tokens[0]<<","<<
                tokens[1]<<","<<
                tokens[2]<<","<<
                tokens[3]<<","<<
                tokens[4]<<","<<
                analysis[0]<<","<<
                analysis[1]<<","<<
                analysis[2]<<","<<
                analysis[3]<<","<<
                analysis[4]<<","<<
                analysis[5]<<"\n";
    }

    writer.close();
    reader.close();
}

void distribution_testing(){
    using namespace int_pair;
    
    std::fstream writer(DIS_SF4_FILE, std::fstream::out);
    std::fstream reader(ANALYSIS_FILE, std::fstream::in);
    std::string line;
    std::vector<std::string> tokens;

    assert(writer.is_open() && "\nWriter file access failed\n");
    assert(reader.is_open() && "\nReader file access failed\n");

    std::vector<uint32_t> data;
    std::array<std::map<uint32_t, uint32_t>, MAX_N> num_range;
    // for(size_t i = 0; i < MAX_N; ++i){num_range[i].reserve(MAX_N<<1);}

    const char* name = "Shared_Face_4";
    distribution_first_line(writer, name);
    std::getline(reader, line);

    while(std::getline(reader, line)){
        tokens = split(line, ',');
        data = str_to_unsigned(tokens,11);

        // std::cout<<data[0]<<"\t"<<
        //             data[5]<<"\t"<<
        //             data[7]<<"\t"<<
        //             data[8]<<"\t"<<
        //             data[9]<<"\t"<<
        //             data[10]<<"\n";

        record_distribution(num_range[data[0]-1], data[10]);
    }

    for(auto i = 0; i < MAX_N; ++i){
        std::cout<<"# Cells -> N = "<<i+1<<"\n";
        for(auto j = num_range[i].cbegin(); j != num_range[i].cend(); ++j){
            std::cout<<"\t"<<name<<" = "<<j->first<<"\t# occur = "<<j->second<<"\n";
            writer<<i+1<<","<<
                    j->first<<","<<
                    j->second<<"\n";
        }      
    }
    writer.close();
    reader.close();
}

void analyse_Polyomino_testing(){
    using namespace int_pair;
    std::string s = {"(3. -3) (4. -3) (2. -2) (3. -2) (1. -1) (2. -1) (0. 0) (1. 0) (0. 1) "};
    std::vector<Point> v = extract_point_from_string(s, MAX_N);
    for(auto p : v){
        std::cout<<p._x<<"\t"<<p._y<<"\n";
    }

    std::array<unsigned, 6> array;
    array.fill(0);
    analyse_Polyomino(v, array);
    for(auto i : array){
        std::cout<<i<<"\t";
    }

}

void point_pairing_testing(){
    using namespace int_pair;
    Point p(4, 8);
    uint64_t h = p.pair_to_int();
    std::cout<<h<<"\n";
    std::cout<<Point::int_to_pair(h)._x<<"\n";
    std::cout<<Point::int_to_pair(h)._y<<"\n";
}

void points_to_vector(){
    using namespace int_pair;
    
    std::fstream writer(DISTANCE_FILE, std::fstream::out);
    std::fstream reader(DATA_FILE, std::fstream::in);
    std::string line;
    std::vector<std::string> tokens;
    std::vector<int> distance;

    assert(writer.is_open() && "\nWriter file access failed\n");
    assert(reader.is_open() && "\nReader file access failed\n");


    
    std::getline(reader, line);
    writer<<"# Cells -> N"<<","<<
            "Point Set"<<","<<
            "x * x + y * y"<<","<<
            "Distance"<<"\n";
    while(std::getline(reader, line)){
        tokens = split(line, ',');
        auto v = extract_point_from_string(*tokens.rbegin(), std::stoi(tokens[0]));
        
        writer<<tokens[0]<<",";
        for(Point p : v){
            writer<<"("<<p._x<<". "<<p._y<<") ";
        }
        writer<<",";
        distance.clear();
        for(Point p : v){
            int dis = p._x * p._x + p._y * p._y;
            distance.emplace_back(dis);
            writer<<dis<<" ";
        }
        writer<<",";
        for(int i : distance){
            writer<<std::sqrtl(i)<<" ";
        }
        writer<<"\n";
    }
    writer.close();
    reader.close();
}

void distance_testing(){
    using namespace int_pair;
    std::string s = {"(0. -1) (0. 0) (1. 0) (2. 0) (3. 0) (0. 1) (3. 1) "};
    std::vector<Point> v = extract_point_from_string(s, 7);
    std::vector<int> data;

    data.clear();
    for(auto p : v){
        Point m = p;
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.rotate_clockwise_90();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.rotate_clockwise_180();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.rotate_clockwise_270();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.reflect_x_axis();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.reflect_line_45();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.reflect_y_axis();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";

    data.clear();
    for(auto p : v){
        Point m = p.reflect_line_135();
        data.emplace_back(m._x * m._x + m._y * m._y);
        std::cout<<"("<<m._x<<". "<<m._y<<") ";
    }
    std::cout<<"\n";
    for(auto i : data){
        std::cout<<i<<"     ";
    }
    std::cout<<"\n\n\n";
}



int main(){
    timer my_timer;
    my_timer.start();

    distance_testing();

    my_timer.end();
}