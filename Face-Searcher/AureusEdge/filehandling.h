#pragma once

#include "includes.h"

void SetDetectionParametersFromFile(CX_VIDEO p_video, const char* settingsPath, int cameraindex);

void SetResultsSettingsFromFile(CX_VIDEO p_video, const char* settingsPath, int cameraindex);

void SetParametersAndSettingsFromFiles(CX_VIDEO p_video, int cameraindex);

void WriteContentToFile(const char* content, const char* filepath);