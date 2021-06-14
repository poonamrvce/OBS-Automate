#include "ObsUtils.hpp"

int loadModule(const char* binPath, const char* dataPath) {
	obs_module_t *module;

	int code = obs_open_module(&module, binPath, dataPath);
	if (code != MODULE_SUCCESS) {
		LOG(ERROR)<<"Failed to load module file "<<binPath<<" ,CODE: "<<code<<std::endl;
		return -1;
	}

	if(obs_init_module(module) != true) {
		LOG(ERROR)<<"obs_init_module failed"<<std::endl;
		return -1;
	}

	return 0;
}



int ObsInit(int argc, char *argv[]) {
	///////////////
	// OBS init  //
	///////////////

	QApplication app(argc, argv);

	if(!obs_startup("en-US", nullptr, nullptr) || !obs_initialized()) {
		return -1;
	}

    struct obs_video_info ovi;
	struct obs_audio_info oai;
	obs_output_t*   output;
	obs_encoder_t*  enc_a;
	obs_encoder_t*  enc_v;
	obs_data_t*     ffmpeg_mux_settings;
	obs_data_t*     enc_a_settings;
	obs_data_t*     enc_v_settings;

	memset(&ovi, 0, sizeof(ovi));
	memset(&oai, 0, sizeof(oai));

	ovi.adapter         = 0;
	ovi.graphics_module = "libobs-opengl.so.0.0";
	ovi.output_format   = VIDEO_FORMAT_NV12; // TODO to settings with VIDEO_FORMAT_I420
	ovi.fps_num         = 25;
	ovi.fps_den         = 1;
	ovi.base_width      = 1920;
	ovi.base_height     = 1080;
	ovi.output_width    = 1920;
	ovi.output_height   = 1080;
	ovi.gpu_conversion  = true;

	if(obs_reset_video(&ovi) != OBS_VIDEO_SUCCESS) {
		LOG(ERROR)<<"obs_reset_video failed"<<std::endl;
		return -1;
	}

	oai.samples_per_sec  = 48000;
	oai.speakers         = SPEAKERS_STEREO; // TODO to settings

	if (obs_reset_audio(&oai) != true) {
		LOG(ERROR)<<"obs_reset_audio failed"<<std::endl;
		return -1;
	}

	// Load modules
	// For color_source
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "image-source.so", LIBOBS_PLUGINS_DATA_PATH "image-source")) {
        LOG(ERROR)<<"failed to load lib image-source.so"<<std::endl;
		return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-ffmpeg.so", LIBOBS_PLUGINS_DATA_PATH "obs-ffmpeg")) {
        LOG(ERROR)<<"failed to load lib obs-ffmpeg.so"<<std::endl;
        return -1;
	}
	
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-transitions.so", LIBOBS_PLUGINS_DATA_PATH "obs-transitions")) {
        LOG(ERROR)<<"failed to load lib obs-transitions.so"<<std::endl;
        return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "rtmp-services.so", LIBOBS_PLUGINS_DATA_PATH "rtmp-services")) {
        LOG(ERROR)<<"failed to load lib rtmp-services.so"<<std::endl;
        return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-x264.so", LIBOBS_PLUGINS_DATA_PATH "obs-x264")) {
        LOG(ERROR)<<"failed to load lib obs-x264.so"<<std::endl;
        return -1;
	}
	
	// For fdk-aac
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-libfdk.so", LIBOBS_PLUGINS_DATA_PATH "obs-libfdk")) {
        LOG(ERROR)<<"failed to load lib obs-libfdk.so"<<std::endl;
        return -1;
	}
	// For rtmp-output
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-outputs.so", LIBOBS_PLUGINS_DATA_PATH "obs-outputs")) {
        LOG(ERROR)<<"failed to load lib obs-outputs.so"<<std::endl;
        return -1;
	}

	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-browser.so", LIBOBS_PLUGINS_DATA_PATH "obs-browser")) {
        LOG(ERROR)<<"failed to load lib obs-browser.so"<<std::endl;
        return -1;
	}

	// if(0 != loadModule(LIBOBS_PLUGINS_PATH "text-freetype2.so", LIBOBS_PLUGINS_DATA_PATH "text-freetype2")) {
    //     LOG(ERROR)<<"failed to load lib text-freetype2.so"<<std::endl;
    //     return -1;
	// }

	obs_post_load_modules();

	LOG(INFO)<<"All is well"<<std::endl;

	return 0;
}