
#include "Source.hpp"

obs_source_t* Source::get_local_file_source(string name,  const char* localFile, bool looping) {

    LOG(DEBUG)<<"video file:"<<localFile<<endl;
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(INFO)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	LOG(DEBUG)<<"create ffmpeg src"<<endl;

	obs_data_set_string(obs_data, "local_file", localFile);
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

obs_source_t* Source::get_browser_source(string name,  const char* url) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}
	bool is_local=strncmp(url,"http",4)!=0;
	LOG(DEBUG)<<"create browser src"<<endl;

	if(is_local)
		obs_data_set_string(obs_data, "local_file", url);
	else
		obs_data_set_string(obs_data, "url", url);
	obs_data_set_bool(obs_data, "is_local_file", is_local);
	

	// obs_property_t *url_property = obs_properties_get(props, "url");
	// obs_property_set_visible(url_property, !is_local);

	// obs_property_t *local_file_property = obs_properties_get(props, "local_file");
	// obs_property_set_visible(local_file_property, is_local);

	// obs_data_set_int(obs_data, "width", 800);
	// obs_data_set_int(obs_data, "height", 600);
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

obs_source_t* Source::get_color_source(string name, int color) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	// obs_data_set_int(obs_data, "width", 800);
	// obs_data_set_int(obs_data, "height", 600);
	obs_data_set_int(obs_data, "color", color);

	obs_source_ = obs_source_create("color_source", name.c_str(), obs_data, nullptr);

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::get_image_source(string name, const char* localFile) {
	obs_data_t	*obs_data;
    obs_data = obs_data_create();
	if (!obs_data) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	obs_data_set_string(obs_data, "file", localFile);
	obs_data_set_bool(obs_data,"unload",false);

	obs_source_ = obs_source_create("image_source", name.c_str(), obs_data, nullptr);

	obs_data_release(obs_data);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}
    return obs_source_;
}

obs_source_t* Source::get_text_source(string name,  const char* text) {

	obs_data_t *settings = obs_data_create();
	obs_data_t *font = obs_data_create();

	if (!settings || !font) {
		LOG(ERROR)<<"Failed to create obs_data"<<endl;
		return NULL;
	}

	obs_data_set_string(font, "face", "Monospace");
	obs_data_set_int(font, "flags", 1); // Bold text
	obs_data_set_int(font, "size", 30 );

	obs_data_set_obj(settings, "font", font);
	obs_data_set_string(settings, "text", text);
	obs_data_set_bool(settings, "outline", false);

	if(0 != loadModule(LIBOBS_PLUGINS_PATH "text-freetype2.so", LIBOBS_PLUGINS_DATA_PATH "text-freetype2")) {
        LOG(ERROR)<<"failed to load lib text-freetype2.so"<<std::endl;
	}

	obs_source_ = obs_source_create("text_ft2_source", name.c_str(), settings, nullptr);

	obs_data_release(font);
	obs_data_release(settings);
	if (!obs_source_) {
		LOG(ERROR)<<"Failed to create obs_source"<<endl;
		return NULL;
	}else
		LOG(DEBUG)<<"created text source"<<endl;
    return obs_source_;
}




Source::Source(SourceParams  *params) {
	_name=params->name;
    switch(params->sourceType) {

        case SourceType::LocalFile:
        get_local_file_source(params->name, params->localFile, true);
        break;

		case SourceType::Browser:
		get_browser_source(params->name,params->url);
		break;

		case SourceType::Color:
		get_color_source(params->name,params->color);
        break;

		case SourceType::Image:
		get_image_source(params->name,params->localFile);
		break;

		case SourceType::Text:
		get_text_source(params->name,params->text);
		break;

        default:
        LOG(ERROR)<<"Unknown SourceType received"<<endl;
    } 
}