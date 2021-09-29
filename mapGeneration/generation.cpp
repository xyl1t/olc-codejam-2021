#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <time.h>

enum TileType {
    Tile_Empty = '-',
    Tile_Wall = '#',
    Tile_Floor = '.',
    Tile_Door = '%',
    Tile_Player = 'P',
    Tile_Enemy = 'E'
};

class Generation{
   
public:
    struct POS{
        int y = 0, x = 0;
        char c;
        POS(int x, int y):x(x),y(y){}
        POS(int x, int y,char c):x(x),y(y),c(c){}
        POS(){}
        POS operator+(const POS& p){
            return POS(this->x + p.x,this->y + p.y);
        }
        POS operator-(const POS& p){
            return POS(this->x - p.x,this->y - p.y);
        }
        bool operator==(const POS& p){
            return this->x == p.x && this->y == p.y;
        }
    };

    int WIDTH = 0,HEIGHT = 0;
    int dungeonWidth = 0, dungeonHeight = 0;
    int borderLeft = 0, borderRight = 0, borderUp = 0, borderDown = 0;
    std::vector<std::string> map;
    std::vector<std::vector<POS>> rooms; // all rooms
    std::vector<POS> walls;  //all walls, on which the rooms can spawn, the corneres are not included here
    std::vector<std::array<std::vector<POS>,4>> connectpoints; //the doors can spawn here
    std::vector<std::vector<POS>> oarea_rooms; // all rooms, but they contain only the inner room section(if this section is overlapsed by a room, it has to respawn)
    int spacing = 2; // room spacing
    int pdirectionY = -1,pdirectionX = -1;
    
    Generation(int WIDTH, int HEIGHT, int borderLeft, int borderRight, int borderUp, int borderDown)
    :WIDTH(WIDTH),HEIGHT(HEIGHT),borderLeft(borderLeft),borderRight(borderRight),
    borderUp(borderUp),borderDown(borderDown){
        map.reserve(HEIGHT);
        for(int j = 0; j < HEIGHT; j++){
            map.push_back("");
            for(int i = 0; i < WIDTH; i++) {
                map[j].push_back(Tile_Empty);
            }
        }
        
        dungeonHeight = HEIGHT - (borderDown + borderUp);
        dungeonWidth = WIDTH - (borderLeft + borderRight);
    }
    
    int Start(int amountOfRooms, int minWidth, int maxWidth, int minHeight, int maxHeight){
        while(minHeight * minWidth * amountOfRooms > dungeonWidth * dungeonHeight)
            amountOfRooms--;

        if(amountOfRooms == 0) return -1;

        rooms.reserve(amountOfRooms);
        oarea_rooms.reserve(amountOfRooms);

        //not sure if thats neccesery
        POS spawnPoint = POS(dungeonHeight / 2, dungeonWidth / 2);
        SpawnHouse(minWidth, maxWidth, minHeight, maxHeight, spawnPoint);
        
        for(int i = 0; i < amountOfRooms; i++){
            SpawnHouse(minWidth, maxWidth, minHeight, maxHeight);
        }
        
        //shiftPart
        POS leftp =  POS(0,dungeonWidth+1),
        rightp = POS(0,-1),
        upp = POS(dungeonHeight+1,0),
        downp = POS(-1,0);
        
        //find the leftpoint rightp upp downp of the whole dungeon
        for(int j = 0; j < rooms.size(); j++){
            for(int i = 0; i < rooms[j].size(); i++){
                if(leftp.y > rooms[j][i].y)
                    leftp = rooms[j][i];
                if(rightp.y < rooms[j][i].y)
                    rightp = rooms[j][i];
                
                
                if(downp.x < rooms[j][i].x)
                    downp = rooms[j][i];
                if(upp.x > rooms[j][i].x)
                    upp = rooms[j][i];
            }
        }
        
        int leftspan = leftp.y;
        int rightspan =  dungeonWidth - rightp.y;
        int upspan = upp.x;
        int downspan = dungeonHeight - downp.x;
        
        //horizontal shift
        int diff;
        int horizontalShift = 0, verticalShift = 0;
        
        //then center the whole rooms on the dungeon
        while(1){
            diff = rightspan - leftspan;
            if(diff <= 1 && diff >= -1){
                break;
            } else{
                if(diff < 0) { leftspan--; rightspan++; horizontalShift--; } //left shift
                else { leftspan++; rightspan--; horizontalShift++; } //right shift
            }
        }
        
        while(1){
            diff = downspan - upspan;
            if(diff <= 1 && diff >= -1){
                break;
            } else{
                if(diff < 0) { upspan--; downspan++; verticalShift--; }
                else { downspan--; upspan++; verticalShift++; }
            }
        }
        
        
        //write the dungeon to map
        for(int i = 0; i < rooms.size(); i++)
            for(int j = 0; j < rooms[i].size(); j++){
                POS p = rooms[i][j];
                map[p.x+borderUp + verticalShift][p.y + borderRight + horizontalShift] = p.c;
            }
        
        //spawn player
        int playerRoom = rand() % rooms.size();
        POS p;
        do{
            int j = rand() % rooms[playerRoom].size();
             p = rooms[playerRoom][j];
        }while(map[p.x + borderUp + verticalShift][p.y + borderRight + horizontalShift] == Tile_Wall);
        map[p.x + borderUp + verticalShift][p.y+ borderRight + horizontalShift] = Tile_Player;
        rooms.erase(rooms.begin() + playerRoom);
        
        
        //spawning enemies
        int enemyTotal = 0;
        const int enemyDensity = 10 / 1;
        for(int j = 0; j < rooms.size(); j++){
            int floors = 0;
            for(int i = 0; i < rooms[j].size(); i++){
                POS p = POS(rooms[j][i].x + borderUp + verticalShift, rooms[j][i].y + borderRight + horizontalShift);
                if(map[p.x][p.y] != Tile_Wall)
                    floors++;
            }
            
            int enemies = floors / enemyDensity;
            enemyTotal += enemies;
            int eamount = 0;
            do{
                int i = rand() % rooms[j].size();
                POS p = POS(rooms[j][i].x + borderUp + verticalShift, rooms[j][i].y + borderRight + horizontalShift);
                if(map[p.x][p.y] != Tile_Wall && map[p.x][p.y] != Tile_Enemy){
                    eamount++;
                    map[p.x][p.y] = Tile_Enemy;
                }
            }while(eamount < enemies);
        }
         //for(int l = 0; l < HEIGHT; l++)
          //   std::cout << map[l] << std::endl;
        
        //fill out the empty tiles here...
        std::vector<POS> emptyspace;
        for(int k = 0; k < HEIGHT-2; k++)
            for(int j = 0; j < WIDTH-2; j++){
                if(map[k][j] == Tile_Wall && map[k][j+1] == Tile_Empty && map[k-1][j+1] == Tile_Wall){
                    for(int i = j+1; i < WIDTH-2; i++)
                        if(map[k][i] == Tile_Wall){
                            emptyspace.push_back(POS(k,j+1));
                            isValid = true;
                            TryFillOutSpacing(POS(k,j+1), emptyspace);
                            for(POS p : emptyspace)
                                map[p.x][p.y] = Tile_Floor;
                            emptyspace.clear();
                        }
                }
            }
        
        SpawnDoors(verticalShift, horizontalShift);
        
        
        //removing the small space between walls
        for(int j = 0; j < HEIGHT; j++){
            for(int i = 0; i < WIDTH; i++){
                if(map[j][i] == Tile_Wall){
                    if (i+2 < WIDTH && i-1 >= 0 && map[j][i-1] == Tile_Floor && map[j][i+1] == Tile_Floor && map[j][i+2] == Tile_Wall)
                        map[j][i] = Tile_Floor;
                    
                    if (i-2 >= 0 && i+1 < WIDTH && map[j][i+1] == Tile_Floor && map[j][i-1] == Tile_Floor && map[j][i-2] == Tile_Wall)
                        map[j][i] = Tile_Floor;
               
                    if (j+2 < HEIGHT && j-1 >= 0 && map[j-1][i] == Tile_Floor && map[j+1][i] == Tile_Floor && map[j+2][i] == Tile_Wall)
                        map[j][i] = Tile_Floor;
                
                    if (j-2 >= 0 && j+1 < HEIGHT && map[j+1][i] == Tile_Floor && map[j-1][i] == Tile_Floor && map[j-2][i] == Tile_Wall)
                        map[j][i] = Tile_Floor;
                }
            }
        }
        return 0;
    }
    bool isValid = true;
    void TryFillOutSpacing(POS pos,std::vector<POS>& spacing){
        if (map[pos.x][pos.y] == Tile_Wall) return;
        OpenSpace(POS(pos.x-1,pos.y),spacing);
        OpenSpace(POS(pos.x+1,pos.y),spacing);
        OpenSpace(POS(pos.x,pos.y-1),spacing);
        OpenSpace(POS(pos.x,pos.y+1),spacing);
    }
    
    void OpenSpace(POS pos,std::vector<POS>& spacing){
        if(!isValid) return;
        if(!spacing.empty() && spacing[spacing.size()-1] == pos)
            return;
        
        if(pos.x >= dungeonWidth || pos.y >= dungeonHeight || pos.x <=  borderUp || pos.y <= borderLeft){
            isValid = false;
            spacing.clear();
            return;
        }
        
        if(map[pos.x][pos.y] == Tile_Wall)
            return;
        
        for(POS p : spacing)
            if(p == pos) return;
        
        spacing.push_back(pos);
        TryFillOutSpacing(pos, spacing);
    }

    void SpawnDoors(int verticalShift, int horizontalShift){
        POS p;
        for(int j = 0; j < connectpoints.size(); j++){ //vector
            for(int i = 0; i < connectpoints[j].size(); i++){ //array
                
                int c = 0;
                while(1){
                    if(c > connectpoints[j][i].size()-1 || connectpoints[j][i].empty())
                        break;
                    p = connectpoints[j][i][c];
                    p.x += borderUp + verticalShift;
                    p.y += borderRight + horizontalShift;
                    if(!(map[p.x+1][p.y] == Tile_Floor && map[p.x-1][p.y] == Tile_Floor) &&
                       !(map[p.x][p.y+1] == Tile_Floor && map[p.x][p.y-1] == Tile_Floor)){
                        connectpoints[j][i].erase(connectpoints[j][i].begin() + c);
                        continue;
                    }
                    c++;
                }
                
                if(connectpoints[j][i].empty())
                    continue;
                
                if(connectpoints[j][i].size() == 1){
                    p = connectpoints[j][i][0];
                    p.x += borderUp + verticalShift;
                    p.y += borderRight + horizontalShift;
                    map[p.x][p.y] = Tile_Floor;
                    continue;
                }
                
                int start;
                int end;
                
                do{
                    start = rand() % (connectpoints[j][i].size()-1);
                    end = rand() % (connectpoints[j][i].size() - start);
                } while((start+end)-start < 1);
                
                for(int k = start; k < start + end; k++){
                    p = connectpoints[j][i][k];
                    p.x += borderUp + verticalShift;
                    p.y += borderRight + horizontalShift;
                    map[p.x][p.y] = Tile_Floor;
                }
            }
            
           // for(int l = 0; l < HEIGHT; l++)
              //  std::cout << map[l] << std::endl;
        }
       
    }

    void SpawnHouse(const int& minWidth, const int& maxWidth, const int& minHeight, const int& maxHeight, const POS& pos = POS()){
        POS spawn;
        int width = 0;
        int height = 0;
        int y = 0;
        int x = 0;
    
        if(pos.y != 0 && pos.x != 0){
            //spawn the first room exactly in the middle of the map.
            width = minWidth + (rand() % (maxWidth - minWidth));
            height = minHeight + (rand() % (maxHeight - minHeight));
            x = pos.x-height/2;
            y = pos.y-width/2;
        } else {
            int dirX = 0, dirY = 0;
             do {
                if (minHeight - maxHeight != 0 && minWidth - maxWidth != 0) {
                    width = minWidth + (rand() % (maxWidth - minWidth));
                    height = minHeight + (rand() % (maxHeight - minHeight));
                } else {
                    width = maxWidth;
                    height = maxHeight;
                }
                int i = rand() % walls.size();
                 
                 dirX = (rand() % 2 == 0 ? 0 : 1);
                 dirY = (rand() % 2 == 0 ? 0 : 1);
                 x = walls[i].x - dirX * (height-1);
                 y = walls[i].y - dirY * (width-1);
            } while((dirX == pdirectionX && dirY == pdirectionY) || !CanPlaceRoom(x,y,width,height));
            pdirectionY = dirY;
            pdirectionX = dirX;
        }
        
        
        std::vector<POS> border;
        rooms.push_back(std::vector<POS>());
        oarea_rooms.push_back(std::vector<POS>());
        connectpoints.push_back(std::array<std::vector<POS>,4>());
        
         for(int j = x; j < height+x; j++)
            for(int i = y; i < width+y; i++){
                if(j == x || i == y || i == width+y-1 || j == height+x-1) {
                    rooms.back().push_back(POS(j,i,Tile_Wall));
                    if((j == x && i == y) || (j == x+height-1 && i == y+width-1)
                       || (j == x && i == y+width-1) || (j == x+height-1 && i == y)){       //edges
                        continue;
                    }
                    
                    if(j == x)
                        connectpoints.back()[0].push_back(POS(j,i));
                    if(j == height+x-1)
                        connectpoints.back()[1].push_back(POS(j,i));
                    if(i == y+width-1)
                        connectpoints.back()[2].push_back(POS(j,i));
                    if(i == y)
                        connectpoints.back()[3].push_back(POS(j,i));

                    if((j == x && i == y+1) || (j == x+height-1 && i == y+width-2)       //the rooms can't spawn on this places...
                    || (j == x+1 && i == y+width-1) || (j == x+height-2 && i == y)
                    
                    || (j == x+1 && i == y) || (j == x+height-2 && i == y+width-1)
                        || (j == x && i == y+width-2) || (j == x+height-1 && i == y+1)){
                        continue;
                    }
                    walls.push_back(POS(j,i));
                }
                else {
                    rooms.back().push_back(POS(j,i,Tile_Floor));
                    if(j + spacing >= x+height || j - spacing <= x || i + spacing >= y+width || i - spacing <= y)
                        continue;
                    oarea_rooms.back().push_back(POS(j,i,Tile_Floor));
                }
            }
        
    }

    bool CanPlaceRoom(int x, int y, int width, int height){
        if( x < 0 || y < 0 || x+height >= dungeonHeight || y+width >= dungeonWidth){
            return false;
        }
        
        auto tempRooms = rooms;
        auto tempORooms = oarea_rooms;
        auto tempWalls = walls;
        // iterating through the room
        for(int X = x; X < height+x; X++)
            for(int Y = y; Y < width+y; Y++){
                
                //checking here if the walls are too close to the others room walls, if its true then the room can't be placed
                if(X == x){
                    for(int i =1; i < spacing; i++){
                        for(int j = 0; j < walls.size(); j++){
                            if(walls[j].x == X-i && walls[j].y == Y){
                                return false;
                            }
                        }
                    }
                } if(X == height+x-1){
                    for(int i = 1; i < spacing; i++){
                        for(int j = 0; j < walls.size(); j++){
                            if(walls[j].x == X+i && walls[j].y == Y){
                                return false;
                            }
                        }
                    }
                } if(Y == y){
                    for(int i = 1; i < spacing; i++){
                        for(int j = 0; j < walls.size(); j++){
                            if(walls[j].y == Y-i && walls[j].x == X){
                                return false;
                            }
                        }
                    }
                } if(Y == width+y-1){
                    for(int i = 1; i < spacing; i++){
                        for(int j = 0; j < walls.size(); j++){
                            if(walls[j].y == Y+i && walls[j].x == X){
                                return false;
                            }
                        }
                    }
                }
                
                //check if the room overlapses any other existing room, if it does the room can't be placed
                // iterating through all existing rooms
               for(int i = 0; i < tempRooms.size(); i++){
                    for(int j = 0; j < tempRooms[i].size(); j++){
                        
                        if(tempRooms[i][j].x == X && tempRooms[i][j].y == Y){
                            for(int k = 0; k < tempORooms[i].size(); k++){
                                if(tempORooms[i][k].x == X && tempORooms[i][k].y == Y){
                                    tempORooms[i].erase(tempORooms[i].begin() + k);
                                    break;
                                }
                            }
                            
                            tempRooms[i].erase(tempRooms[i].begin() + j);
                            break;
                        }
                    }
                    
                    if(tempORooms[i].empty()) return false;

                    //removing the overlapsed walls
                    int j = 0;
                    while(true){
                        if(tempWalls[j].x == X && tempWalls[j].y == Y)
                            tempWalls.erase(tempWalls.begin() + j);
                        else j++;
                        if(j >= tempWalls.size()) break;
                    }
                }
            }

        oarea_rooms = tempORooms;
        rooms = tempRooms;
        walls = tempWalls;
        return true;
    }
};
