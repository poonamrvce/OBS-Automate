
#include "Source.hpp"

obs_source_t* Source::localFileSourceCreate(string name, string url, bool looping) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(INFO)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	LOG(DEBUG)<<"create ffmpeg src"<<endl;

	obs_data_set_string(obs_data, "local_file", url.c_str());
	obs_data_set_bool(obs_data, "is_local_file", true);
	obs_data_set_bool(obs_data, "looping", looping);
	obs_data_set_bool(obs_data, "hw_decode", false);

	obs_source_ = obs_source_create("ffmpeg_source", name.c_str(), obs_data, nullptr);

	printf("WIDTH %d HEIGHT %d\n" ,(obs_source_get_width(obs_source_)), (obs_source_get_height(obs_source_)));

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::browserSourceCreate(string name, string url) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}
	bool is_local=url.substr(0,4)!="http";
	LOG(DEBUG)<<"create browser src"<<endl;

	if(is_local)
		obs_data_set_string(obs_data, "local_file", url.c_str());
	else
		obs_data_set_string(obs_data, "url", url.c_str());
	obs_data_set_bool(obs_data, "is_local_file", is_local);
	

	// obs_property_t *url_property = obs_properties_get(props, "url");
	// obs_property_set_visible(url_property, !is_local);

	// obs_property_t *local_file_property = obs_properties_get(props, "local_file");
	// obs_property_set_visible(local_file_property, is_local);

	obs_data_set_int(obs_data, "width", 800);
	obs_data_set_int(obs_data, "height", 600);
	obs_data_set_int(obs_data, "fps", 25);
	obs_data_set_bool(obs_data, "hw_decode", false);
	obs_data_set_bool(obs_data,"is_showing",true);

	obs_source_ = obs_source_create("browser_source", name.c_str(), obs_data, nullptr);

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::colorSourceCreate(string name, int color) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	obs_data_set_int(obs_data, "width", 800);
	obs_data_set_int(obs_data, "height", 600);
	obs_data_set_int(obs_data, "color", color);

	obs_source_ = obs_source_create("color_source", name.c_str(), obs_data, nullptr);

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::imageSourceCreate(string name, string localFile) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	obs_data_set_string(obs_data, "file", localFile.c_str());
	obs_data_set_bool(obs_data,"unload",false);

	obs_source_ = obs_source_create("image_source", name.c_str(), obs_data, nullptr);

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}




Source::Source(SourceParams  *params) {
    switch(params->sourceType) {

        case SourceType::LocalFile:
        localFileSourceCreate(params->name, params->localFile, true);
        break;

		case SourceType::Browser:
		browserSourceCreate(params->name,params->url);
		break;

		case SourceType::Color:
		colorSourceCreate(params->name,params->color);
        break;

		case SourceType::Image:
		imageSourceCreate(params->name,params->localFile);
		break;

        default:
        LOG(ERROR)<<"Unknown SourceType received"<<endl;
    } 
}

unordered_map<string,pair<SourceType,string>> Source::all_sources={
	{"im",{SourceType::Image,"Image"}},
	{"br",{SourceType::Browser,"Browser"}},
	{"co",{SourceType::Color,"Color"}},
	{"vi",{SourceType::LocalFile,"Video"}},
	{"rt",{SourceType::RTMP,"RTMP"}},
};

string Source::allSourcePattern(){
	string s="(";
	for(auto i=Source::all_sources.begin();i!=Source::all_sources.end();i++){
		s.append((*i).first);
		s.append("|");
	}
	s.pop_back();
	s.append(")");
	return s;
}