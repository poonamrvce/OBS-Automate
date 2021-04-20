
#include "Source.hpp"
#include "Trace.hpp"


obs_source_t* Source::localFileSourceCreate(std::string name, std::string url, bool looping) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		trace_error("Failed to create obs_data",);
		return NULL;
	}

	trace_debug("create ffmpeg src");

	obs_data_set_string(obs_data, "local_file", url.c_str());
	obs_data_set_bool(obs_data, "is_local_file", true);
	obs_data_set_bool(obs_data, "looping", looping);
	obs_data_set_bool(obs_data, "hw_decode", false);

	obs_source_ = obs_source_create("ffmpeg_source", name.c_str(), obs_data, nullptr);

	printf("WIDTH %d HEIGHT %d\n" ,(obs_source_get_width(obs_source_)), (obs_source_get_height(obs_source_)));

	obs_data_release(obs_data);
	if (!obs_source_) {
		trace_error("Failed to create obs_source");
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::browserUrlSourceCreate(std::string name, std::string url){
	obs_data_t *obs_data;
	obs_data = obs_data_create();
	if(!obs_data){
		trace_error("Failed to create obs_data",);
		return NULL;
	}

	obs_data_set_string(obs_data, "url", url.c_str());
	obs_data_set_bool(obs_data, "is_local_file", false);
	obs_data_set_int(obs_data, "width", 640);
	obs_data_set_int(obs_data, "height", 480);
	obs_data_set_int(obs_data, "fps", 25);

	obs_source_ = obs_source_create("browser_source", name.c_str(), obs_data, nullptr);

	printf("WIDTH %d HEIGHT %d %p\n" ,
		(obs_source_get_width(obs_source_)), (obs_source_get_height(obs_source_)), obs_source_);

	obs_data_release(obs_data);
	if (!obs_source_) {
		trace_error("Failed to create obs_source");
		return NULL;
	}
    return obs_source_;

}


Source::Source(SourceParams  *params) {
    switch(params->sourceType) {
        case LocalFile:
        localFileSourceCreate(params->name, params->url, true);
        break;

		case BrowserUrl:
		browserUrlSourceCreate(params->name, params->url);
		break;

        default:
        trace_error("Unknown SourceType received");
    } 
}