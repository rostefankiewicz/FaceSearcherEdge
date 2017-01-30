#include "Camera.h"

void RecordCameraInfo(Camera* camera, char* cameraInfo)
{
	strcpy(cameraInfo, "");
	char info[1024];

	sprintf(info, "Camera %d with id %s of media type %d on stream %d is currently ", camera->m_cameraindex, camera->m_cameraid, camera->m_type, camera->m_streamindex);
	strcat(cameraInfo, info);
	if (camera->m_running)
	{
		strcpy(info, "running\n");
		strcat(cameraInfo, info);
	}
	else
	{
		strcpy(info, "stopped\n");
		strcat(cameraInfo, info);
	}
	sprintf(info, "camera xml path %s\n ", camera->m_configPath);
	strcat(cameraInfo, info);
	sprintf(info, "ip url: %s\n", camera->m_url);
	strcat(cameraInfo, info);
}