#pragma once

#include "includes.h"
#include "EmailConfig.h"

struct Camera
{
	CX_AUREUS m_aureus;
	CX_VIDEO m_video;
	int m_type; // media 0, usb 1, ip 2
	int m_streamindex;
	int m_cameraindex;
	EmailConfig *m_email;
	char m_configPath[1024];
	char m_location[1024];
	char m_cameraid[1024];
	char m_connection_info[1024];
	char m_url[1024];
	char m_save_xml_folder[1024];
	bool m_running; // Is camera running or not
	bool m_unexpectedlyterminated;
	bool m_showProgress; // Print stuff out in frame callback
	int m_videonumber; // For media type
};

void RecordCameraInfo(Camera* camera, char* cameraInfo);