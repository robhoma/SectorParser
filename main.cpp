#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <raylib.h>

using json = nlohmann::json;

struct worldData {
    std::vector<int> points;
    std::vector<int> colors;
};

bool loadWorldData(const std::string &filePath, std::vector<worldData> &worldSectors) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        std::cerr << "JSON ERROR 1 - Failed to open file: " << filePath << std::endl;
        return false;
    }

    json j;
    try {
        ifs >> j;
    } catch (const json::parse_error &e){
        std::cerr << "JSON ERROR 2 - Failed to parse JSON: " << e.what() << std::endl;
        return false;
    }

    if (!j.contains("sectors") || !j["sectors"].is_array()) {
        std::cerr << "JSON ERROR 3 - missing or invalid 'sectors' within JSON file.\n" << std::endl;
        return false;
    }

    worldSectors.clear();
    for (const auto &sectorObject : j["sectors"]) {
        if (!sectorObject.contains("points") || !sectorObject["points"].is_array()) {
            std::cerr << "CRITICAL JSON ERROR: 'points_x' missing or is not an array.\n";
            return false;
        }
        if (!sectorObject.contains("colors") || !sectorObject["colors"].is_array()) {
            std::cerr << "CRITICAL JSON ERROR: 'colors' missing or is not an array.\n";
            return false;
        }

        worldData wD;

        for (const auto &p : sectorObject["points"]) {
            wD.points.push_back(p.get<int>());
        }
        for (const auto &c : sectorObject["colors"]) {
            wD.colors.push_back(c.get<int>());
        }

        worldSectors.push_back(std::move(wD));
    }

    return true;
}

bool running = true;

int main() {
    std::vector<worldData> worldSectors;

    if (!loadWorldData("../test.json", worldSectors)) {
        std::cerr << "Failed to load 'test.json'. Exiting...\n";
        return 1;
    }

    // Debug
    std::cout << "Loaded " << worldSectors.size() << " sectors:\n";
    for (size_t s = 0; s < worldSectors.size(); s++) {
        std::cout << "  Sector " << s << " has "
                  << worldSectors[s].points.size() << " points and "
                  << worldSectors[s].colors.size() << " color components\n";
    }

    InitWindow(800, 800, "sector parsing");
    SetTargetFPS(120);


    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        for (auto &sector : worldSectors) {
            if (sector.colors.size() == 4) {
                Color sectorColor = {
                    (unsigned char)(sector.colors[0]),
                    (unsigned char)(sector.colors[1]),
                    (unsigned char)(sector.colors[2]),
                    (unsigned char)(sector.colors[3])
                };

                for (size_t i = 0; i + 1 < sector.points.size(); i+=2) {
                    int x = sector.points[i];
                    int y = sector.points[i + 1];

                    DrawCircle(x, y, 10, sectorColor);
                }
            }
        }
        EndDrawing();
    }


    CloseWindow();
    return 0;
}