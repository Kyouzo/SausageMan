#pragma once
bool started = false;

struct Vars
{
    bool isEspEnabled = false;
    bool EspLine = false;
    bool EspBox = false;
    bool EspHealth = false;
    bool EspDistance = false;
    bool EspName = false;
    bool EspCount = false;
    bool EspTeam = false;
    bool EspBone = false;
    float distance = 50.0f;

    bool isAimbotEnabled = false;               // Always on by default
    float aimbotFOV = 10.0f;                    // Field of view in degrees
    float aimbotSmoothness = 5.0f;              // Smoothness factor (higher = smoother)
    int aimbotBone = 0; // Default to head
    bool aimbotTargetBots = true;               // Toggle for targeting bots
    bool aimbotTargetPlayers = true;            // Toggle for targeting players
    bool aimbotDrawFOV = true;                  // Draw FOV circle
};
Vars var;