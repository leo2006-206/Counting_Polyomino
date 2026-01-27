#include "PolyominoAnalyse.hpp"

namespace int_pair{
    std::vector<std::string> split(std::string& line, const char delimiter){
        std::vector<std::string> result;
        size_t position;
        std::string token;
        while((position = line.find(delimiter)) != std::string::npos){
            token = line.substr(0, position);
            result.emplace_back(token);
            line.erase(0, position + 1);
        }
        result.emplace_back(line);
        return result;
    }

    std::vector<uint32_t> str_to_unsigned(const std::vector<std::string>& tokens, size_t size){
        std::vector<uint32_t> result;
        result.reserve(size);

        for(size_t i = 0; i < tokens.size(); ++i){
            result.push_back(static_cast<uint32_t>(std::stoul(tokens[i])));
        }

        return result;
    }

    std::vector<Point> extract_point_from_string(std::string& line, unsigned size){
        std::vector<Point> result;
        std::string sub;
        size_t pos_end = 0 , pos_delimier;

        result.reserve(size);

        while(line.size()){
            pos_end = line.find(") ");
            sub = line.substr(0, pos_end);
            pos_delimier = sub.find(". ");
            line.erase(0, pos_end+2);
            // std::cout<<sub<<"\t\t"<<sub.substr(1, pos_delimier-1)<<"  -  "<<sub.substr(pos_delimier+1)<<"\t\t"<<pos_end<<"\t\t"<<pos_delimier<<"\t\t"<<line<<"\n";
            result.emplace_back(Point(
                                std::stoi(sub.substr(1, pos_delimier-1)),
                                std::stoi(sub.substr(pos_delimier+1))
                            ));
        }
        return result;
    }

    void analyse_Polyomino(const std::vector<Point>& points, std::array<unsigned, 6>& result){
        std::unordered_map<uint64_t,std::pair<bool, uint8_t>> table;
        Point adjacent_point[4];

        table.reserve(points.size()<<2);
        //size -> N, 4N > (N + 2N + 2  this is the upper bound of given Polyomino N cell + max # child), good size

        for(auto i = points.cbegin(); i != points.cend(); ++i){
            table.insert({i->pair_to_int(), std::make_pair<bool, uint8_t>(true, 0)});
        }
        
        for(auto i = points.cbegin(); i != points.cend(); ++i){
            adjacent_point[0] = i->move_up();
            adjacent_point[1] = i->move_right();
            adjacent_point[2] = i->move_down();
            adjacent_point[3] = i->move_left();

            for(auto j = 0; j < 4; ++j){
                auto search = table.find(adjacent_point[j].pair_to_int());
                if(search != table.end()){
                    search->second.second +=1;
                }
                else{
                    table.insert({adjacent_point[j].pair_to_int(), std::make_pair<bool, uint8_t>(false, 1)});
                }
            }
        }
        //std::cout<<"\nstart\n";
        for(auto i = table.cbegin(); i != table.cend(); ++i){
            // Point a = Point::int_to_pair(i->first);
            // std::cout<<a._x<<"\t"<<a._y<<"\tbool : "<<i->second.first<<"\tnumber : "<<static_cast<int>(i->second.second)<<"\n";
            if(i->second.first == true){
                result[2] += i->second.second;
            }
            else{
                if(i->second.second == 2){
                    result[3] += 1;
                }
                else if(i->second.second == 3){
                    result[4] += 1;
                }
                else if(i->second.second == 4){
                    result[5] += 1;
                }
            }
        }
        result[1] = points.size()<<2;
        result[0] = result[1] - result[2] - result[3] - (result[4]*2) - (result[5]*3);
    }

    void record_distribution(std::unordered_map<uint32_t, uint32_t>& table, uint32_t data){
        auto search = table.find(data);
        if(search != table.end()){
            search->second +=1;
        }
        else{
            table.insert({data, 1});
        }
    }

    void record_distribution(std::map<uint32_t, uint32_t>& tree, uint32_t data){
        auto search = tree.find(data);
        if(search != tree.end()){
            search->second +=1;
        }
        else{
            tree.insert({data, 1});
        }
    }

    void record_distribution(std::map<uint64_t, uint32_t>& tree, uint64_t data){
        auto search = tree.find(data);
        if(search != tree.end()){
            search->second +=1;
        }
        else{
            tree.insert({data, 1});
        }
    }

    Point compute_distance_from_origin(const std::vector<Point>& points){
        Point result;
        for(auto i = points.cbegin(); i != points.cend(); ++i){
            result._x += (i->_x * i->_x);
            result._y += (i->_y * i->_y);
        }
        return result;
    }
}