
#include <iostream>

#include <ericw-tools-wrapper/wrapper.h>
#include <qbsp/map.hh>

#include "rayApp.h"
#include <raymath.h>
#include <OpenGL/gl3.h>

RayApp::RayApp()
{
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Ericw Tools Wrapper Example");
    SetTargetFPS(60);
    camera = {{2.0f, 5.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};
}

void RayApp::LoadMap(string mapPath)
{
    auto m = ericwtoolswrapper::QBsp_CreateBSP(mapPath);
    auto ws = m->WorldSpawn();
    for (const auto f : ws->Faces())
    {
        auto mesh = Mesh{0};
        mesh.triangleCount = f->indices.size() / 3;
        mesh.vertexCount = f->verts.size();
        mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
        mesh.indices = (unsigned short *)MemAlloc(f->indices.size() * sizeof(unsigned short));
        int iv = 0;
        for (int v = f->verts.size() - 1; v >= 0; v--)
        {
            mesh.vertices[iv++] = -f->verts[v][0] / inverseScale;
            mesh.vertices[iv++] = f->verts[v][2] / inverseScale;
            mesh.vertices[iv++] = f->verts[v][1] / inverseScale;
        }

        for (int i = 0; i < f->indices.size(); i++)
        {
            mesh.indices[i] = f->indices[i];
        }

        UploadMesh(&mesh, false);
        meshes.push_back(mesh);
    }

    defaultMaterial = LoadMaterialDefault();
    model.transform = MatrixIdentity();
    model.meshCount = meshes.size();
    model.meshes = (Mesh *)MemAlloc(model.meshCount * sizeof(Mesh));
    model.materials = (Material *)MemAlloc(1 * sizeof(Material));
    model.materials[0] = defaultMaterial;
    model.meshMaterial = (int *)MemAlloc(model.meshCount * sizeof(int));

    for (int m = 0; m < meshes.size(); m++)
    {
        model.meshes[m] = meshes[m];
        model.meshMaterial[m] = 0;
    }
}

void RayApp::Run()
{
    DisableCursor();
    // glFrontFace(GL_CW);
    Mesh m = GenMeshCube(2, 2, 2);
    Model mo = LoadModelFromMesh(m);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_M))
        {
            EnableCursor();
        }

        if (IsCursorHidden())
        {
            UpdateCamera(&camera, CAMERA_FREE);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawModel(model, {0, 0, 0}, 1, BLUE);
        DrawModelWires(model, {0, 0, 0}, 1, RED);
        DrawGrid(64, 1.0f); // Draw a grid

        EndMode3D();
        DrawFPS(10, 10);

        EndDrawing();
    }
}