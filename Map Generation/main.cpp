#include <iostream>
#include <array>
#include <vector>
#include <time.h>

class Generation{
public:
    static constexpr int WIDTH = 32, HEIGHT = 32;
    std::array<std::array<char,WIDTH>,HEIGHT> map;
    std::vector<std::pair<int, int>> p_ConnectPoints; //possible connect points
    std::vector<std::pair<int, int>> AllPoints;
    std::vector<std::vector<std::pair<int,int>>> doorWalls;
    time_t start; //maybe use Chrono instead??
    
    void ProduceMap(){
        while(1){
            std::vector<char> room;
            AllPoints.clear();
            doorWalls.clear();
            start = time(NULL);
            for(int j = 0; j < HEIGHT; j++)
                for(int i = 0; i < WIDTH; i++)
                    map[j][i] = '.'; //E
            
            if(GetRoom(room, 5, 7) == -1) continue; //room where the player spawns
                   
            for(int i = 0; i < 7; i++)
                if(GetRoom(room, 5, 8) == -1) continue;
            
            if(GetRoom(room, 8, 10) == -1) continue; //boss room
            
            break;
        }
        
        for(int j = 0; j < doorWalls.size();j++)
            for(int i = 0; i < doorWalls[j].size();i++){
                auto pos = doorWalls[j][rand() % (doorWalls[j].size())];
                map[pos.first][pos.second] = ' '; //%
            }
    }
    
    int GetRoom(std::vector<char>& room,const int& minS, const int& maxS){
        static bool playerSpawned = false;
        int width = 0;
        int height = 0;
        int y = 0;
        int x = 0;
        int pos = 0;
        if(!room.empty())
            room.clear();
        
        if(!p_ConnectPoints.empty()){
            do{
                if(time(NULL) - start >= 1) return -1;
                width = minS + (rand() % (maxS - minS));
                height = minS + (rand() % (maxS - minS));
                pos = rand() % p_ConnectPoints.size();
                x = p_ConnectPoints[pos].first - (rand() % 2 == 0 ? height-1 : 0);
                y = p_ConnectPoints[pos].second - (rand() % 2 == 0 ? width-1 : 0);
               
                
            } while(!CanPlaceRoom(x,y,width,height));
            p_ConnectPoints.clear();
            doorWalls.push_back(std::vector<std::pair<int,int>>());
        }
        else {
            width = minS + (rand() % (maxS - minS));
            height = minS + (rand() % (maxS - minS));
            y = rand() % (WIDTH-width);
            x = rand() % (HEIGHT-height);
        }
        room.reserve(width * height);

        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++){
                if(j == x || i == y || i == width+y-1 || j == height+x-1) {
                    room.push_back('#'); //#
                    if((j == x && i == y) || (j == x+height-1 && i == y+width-1)
                       || (j == x && i == y+width-1) || (j == x+height-1 && i == y)){
                        continue;
                    }
                       
                       if((j == x && i == y+1) || (j == x+height-1 && i == y+width-2)
                       || (j == x+1 && i == y+width-1) || (j == x+height-2 && i == y)
                       
                       || (j == x+1 && i == y) || (j == x+height-2 && i == y+width-1)
                          || (j == x && i == y+width-2) || (j == x+height-1 && i == y+1)){
                          
                           
                           if(FoundDoorWall(std::make_pair(j,i)))
                               doorWalls[doorWalls.size()-1].push_back(std::make_pair(j,i));
                           AllPoints.push_back(std::make_pair(j,i));
                               
                           continue;
                       }
                    
                    p_ConnectPoints.push_back(std::make_pair(j,i));
                    AllPoints.push_back(std::make_pair(j,i));
                    continue;
                }
                if(!playerSpawned) { playerSpawned = true; room.push_back('P');}
                else room.push_back(' ');  //-
            }
       
        int r = 0;
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++,r++)
                map[j][i] = room[r];
        return 0;
    }
    
    bool CanPlaceRoom(int x, int y, int width, int height){
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++)
                if(map[j][i] == 'G') return false; //-
        return x > 0 && y > 0 && x+height < HEIGHT && y+width < WIDTH;
    }
    
    bool FoundDoorWall(std::pair<int,int> p_door){
        for(int j = 0; j < AllPoints.size(); j++)
            if(AllPoints[j] == p_door)
                return true;
        return false;
    }
};

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    Generation gen;
    gen.ProduceMap();
    
    for(int i = 0; i < gen.HEIGHT; i++){
        for(int j = 0; j < gen.WIDTH; j++)
            std::cout << gen.map[i][j];
        std::cout << std::endl;
    }
    return 0;
}
