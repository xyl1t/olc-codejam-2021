//WARNING: THIS IS THE WORST CODE YOU HAVE EVER SEEN ON THE INTERNET, PLEASE LOOK AWAY!!

#include <iostream>
#include "generation.cpp"

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    Generation gen;
    gen.ProduceMap();
    
    //comment this out, if you don't need to see the map
    for(int j = 0; j < gen.HEIGHT; j++){ 
        for(int i = 0; i < gen.WIDTH; i++)
            std::cout << gen.map[j][i];
        std::cout << std::endl;
    }
    return 0;
}