#include <iostream>
#include <array>
#include <vector>
#include <time.h>

class Generation{
public:
    using POS = std::pair<int,int>;
    static constexpr int WIDTH = 32, HEIGHT = 32;
    std::array<std::array<char,WIDTH>,HEIGHT> map;
    std::vector<std::vector<POS>> rooms;
    std::vector<POS> p_ConnectPoints; //possible connect points
    std::vector<std::vector<POS>> doorWalls;
    std::vector<std::vector<POS>> allWalls;
   // std::vector<std::vector<POS>> allRooms;
    std::time_t start;
    void ProduceMap(){
        bool success = false;
        while(!success){
            success = true;
            std::vector<char> room;
            rooms.clear();
            allWalls.clear();
            doorWalls.clear();
            p_ConnectPoints.clear();
            start = std::time(NULL);
            for(int j = 0; j < HEIGHT; j++)
                for(int i = 0; i < WIDTH; i++)
                    map[j][i] = '.'; //E
                   
            for(int i = 0; i < 10; i++)
                if(GetRoom(room, 8, 10) == -1) success = false;
            
            if(GetRoom(room, 10, 10) == -1) success = false; //boss room
            
        }
      
        
        SpawnDoors();
        std::pair<int,int> pos;
        do{
            int x = rand() % (rooms.size()-1);
            int y = rand() % rooms[x].size();
            pos = rooms[x][y];
        }while(map[pos.first][pos.second] == '#');
        map[pos.first][pos.second]  = 'P';
    }
    
    
    void SpawnDoors(){
        std::vector<std::vector<POS>> walls;
        
       /* for(int j = 0; j < allWalls.size(); j++){
            int i = 0;
            while(1){
                if(allWalls[j].size()-1 == i) break;
                auto pos = allWalls[j][i];
                if(map[pos.first][pos.second] == '-'){
                   // if(allWalls.size()-1 == i)
                     //   allWalls[j].pop_back();
                    allWalls[j].erase(allWalls[j].begin() + i);
                   
                    continue;
                }
                i++;
               
            }
        }*/
        
        for(int j = 0; j < allWalls.size(); j++){
            int i = 0;//rand() % allWalls[j].size();
            while(1){
                if(allWalls[j].size()-1 == i) break;
                auto pos = allWalls[j][i];
                if(map[pos.first][pos.second] == '-'){
                   // if(allWalls.size()-1 == i)
                     //   allWalls[j].pop_back();
                    allWalls[j].erase(allWalls[j].begin() + i);
                   
                    continue;
                }
                i++;
               
            }
        }
        
       // if(pos.second >= 0 && pos.second < WIDTH && map[pos.first][pos.second + 1] == '-' &&
         //      map[pos.first][pos.second - 1] == '-')
        
        
        auto WallExist = [&](POS pos){
            for(int k = 0; k < walls.size(); k++)
                for(int l = 0; l < walls[k].size();l++){
                    if(walls[k][l] == pos) return true;
                }
            return false;
        };
       // bool goFurther = false;
        walls.push_back(std::vector<POS>());
        for(int j = 0; j < allWalls.size(); j++){
            for(int i = 0; i < allWalls[j].size();i++){
                auto pos = allWalls[j][i];
                if(map[pos.first][pos.second] == '-')
                    continue;
                
               
                if(WallExist(pos))
                    continue;
                
                 if(pos.second >= 0 && pos.second < WIDTH && map[pos.first][pos.second + 1] == '-' &&
                        map[pos.first][pos.second - 1] == '-')
                    walls.back().push_back(pos);
                else if(pos.first >= 0 && pos.first < HEIGHT && map[pos.first + 1][pos.second] == '-' &&
                   map[pos.first - 1][pos.second] == '-')
                    walls.back().push_back(pos);
                
                else if(walls.back().size() != 0)
                    walls.push_back(std::vector<POS>());
            }
        }
        
        for(int j = 0; j < walls.size();){
              if(walls[j].size() != 0) {
                  int i = rand() % (walls[j].size());
                  auto pos = walls[j] [i];
                  if(map[pos.first][pos.second] == '-'){
                      walls[j].erase(walls[j].begin() + i);
                      continue;
                  }
                  if( (pos.second < WIDTH && map[pos.first][pos.second + 1] == '%') || (pos.second >= 0 &&                                  map[pos.first][pos.second - 1] == '%')){
                      j++;
                      continue;
                  }
                      
                  else if((pos.first < HEIGHT && map[pos.first + 1][pos.second] == '%') || (pos.first >= 0 &&
                         map[pos.first - 1][pos.second] == '%')){
                      j++;
                      continue;
                  }
                 /* if(j != walls.size()-1 && rand() % 3 == 0)
                      map[pos.first][pos.second] = '-';
                  else*/
                      map[pos.first][pos.second] = '%';
              }
            j++;
          }
        
        
        /*int j;
        while(1){
            if(walls[j].size() != 0) {
                auto pos = walls[j] [rand() % (walls[j].size())];
                if(map[pos.first][pos.second] == '-')
                    continue;
                j++;
                map[pos.first][pos.second] = '%';
            }
          
            if(j == walls[j].size())
            j++;
            
        }*/
        
       /* for(int j = 0; j < allWalls.size()-1; j++){
            doorWalls.push_back(std::vector<std::pair<int,int>>());
            for(int i = 0; i < allWalls[j].size();){
                
                auto pos = allWalls[j][i];
                if(map[pos.first][pos.second] == '-') {i++; continue;}
                if(pos.first > 0 && pos.first < HEIGHT && map[pos.first + 1][pos.second] == '-' &&
                   map[pos.first - 1][pos.second] == '-')
                    doorWalls[j].push_back(pos);
                else if(pos.second > 0 && pos.second < WIDTH && map[pos.first][pos.second + 1] == '-' &&
                    map[pos.first][pos.second - 1] == '-')
                    doorWalls[j].push_back(pos);
                i++;
            }
        }*/
        
      /*  for(int j = 0; j < doorWalls.size();){ // the thing is that there are not walls really stored...
            if(doorWalls[j].size() == 0) {
                j++;
                continue;
            }
            auto pos = doorWalls[j] [rand() % (doorWalls[j].size())];
            map[pos.first][pos.second] = '%';
            j++;
        }*/
       /* bool islocatedBoss = false;
        for(int j = 0; j < doorWalls.size();j++){
            //for(int i = 0; i < doorWalls[j].size();i++){
          //  if(doorWalls[j].size() == 0) continue;
            if(doorWalls[j].size() == 0) {continue;}
            int i = rand() % (doorWalls[j].size());
            POS pos;
           // if(i == doorWalls[j].size()-1){
                for(; i < rand() % (doorWalls[j].size());i++){
                    pos = doorWalls[j][i];
                    if(map[pos.first+1][pos.second] == '%' || map[pos.first-1][pos.second] == '%'
                       || map[pos.first][pos.second+1] == '%' || map[pos.first][pos.second-1] == '%') continue;
                    
                    for(int s = 0; s < rooms.back().size();s++) //checking if the pos is on the boss room
                        if(pos == doorWalls.back()[s]) { islocatedBoss = true; break;}
                    
                    
                    if(!islocatedBoss)
                        map[pos.first][pos.second] = '-';
                    islocatedBoss = false;
                }
            
        }*/
    }
    
    int GetRoom(std::vector<char>& room,const int& minS, const int& maxS){
        int width = 0;
        int height = 0;
        int y = 0;
        int x = 0;
        int pos = 0;
        int dirY = 0;
        int dirX = 0;
        if(!room.empty())
            room.clear();
        
        if(!p_ConnectPoints.empty()){
           
            do{
                if(time(NULL) - start >= 1) return -1;
                if(minS - maxS != 0){
                    width = minS + (rand() % (maxS - minS));
                    height = minS + (rand() % (maxS - minS));
                } else{
                    width = maxS;
                    height = maxS;
                }
                pos = rand() % p_ConnectPoints.size();
                x = p_ConnectPoints[pos].first - (rand() % 3 == 0 ? 0 : height-1);
                y = p_ConnectPoints[pos].second - (rand() % 3 == 0 ? 0 : width-1);
               
                
            } while(!CanPlaceRoom(x,y,width,height));
            p_ConnectPoints.clear();
            
        }
        else {
            width = minS + (rand() % (maxS - minS));
            height = minS + (rand() % (maxS - minS));
           
            x = rand() % (HEIGHT-height);
            y = rand() % (WIDTH-width);
        }
        allWalls.push_back(std::vector<std::pair<int,int>>());
        room.reserve(width * height);
        rooms.push_back(std::vector<std::pair<int,int>>());
       // doorWalls.push_back(std::vector<POS>());
       // allRooms.push_back(std::vector<POS>());
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++){
                if(j == x || i == y || i == width+y-1 || j == height+x-1) {
                    room.push_back('#'); //W
                    //if(FoundDoorWall(std::make_pair(j,i)))
                      //  doorWalls.back().push_back(std::make_pair(j,i));
                    if((j == x && i == y) || (j == x+height-1 && i == y+width-1)
                       || (j == x && i == y+width-1) || (j == x+height-1 && i == y)){       //edges
                        continue;
                    }
                       
                       if((j == x && i == y+1) || (j == x+height-1 && i == y+width-2)       //the doors can't spawn on this places...
                       || (j == x+1 && i == y+width-1) || (j == x+height-2 && i == y)
                       
                       || (j == x+1 && i == y) || (j == x+height-2 && i == y+width-1)
                          || (j == x && i == y+width-2) || (j == x+height-1 && i == y+1)){
                           allWalls.back().push_back(std::make_pair(j,i));
                          // allRooms.back().push_back(std::make_pair(j,i));
                           continue;
                       }
                   
                    
                  /*  if((j == x && i == y+1) || (j == x+height-1 && i == y+width-2)       //the doors can't spawn on this places...
                    || (j == x+1 && i == y+width-1) || (j == x+height-2 && i == y)
                    
                    || (j == x+1 && i == y) || (j == x+height-2 && i == y+width-1)
                       || (j == x && i == y+width-2) || (j == x+height-1 && i == y+1)){
                        p_ConnectPoints.push_back(std::make_pair(j,i));
                    }*/
                    p_ConnectPoints.push_back(std::make_pair(j,i));
                    allWalls.back().push_back(std::make_pair(j,i));
                  //  allRooms.back().push_back(std::make_pair(j,i));
                }
                else {
                    room.push_back('-');
                //    allRooms.back().push_back(std::make_pair(j,i));
                    if(j+3 >= x+height || j-3 <= x || i + 3 >= y+width || i-3 <= y)
                        continue;
                    rooms.back().push_back(std::make_pair(j,i));
                }
            }
       
        int r = 0;
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++,r++)
                map[j][i] = room[r];
        
        for(int j = 0; j < HEIGHT; j++){
            for(int i = 0; i < WIDTH; i++)
                std::cout << map[j][i];
            std::cout << std::endl;
        }
        return 0;
    }
    
    bool CanPlaceRoom(int x, int y, int width, int height){
       if(isOverlapsed(x,y,width,height))
            return false;
       /* for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++)
                if(map[j][i] == '-') return false; //-
                */
        return x > 0 && y > 0 && x+height < HEIGHT && y+width < WIDTH;
    }
    
    std::vector<POS> changeRoom;
    bool isOverlapsed(int x, int y, int width, int height){ // a little bit shitty code :D
        auto PosOverlapsed = [&](std::pair<int,int> pos){
           for(int j = x; j < x + height; j++)
               for(int i = y; i < y + width; i++)
                   if(pos.first == j && pos.second == i)
                       return true;
           return false;
       };
               
       auto WholeRoomLapsed = [&] (const std::vector<std::pair<int,int>>& room) {
           int d = 0;
           for(int i = 0; i < room.size(); i++,d++){
               if(!PosOverlapsed(room[i])) return false;
               else {
                   changeRoom.erase(changeRoom.begin() + d);
                   d--;
               }
           }
           return true;
       };

        for(int j = 0; j < rooms.size(); j++){
            changeRoom = rooms[j];
           if(WholeRoomLapsed(rooms[j])) return true;
           else{
               rooms[j] = changeRoom;
           }
        }
       return false;
    }

    
    
    
  /*  bool FoundDoorWall(std::pair<int,int> p_door){
        for(int j = 0; j < allRooms.size(); j++)
            for(int i = 0; i < allRooms[j].size(); i++)
                if(allRooms[j][i] == p_door)
                    return true;
        for(int j = 0; j < map.size(); j++)
              for(int i = 0; i < map[j].size(); i++)
                  if((map[j][i] == '#' && p_door.first == j && p_door.second == i) || (map[j][i] == '-' && p_door.first == j && p_door.second == i))
                      return true;
        return false;
    }*/
};

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    Generation gen;
    gen.ProduceMap();
    
    for(int j = 0; j < gen.HEIGHT; j++){
        for(int i = 0; i < gen.WIDTH; i++)
            std::cout << gen.map[j][i];
        std::cout << std::endl;
    }
    return 0;
}