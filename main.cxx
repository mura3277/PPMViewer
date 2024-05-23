#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <filesystem>
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "tinyfiledialogs.h"

using namespace std;

bool QUIT = false;
string WORKING_DIR = filesystem::current_path().string();
const char* IMAGE_PATH = nullptr;
bool UPDATE_IMAGE = true;

char* readFile(const string& filename) {
    char* buffer;
    long length;
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) {
        perror("Failed: ");
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = static_cast<char*>(malloc(length));
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

uint8_t* loadPPM(char* imageFile, int* details) {
    if (strcmp(strtok(imageFile," \n"), "P3") != 0) {
        printf("Only PPM P3 format is supported!\n");
        return nullptr;
    }
    for (int i = 0; i < 3; ++i) details[i] = atoi(strtok(nullptr," \n"));
    auto* pixels = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * (details[0] * details[1]) * 3));
    for (int i = 0; i < details[0] * details[1]; ++i) {
        pixels[(i * 3) + 0] = atoi(strtok(nullptr," \n"));
        pixels[(i * 3) + 1] = atoi(strtok(nullptr," \n"));
        pixels[(i * 3) + 2] = atoi(strtok(nullptr," \n"));
    }
    return pixels;
}

void renderToolbar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {
                IMAGE_PATH = tinyfd_openFileDialog(nullptr, WORKING_DIR.c_str(), 0, nullptr, nullptr, 0);
                UPDATE_IMAGE = true;
            }
            if (ImGui::MenuItem("About..")) {
                tinyfd_messageBox(nullptr, "PPM Image Viewer by Muramasa", "ok", "info", 1);
            }
            if (ImGui::MenuItem("Exit")) {
                QUIT = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void renderPPMViewer(Texture* texture) {
    if (UPDATE_IMAGE && IMAGE_PATH) {
        UnloadTexture(*texture);
        int details[3] = {};
        uint8_t* pixels = loadPPM(readFile(IMAGE_PATH), details);
        Image image = { .data = pixels, .width = details[0], .height = details[1], .mipmaps = 1, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8};
        *texture = LoadTextureFromImage(image);
        UPDATE_IMAGE = false;
    }
    if (IMAGE_PATH) {
//        rlImGuiImage(texture);
        SetWindowSize(texture->width, texture->height);
        DrawTexture(*texture, 0, 0, WHITE);
    }
}

int main() {
    Texture texture = {0};
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(540, 540, "PPM Viewer");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    //Main game loop
    while (!WindowShouldClose() && !QUIT) {
        BeginDrawing();
            ClearBackground(DARKGRAY);
            rlImGuiBegin(); //start ImGui Content
                renderToolbar();
                renderPPMViewer(&texture);
            rlImGuiEnd(); //end ImGui Content
        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow(); //Close window and OpenGL context
    UnloadTexture(texture);
    return 0;
}
