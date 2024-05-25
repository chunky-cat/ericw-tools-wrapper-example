#include <iostream>
#include <string>

#include "rayApp.h"

int main()
{
    auto app = RayApp();

    auto start_time = std::chrono::high_resolution_clock::now();

    app.LoadMap("/path/to/your/level.map");
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << "map compiled in: " << time / std::chrono::milliseconds(1) << " ms\n";

    app.Run();
    return 0;
}