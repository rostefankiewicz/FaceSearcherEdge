#include "filehandling.h"

using namespace std;

void SetDetectionParametersFromFile(CX_VIDEO p_video, const char* settingsPath, int cameraindex)
{
	char msg[1024];

	int version;
	float face_detect_top;
	float face_detect_left;
	float face_detect_height;
	float face_detect_width;
	float face_detect_min_height_prop;
	float face_detect_max_height_prop;
	int frame_interval;
	int reduce_frame_step;
	bool generate_templates;
	bool perform_ranking;
	float verification_thresh;

	ifstream settingsFile;
	settingsFile.open(settingsPath);
	if (!settingsFile)
	{
		printf("Could not open control settings %s for camera %d\n", settingsPath, cameraindex);
	}

	// Read the settings from the file
	string line;
	while (settingsFile >> line)
	{
		size_t indexOfEquality = line.find("=");
		if (indexOfEquality == string::npos)
		{
			printf("In control settings file %s on line %s there is no =\n", settingsPath, line);
			return;
		}
		string parameter = line.substr(0, indexOfEquality);
		string value = line.substr(indexOfEquality + 1);
		if (parameter == "version")
		{
			version = stoi(value);
			printf("version: %d ", version);
		}
		else if (parameter == "face_detect_top")
		{
			face_detect_top = stof(value);
			printf("face_detect_top: %f ", face_detect_top);
		}
		else if (parameter == "face_detect_left")
		{
			face_detect_left = stof(value);
			printf("face_detect_left: %f ", face_detect_left);
		}
		else if (parameter == "face_detect_height")
		{
			face_detect_height = stof(value);
			printf("face_detect_height: %f ", face_detect_height);
		}
		else if (parameter == "face_detect_width")
		{
			face_detect_width = stof(value);
			printf("face_detect_width: %f ", face_detect_width);
		}
		else if (parameter == "face_detect_min_height_prop") {
			face_detect_min_height_prop = stof(value);
			printf("face_detect_min_height_prop: %f ", face_detect_min_height_prop);
		}
		else if (parameter == "face_detect_max_height_prop")
		{
			face_detect_max_height_prop = stof(value);
			printf("face_detect_max_height_prop: %f ", face_detect_max_height_prop);
		}
		else if (parameter == "frame_interval")
		{
			frame_interval = stoi(value);
			printf("frame_interval: %d ", frame_interval);
		}
		else if (parameter == "reduce_frame_step")
		{
			reduce_frame_step = stoi(value);
			printf("reduce_frame_step: %d ", reduce_frame_step);
		}
		else if (parameter == "generate_templates")
		{
			value == "true" ? generate_templates = true : generate_templates = false;
			printf("generate_templates: %d ", generate_templates);
		}
		else if (parameter == "perform_ranking")
		{
			value == "true" ? perform_ranking = true : perform_ranking = false;
			printf("perform_ranking: %d ", perform_ranking);
		}
		else if (parameter == "verification_thresh")
		{
			verification_thresh = stof(value);
			printf("verification_thresh: %f ", verification_thresh);
		}
	}
	printf("\n");
	settingsFile.close();

	// Assign the settings
	if (!CX_SetDetectionParameters(p_video, face_detect_top, face_detect_left, face_detect_height, face_detect_width, face_detect_min_height_prop, face_detect_max_height_prop, msg))
	{
		printf("FAILED to set detection parameters for camera d:\n%s\n\n", cameraindex, msg);
	}
	if (0 == CX_SetFrameInterval(p_video, frame_interval, msg))
	{
		printf("FAILED to set frame interval for camera %d:\n%s\n\n", cameraindex, msg);
	}
	if (0 == CX_SetFrameReductionStep(p_video, reduce_frame_step, msg))
	{
		printf("FAILED to set frame reductions step for camera %d:\n%s\n\n", cameraindex, msg);
	}
	if (0 == CX_SetGenerateTemplatesFlag(p_video, generate_templates, msg))
	{
		printf("FAILED to set generate templates flag for camera %d:\n%s\n\n", cameraindex, msg);
	}
	if (0 == CX_SetPerformGalleryFRFlag(p_video, perform_ranking, msg))
	{
		printf("FAILED to set perform ranking flag for camera %d:\n%s\n\n", cameraindex, msg);
	}
	if (0 == CX_SetVerificationThreshold(p_video, verification_thresh, msg))
	{
		printf("FAILED to set verification threshold for camera %d:\n%s\n\n", cameraindex, msg);
	}
}


void SetResultsSettingsFromFile(CX_VIDEO p_video, const char* settingsPath, int cameraindex)
{
	char msg[1024];

	// The setting variables
	bool save_xml_results;
	char save_xml_folder[1024];
	bool remove_files;
	bool post_xml;
	char post_url[1024];
	bool save_images;
	char save_images_folder[1024];
	bool save_only_verified;
	char root_tag[AUREUS_TAG_SIZE];
	bool person_name;
	char person_name_tag[AUREUS_TAG_SIZE];
	bool person_id;
	char person_id_tag[AUREUS_TAG_SIZE];
	bool processed_frames;
	char processed_frames_tag[AUREUS_TAG_SIZE];
	bool head_id;
	char head_id_tag[AUREUS_TAG_SIZE];
	bool stream_type;
	char stream_type_tag[AUREUS_TAG_SIZE];
	bool stream_connection_info;
	char stream_connection_info_tag[AUREUS_TAG_SIZE];
	bool stream_index;
	char stream_index_tag[AUREUS_TAG_SIZE];
	bool verification_threshold;
	char verification_threshold_tag[AUREUS_TAG_SIZE];
	bool frame_number;
	char frame_number_tag[AUREUS_TAG_SIZE];
	bool utc_time;
	char utc_time_tag[AUREUS_TAG_SIZE];
	bool ranked_results;
	int ranked_results_n;
	char ranked_results_tag[AUREUS_TAG_SIZE];
	bool matched_person_name;
	char matched_person_name_tag[AUREUS_TAG_SIZE];
	bool matched_person_id;
	char matched_person_id_tag[AUREUS_TAG_SIZE];
	bool matched_image_id;
	char matched_image_id_tag[AUREUS_TAG_SIZE];
	bool date_time_stamp;
	char date_time_stamp_tag[AUREUS_TAG_SIZE];
	bool confidence_measure;
	char confidence_measure_tag[AUREUS_TAG_SIZE];
	bool focus_measure;
	char focus_measure_tag[AUREUS_TAG_SIZE];
	bool eye_positions;
	char eye_positions_tag[AUREUS_TAG_SIZE];
	bool face_box;
	char face_box_tag[AUREUS_TAG_SIZE];
	bool match_score;
	char match_score_tag[AUREUS_TAG_SIZE];
	bool matched_status;
	char matched_status_tag[AUREUS_TAG_SIZE];
	bool tracked_image;
	char tracked_image_tag[AUREUS_TAG_SIZE];
	bool matched_person_thumbnail;
	char matched_person_thumbnail_tag[AUREUS_TAG_SIZE];
	bool matched_thumbnail;
	char matched_thumbnail_tag[AUREUS_TAG_SIZE];

	ifstream settingsFile;
	settingsFile.open(settingsPath);
	if (!settingsFile)
	{
		printf("Could not open control settings %s for camera %d\n", settingsPath, cameraindex);
	}

	// Read the settings from the file
	string line;
	while (settingsFile >> line)
	{
		size_t indexOfEquality = line.find("=");
		if (indexOfEquality == string::npos)
		{
			printf("In control settings file %s on line %s there is no =\n", settingsPath, line);
			return;
		}
		string setting = line.substr(0, indexOfEquality);
		string value = line.substr(indexOfEquality + 1);

		if (setting == "save_xml_results")
		{
			value == "true" ? save_xml_results = true : save_xml_results = false;
			printf("save_xml_results: %d ", save_xml_results);
		}
		else if (setting == "save_xml_folder")
		{
			strcpy(save_xml_folder, value.c_str());
			printf("save_xml_folder: %s ", save_xml_folder);
		}
		else if (setting == "remove_files")
		{
			value == "true" ? remove_files = true : remove_files = false;
			printf("remove_files: %d ", remove_files);
		}
		else if (setting == "post_xml")
		{
			value == "true" ? post_xml = true : post_xml = false;
			printf("post_xml: %d ", post_xml);
		}
		else if (setting == "post_url")
		{
			strcpy(post_url, value.c_str());
			printf("post_url: %s ", post_url);
		}
		else if (setting == "save_images")
		{
			value == "true" ? save_images = true : save_images = false;
			printf("save_images: %d ", save_images);
		}
		else if (setting == "save_images_folder")
		{
			strcpy(save_images_folder, value.c_str());
			printf("save_images_folders: %s ", save_images_folder);
		}
		else if (setting == "save_only_verified")
		{
			value == "true" ? save_only_verified = true : save_only_verified = false;
			printf("save_only_verified: %d ", save_only_verified);
		}
		else if (setting == "root_tag")
		{
			strcpy(root_tag, value.c_str());
			printf("root_tag: %s ", root_tag);
		}
		else if (setting == "person_name")
		{
			value == "true" ? person_name = true : person_name = false;
			printf("person_name: %d ", person_name);
		}
		else if (setting == "person_name_tag")
		{
			strcpy(person_name_tag, value.c_str());
			printf("person_name_tag: %s ", person_name_tag);
		}
		else if (setting == "person_id")
		{
			value == "true" ? person_id = true : person_id = false;
			printf("person_id: %d ", person_id);
		}
		else if (setting == "person_id_tag")
		{
			strcpy(person_id_tag, value.c_str());
			printf("person_id_tag: %s ", person_id_tag);
		}
		else if (setting == "processed_frames")
		{
			value == "true" ? processed_frames = true : processed_frames = false;
			printf("processed_frames: %d ", processed_frames);
		}
		else if (setting == "processed_frames_tag")
		{
			strcpy(processed_frames_tag, value.c_str());
			printf("processed_frames_tag: %s ", processed_frames_tag);
		}
		else if (setting == "head_id")
		{
			value == "true" ? head_id = true : head_id = false;
			printf("head_id: %d ", head_id);
		}
		else if (setting == "head_id_tag")
		{
			strcpy(head_id_tag, value.c_str());
			printf("head_id_tag: %s ", head_id_tag);
		}
		else if (setting == "stream_type")
		{
			value == "true" ? stream_type = true : stream_type = false;
			printf("stream_type: %d ", stream_type);
		}
		else if (setting == "stream_type_tag")
		{
			strcpy(stream_type_tag, value.c_str());
			printf("stream_type_tag: %s ", stream_type_tag);
		}
		else if (setting == "stream_connection_info")
		{
			value == "true" ? stream_connection_info = true : stream_connection_info = false;
			printf("stream_connection_info: %d ", stream_connection_info);
		}
		else if (setting == "stream_connection_info_tag")
		{
			strcpy(stream_connection_info_tag, value.c_str());
			printf("stream_connection_info_tag: %s ", stream_connection_info_tag);
		}
		else if (setting == "stream_index")
		{
			value == "true" ? stream_index = true : stream_index = false;
			printf("stream_index: %d ", stream_index);
		}
		else if (setting == "stream_index_tag")
		{
			strcpy(stream_index_tag, value.c_str());
			printf("stream_index_tag: %s ", stream_index_tag);
		}
		else if (setting == "verification_threshold")
		{
			value == "true" ? verification_threshold = true : verification_threshold = false;
			printf("verification_threshold: %d ", verification_threshold);
		}
		else if (setting == "verification_threshold_tag")
		{
			strcpy(verification_threshold_tag, value.c_str());
			printf("verification_threshold_tag: %s ", verification_threshold_tag);
		}
		else if (setting == "frame_number")
		{
			value == "true" ? frame_number = true : frame_number = false;
			printf("frame_number: %d ", frame_number);
		}
		else if (setting == "frame_number_tag")
		{
			strcpy(frame_number_tag, value.c_str());
			printf("frame_number_tag: %s ", frame_number_tag);
		}
		else if (setting == "utc_time")
		{
			value == "true" ? utc_time = true : utc_time = false;
			printf("utc_time: %d ", utc_time);
		}
		else if (setting == "utc_time_tag")
		{
			strcpy(utc_time_tag, value.c_str());
			printf("utc_time_tag: %s ", utc_time_tag);
		}
		else if (setting == "ranked_results")
		{
			value == "true" ? ranked_results = true : ranked_results = false;
			printf("ranked_results: %d ", ranked_results);
		}
		else if (setting == "ranked_results_n")
		{
			ranked_results_n = stoi(value);
			printf("ranked_results_n: %d ", ranked_results_n);
		}
		else if (setting == "ranked_results_tag")
		{
			strcpy(ranked_results_tag, value.c_str());
			printf("ranked_results_tag: %s ", ranked_results_tag);
		}
		else if (setting == "matched_person_name")
		{
			value == "true" ? matched_person_name = true : matched_person_name = false;
			printf("matched_person_name: %d ", matched_person_name);
		}
		else if (setting == "matched_person_name_tag")
		{
			strcpy(matched_person_name_tag, value.c_str());
			printf("matched_person_name_tag: %s ", matched_person_name_tag);
		}
		else if (setting == "matched_person_id")
		{
			value == "true" ? matched_person_id = true : matched_person_id = false;
			printf("matched_person_id: %d ", matched_person_id);
		}
		else if (setting == "matched_person_id_tag")
		{
			strcpy(matched_person_id_tag, value.c_str());
			printf("matched_person_id_tag: %s ", matched_person_id_tag);
		}
		else if (setting == "matched_image_id")
		{
			value == "true" ? matched_image_id = true : matched_image_id = false;
			printf("matched_image_id: %d ", matched_image_id);
		}
		else if (setting == "matched_image_id_tag")
		{
			strcpy(matched_image_id_tag, value.c_str());
			printf("matched_image_id_tag: %s ", matched_image_id_tag);
		}
		else if (setting == "date_time_stamp")
		{
			value == "true" ? date_time_stamp = true : date_time_stamp = false;
			printf("date_time_stamp: %d ", date_time_stamp);
		}
		else if (setting == "date_time_stamp_tag")
		{
			strcpy(date_time_stamp_tag, value.c_str());
			printf("date_time_stamp_tag: %s ", date_time_stamp_tag);
		}
		else if (setting == "confidence_measure")
		{
			value == "true" ? confidence_measure = true : confidence_measure = false;
			printf("confidence_measure: %d ", confidence_measure);
		}
		else if (setting == "confidence_measure_tag")
		{
			strcpy(confidence_measure_tag, value.c_str());
			printf("confidence_measure_tag: %s ", confidence_measure_tag);
		}
		else if (setting == "focus_measure")
		{
			value == "true" ? focus_measure = true : focus_measure = false;
			printf("focus_measure: %d ", focus_measure);
		}
		else if (setting == "focus_measure_tag")
		{
			strcpy(focus_measure_tag, value.c_str());
			printf("focus_measure_tag: %s ", focus_measure_tag);
		}
		else if (setting == "eye_positions")
		{
			value == "true" ? eye_positions = true : eye_positions = false;
			printf("eye_positions: %d ", eye_positions);
		}
		else if (setting == "eye_positions_tag")
		{
			strcpy(eye_positions_tag, value.c_str());
			printf("eye_positions_tag: %s ", eye_positions_tag);
		}
		else if (setting == "face_box")
		{
			value == "true" ? face_box = true : face_box = false;
			printf("face_box: %d ", face_box);
		}
		else if (setting == "face_box_tag")
		{
			strcpy(face_box_tag, value.c_str());
			printf("face_box_tag: %s ", face_box_tag);
		}
		else if (setting == "match_score")
		{
			value == "true" ? match_score = true : match_score = false;
			printf("match_score: %d ", match_score);
		}
		else if (setting == "match_score_tag")
		{
			strcpy(match_score_tag, value.c_str());
			printf("match_score_tag: %s ", match_score_tag);
		}
		else if (setting == "matched_status")
		{
			value == "true" ? matched_status = true : matched_status = false;
			printf("matched_status: %d ", matched_status);
		}
		else if (setting == "matched_status_tag")
		{
			strcpy(matched_status_tag, value.c_str());
			printf("matched_status_tag: %s ", matched_status_tag);
		}
		else if (setting == "tracked_image")
		{
			value == "true" ? tracked_image = true : tracked_image = false;
			printf("tracked_image: %d ", tracked_image);
		}
		else if (setting == "tracked_image_tag")
		{
			strcpy(tracked_image_tag, value.c_str());
			printf("tracked_image_tag: %s ", tracked_image_tag);
		}
		else if (setting == "matched_person_thumbnail")
		{
			value == "true" ? matched_person_thumbnail = true : matched_person_thumbnail = false;
			printf("matched_person_thumbnail: %d ", matched_person_thumbnail);
		}
		else if (setting == "matched_person_thumbnail_tag")
		{
			strcpy(matched_person_thumbnail_tag, value.c_str());
			printf("matched_person_thumbnail_tag: %s ", matched_person_thumbnail_tag);
		}
		else if (setting == "matched_thumbnail")
		{
			value == "true" ? matched_thumbnail = true : matched_thumbnail = false;
			printf("matched_thumbnail: %d ", matched_thumbnail);
		}
		else if (setting == "matched_thumbnail_tag")
		{
			strcpy(matched_thumbnail_tag, value.c_str());
			printf("matched_thumbnail_tag: %s ", matched_thumbnail_tag);
		}
	}

	// Set the settings
	CX_ResultsSettings res;
	res.m_save_xml_results = save_xml_results;
	strcpy(res.m_save_xml_folder, save_xml_folder);
	res.m_remove_files = remove_files;
	res.m_post_xml = post_xml;
	strcpy(res.m_post_url, post_url);
	res.m_save_images = save_images;
	strcpy(res.m_save_images_folder, save_images_folder);
	res.m_save_only_verified = save_only_verified;
	strcpy(res.m_root_tag, root_tag);
	res.m_person_name = person_name;
	strcpy(res.m_person_name_tag, person_name_tag);
	res.m_person_id = person_id;
	strcpy(res.m_person_id_tag, person_id_tag);
	res.m_processed_frames = processed_frames;
	strcpy(res.m_processed_frames_tag, processed_frames_tag);
	res.m_head_id = head_id;
	strcpy(res.m_head_id_tag, head_id_tag);
	res.m_stream_type = stream_type;
	strcpy(res.m_stream_type_tag, stream_type_tag);
	res.m_stream_connection_info = stream_connection_info;
	strcpy(res.m_stream_connection_info_tag, stream_connection_info_tag);
	res.m_stream_index = stream_index;
	strcpy(res.m_stream_index_tag, stream_index_tag);
	res.m_verification_threshold = verification_threshold;
	strcpy(res.m_verification_threshold_tag, verification_threshold_tag);
	res.m_frame_number = frame_number;
	strcpy(res.m_frame_number_tag, frame_number_tag);
	res.m_utc_time = utc_time;
	strcpy(res.m_utc_time_tag, utc_time_tag);
	res.m_ranked_results = ranked_results;
	res.m_ranked_results_n = ranked_results_n;
	strcpy(res.m_ranked_results_tag, ranked_results_tag);
	res.m_matched_person_name = matched_person_name;
	strcpy(res.m_matched_person_name_tag, matched_person_name_tag);
	res.m_matched_person_id = matched_person_id;
	strcpy(res.m_matched_person_id_tag, matched_person_id_tag);
	res.m_matched_image_id = matched_image_id;
	strcpy(res.m_matched_image_id_tag, matched_image_id_tag);
	res.m_date_time_stamp = date_time_stamp;
	strcpy(res.m_date_time_stamp_tag, date_time_stamp_tag);
	res.m_confidence_measure = confidence_measure;
	strcpy(res.m_confidence_measure_tag, confidence_measure_tag);
	res.m_focus_measure = focus_measure;
	strcpy(res.m_focus_measure_tag, focus_measure_tag);
	res.m_eye_positions = eye_positions;
	strcpy(res.m_eye_positions_tag, eye_positions_tag);
	res.m_face_box = face_box;
	strcpy(res.m_face_box_tag, face_box_tag);
	res.m_match_score = match_score;
	strcpy(res.m_match_score_tag, match_score_tag);
	res.m_matched_status = matched_status;
	strcpy(res.m_matched_status_tag, matched_status_tag);
	res.m_tracked_image = tracked_image;
	strcpy(res.m_tracked_image_tag, tracked_image_tag);
	res.m_matched_person_thumbnail = matched_person_thumbnail;
	strcpy(res.m_matched_person_thumbnail_tag, matched_person_thumbnail_tag);
	res.m_matched_thumbnail = matched_thumbnail;
	strcpy(res.m_matched_thumbnail_tag, matched_thumbnail_tag);

	// Set results parameters
	if (0 == CX_SetResultsParameters(p_video, res, msg))
	{
		printf("Failed to set results params for camera %d:\n%s\n", cameraindex, msg);
	}
}

void SetParametersAndSettingsFromFiles(CX_VIDEO p_video, int cameraindex)
{
	char msg[1024];

	char cameraindex_str[1024];
	_itoa(cameraindex, cameraindex_str, 10);

	char controlSettings[1024];
	strcat(controlSettings, "C:/Allevate/Face-Searcher/Camera");
	strcat(controlSettings, cameraindex_str);
	strcat(controlSettings, "ControlSettings.txt");

	char resultsSettings[1024];
	strcat(resultsSettings, "C:/Allevate/Face-Searcher/Camera");
	strcat(resultsSettings, cameraindex_str);
	strcat(resultsSettings, "ResultsSettings.txt");

	SetDetectionParametersFromFile(p_video, controlSettings, cameraindex);
	SetResultsSettingsFromFile(p_video, resultsSettings, cameraindex);

	if (!CX_SetForceEveryFrame(p_video, 1, msg)) {
		printf("%s\n", msg);
	}
}

void WriteContentToFile(const char* content, const char* filepath)
{
	ofstream contentFile;
	contentFile.open(filepath);
	contentFile << content;
	contentFile.close();
}