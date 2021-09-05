#include <array>
#include <vector>
#include <time.h>
#include <string>

enum TileType {
	Tile_Empty = '-',
	Tile_Wall = '#',
	Tile_Floor = '.',
	Tile_Door = '%',
	Tile_Player = 'P',
};

class Generation {
public:
    using POS = std::pair<int,int>;
    static constexpr int WIDTH = 32, HEIGHT = 32;
    std::vector<std::string> map;
    std::vector<std::vector<POS>> rooms;
    std::vector<POS> p_ConnectPoints; //possible connect points
    std::vector<std::vector<POS>> allWalls;
    std::time_t start;
    void ProduceMap(){
        bool success = false;
        while(!success){
            success = true;
            std::vector<char> room;
            rooms.clear();
            allWalls.clear();
            p_ConnectPoints.clear();
            start = std::time(NULL);
            for(int j = 0; j < HEIGHT; j++) {
				map.push_back("");
                for(int i = 0; i < WIDTH; i++) {
                    map[j].push_back(Tile_Empty); //E
				}
			}
                   
            for(int i = 0; i < 10; i++)
                if(GetRoom(room, 8, 10) == -1) success = false;
            
            if(GetRoom(room, 10, 10) == -1) success = false;
            
        }
        auto VerticalCheck = [&](int k, int i){
            for(int s = k; s < HEIGHT; s++){
                if(map[s][i] == Tile_Wall){
                    for(int d = k; d > -1; d--){
                        if(map[d][i] == Tile_Wall)
                            return true;
                    }
                    return false;
                }
            }
            return false;
        };
      
        for(int k = 0; k < HEIGHT; k++)
            for(int j = 0; j < WIDTH; j++){
                if(map[k][j] == Tile_Wall) //startpoint
                    for(int i = j+1; i < WIDTH; i++){
                        
                        if(map[k][i] == Tile_Wall ){ //endpoint
                            if(i == j+1 || !VerticalCheck(k,i-1))
                                break;
                            for(int s = j+1; s < i; s++)
                                map[k][s] = Tile_Floor;
                            break;
                        }
                    }
            }
        
        SpawnDoors();
        std::pair<int,int> pos;
        do{
            int x = rand() % (rooms.size()-1);
            int y = rand() % rooms[x].size();
            pos = rooms[x][y];
        }while(map[pos.first][pos.second] == Tile_Wall || map[pos.first][pos.second] == Tile_Door);
        map[pos.first][pos.second]  = Tile_Player;
    }
    
    void SpawnDoors(){
        std::vector<std::vector<POS>> walls;
     
        for(int j = 0; j < allWalls.size()-1; j++){
            for(int s = 0; s < 2; s++){
                int i = rand() % allWalls[j].size();
            
                int k = 0;
                while(1){
                    auto pos = allWalls[j][i+k];
                    
                    if(pos.second <= 0 || pos.second > WIDTH || map[pos.first][pos.second + 1] != Tile_Floor ||
                       map[pos.first][pos.second - 1] != Tile_Floor)
                        break;
                    else {
                        allWalls[j].erase(allWalls[j].begin() + i+k);
                        map[pos.first][pos.second] = Tile_Floor;
                    }
                    
                    if(k+i >= allWalls[j].size()-1)
                        break;
                    k++;
                }
            }
        }
        
        auto WallExist = [&](POS pos){
            for(int k = 0; k < walls.size(); k++)
                for(int l = 0; l < walls[k].size();l++){
                    if(walls[k][l] == pos) return true;
                }
            return false;
        };

        walls.push_back(std::vector<POS>());
        for(int j = 0; j < allWalls.size(); j++){
            for(int i = 0; i < allWalls[j].size();i++){
                auto pos = allWalls[j][i];
                if(map[pos.first][pos.second] == Tile_Floor)
                    continue;
                
               
                if(WallExist(pos))
                    continue;
                
                 if(pos.second >= 0 && pos.second < WIDTH && map[pos.first][pos.second + 1] == Tile_Floor &&
                        map[pos.first][pos.second - 1] == Tile_Floor)
                    walls.back().push_back(pos);
                else if(pos.first >= 0 && pos.first < HEIGHT && map[pos.first + 1][pos.second] == Tile_Floor &&
                   map[pos.first - 1][pos.second] == Tile_Floor)
                    walls.back().push_back(pos);
                
                else if(walls.back().size() != 0)
                    walls.push_back(std::vector<POS>());
            }
        }
        
        auto countEmpty = [&](int x, int y) {
            int ret = 0;
            if(y+1 < WIDTH && map[x][y+1] == Tile_Floor) ret++;
            if(y-1 >= 0 && map[x][y-1] == Tile_Floor) ret++;
            if(x-1 >= 0 && map[x-1][y] == Tile_Floor) ret++;
            if(x+1 < HEIGHT && map[x+1][y] == Tile_Floor) ret++;
            return ret;
        };
        
        for(int j = 0; j < walls.size();){
              if(walls[j].size() != 0) {
                  int i = rand() % (walls[j].size());
                  auto pos = walls[j] [i];
                  if(map[pos.first][pos.second] == Tile_Floor){
                      walls[j].erase(walls[j].begin() + i);
                      continue;
                  }
                  if((pos.second < WIDTH && map[pos.first][pos.second + 1] == Tile_Door) || (pos.second >= 0 && map[pos.first][pos.second - 1] == Tile_Door) 
                    || countEmpty(pos.first, pos.second) > 2){
                      j++;
                      continue;
                  }
                      
                  else if((pos.first < HEIGHT && map[pos.first + 1][pos.second] == Tile_Door) 
                        || (pos.first >= 0 && map[pos.first - 1][pos.second] == Tile_Door  )){
                      j++;
                      continue;
                  }
                    map[pos.first][pos.second] = Tile_Door;
              }
            j++;
          }
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
                if(time(NULL) - start >= 0.1) return -1;
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
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++){
                if(j == x || i == y || i == width+y-1 || j == height+x-1) {
                    room.push_back(Tile_Wall); //W
                    if((j == x && i == y) || (j == x+height-1 && i == y+width-1)
                       || (j == x && i == y+width-1) || (j == x+height-1 && i == y)){       //edges
                        continue;
                    }
                       
                       if((j == x && i == y+1) || (j == x+height-1 && i == y+width-2)       //the doors can't spawn on this places...
                       || (j == x+1 && i == y+width-1) || (j == x+height-2 && i == y)
                       
                       || (j == x+1 && i == y) || (j == x+height-2 && i == y+width-1)
                          || (j == x && i == y+width-2) || (j == x+height-1 && i == y+1)){
                           allWalls.back().push_back(std::make_pair(j,i));
                           continue;
                       }
                    p_ConnectPoints.push_back(std::make_pair(j,i));
                    allWalls.back().push_back(std::make_pair(j,i));
                }
                else {
                    room.push_back(Tile_Floor);
                    if(j+3 >= x+height || j-3 <= x || i + 3 >= y+width || i-3 <= y)
                        continue;
                    rooms.back().push_back(std::make_pair(j,i));
                }
            }
       
        int r = 0;
        for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++,r++)
                map[j][i] = room[r];
        
        /*for(int j = 0; j < HEIGHT; j++){
            for(int i = 0; i < WIDTH; i++)
                std::cout << map[j][i];
            std::cout << std::endl;
        }*/
        return 0;
    }
    
    bool CanPlaceRoom(int x, int y, int width, int height){
       if(isOverlapsed(x,y,width,height))
            return false;
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

        for (int j = 0; j < rooms.size(); j++){
            changeRoom = rooms[j];
           if (WholeRoomLapsed(rooms[j])) return true;
           else
                rooms[j] = changeRoom;
        }
       return false;
    }
};
