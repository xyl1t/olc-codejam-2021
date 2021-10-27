#include <iostream>
#include "generation.hpp"
#include <chrono>

int main() {
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    srand(millis);
    Generation gen(48,48,4,4,4,4);
    if(gen.Start(15,6,10,6,10) == -1)
        std::cout << "ERROR: The rooms can't fit into the map, decrease the minimum rooms size or the amount" << std::endl;
    else
        for(int j = 0; j < gen.HEIGHT; j++)
            std::cout << gen.map[j] << std::endl;
    return 0;
}