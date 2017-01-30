#pragma once

#include "includes.h"
#include "Camera.h"

bool WordToBool(char* value);

// The value of the tag gets stored in value
bool GetValueOfTag(TiXmlElement *settings, char* tag, char* value);

void GetEmailConfig(Camera *camera);

void GetCameraInfo(Camera *camera);

void GetControlSettings(Camera *camera);

void GetResultsSettings(Camera *camera);

bool AddCameraIDToXml(Camera* camera, const char* xmlName);

bool AddCameraIDToXmls(Camera* camera);