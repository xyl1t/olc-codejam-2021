#include <vector>
#include <array>
#include <string>
#include <iostream>

class Generation{
    
public:
    enum TileType {
        Tile_Empty = '-',
        Tile_Wall = '#',
        Tile_Floor = '.',
        Tile_Door = '%',
        Tile_Player = 'P',
        Tile_Enemy = 'E'
    };
    
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
    bool isValidSpace = true;

    Generation(int WIDTH, int HEIGHT, int borderLeft, int borderRight, int borderUp, int borderDown);
    int Start(int amountOfRooms, int minWidth, int maxWidth, int minHeight, int maxHeight);
    void TryFillOutSpacing(POS pos,std::vector<POS>& spacing);
    void OpenSpace(POS pos,std::vector<POS>& spacing);
    void SpawnDoors(int verticalShift, int horizontalShift);
    void SpawnHouse(const int& minWidth, const int& maxWidth, const int& minHeight, const int& maxHeight, const POS& pos = POS());
    bool CanPlaceRoom(int x, int y, int width, int height);
};