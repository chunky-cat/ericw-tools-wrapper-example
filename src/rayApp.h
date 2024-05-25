#pragma once
#include <raylib.h>

#include <string>

using std::string;

class RayApp
{
public:
    RayApp();
    void Run();
    void LoadMap(string mappath);

private:
    Camera3D camera;
    std::vector<Mesh> meshes;
    Model model{0};
    float inverseScale = 24;
    Material defaultMaterial = LoadMaterialDefault();
};
