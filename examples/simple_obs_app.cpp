#include <QApplication>
#include <QPushButton>
#include <string>
#include <unistd.h>
#include "Trace.hpp"
#include "obs/obs.h"

int gTraceLevel = TRACE_LEVEL_TRACE;
int gTraceFormat = TRACE_FORMAT_TEXT;

#define LIBOBS_PLUGINS_PATH "/usr/lib/obs-plugins/"
#define LIBOBS_PLUGINS_DATA_PATH "/usr/share/obs/obs-plugins/"

obs_source_t*  setSceneAndSource() {
	obs_source_t* obs_transition;
	obs_source_t *obs_source;
	obs_scene_t *obs_scene;
	obs_data_t	*obs_data;
	

	/*std::string transition_name = std::string("transition_main");
	obs_transition = obs_source_create("cut_transition", transition_name.c_str(), NULL, nullptr);
	if (!obs_transition) {
		trace_error("Error while creating obs_transition %s", "cut_transition");
		return NULL;
	}*/

	/* Create a scene and attach a source to it */
	/*obs_scene = obs_scene_create("obs_example_main");
	if (!obs_scene) {
		trace_error("Error while creating obs_scene");
		return NULL;
	}*/

	// Create a Source
	obs_data = obs_data_create();
	if (!obs_data) {
		trace_error("Failed to create obs_data",);
		return NULL;
	}

	trace_debug("create ffmpeg src");

	obs_data_set_string(obs_data, "local_file", "/data/sb_media/ChevyCruze_30s_ts.mkv");
	obs_data_set_bool(obs_data, "is_local_file", true);
	obs_data_set_bool(obs_data, "looping", true);
	obs_data_set_bool(obs_data, "hw_decode", false);

	std::string source_name = std::string("obs_src_ffmpeg_file");
	obs_source = obs_source_create("ffmpeg_source", source_name.c_str(), obs_data, nullptr);

	printf("WIDTH %d HEIGHT %d\n" ,(obs_source_get_width(obs_source)), (obs_source_get_height(obs_source)));

	obs_data_release(obs_data);

	if (!obs_source) {
		trace_error("Failed to create obs_source");
		return NULL;
	}

	trace_debug("create scene");
	// Add source to Scene
	/*obs_sceneitem_t* obs_scene_item = obs_scene_add(obs_scene, obs_source);
	if (!obs_scene_item) {
		trace_error("Error while adding scene item");
		return NULL;
	}*/

	// Scale source to output size by setting bounds
	/*struct vec2 bounds;
	bounds.x = 1920;
	bounds.y = 1080;
	uint32_t align = OBS_ALIGN_TOP + OBS_ALIGN_LEFT;
	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_bounds(obs_scene_item, &bounds);
	obs_sceneitem_set_bounds_alignment(obs_scene_item, align);*/

	trace_debug("obs_transition_set");

	//obs_transition_set(obs_transition, obs_scene_get_source(obs_scene));
	//return obs_transition;

	//return obs_scene_get_source(obs_scene);
	return obs_source;
}


int loadModule(const char* binPath, const char* dataPath) {
	obs_module_t *module;

	int code = obs_open_module(&module, binPath, dataPath);
	if (code != MODULE_SUCCESS) {
		trace_error("Failed to load module file",field_c(binPath), field(code));
		return -1;
	}

	if(obs_init_module(module) != true) {
		trace_error("obs_init_module failed");
		return -1;
	}

	return 0;
}

int studioInit() {
	///////////////
	// OBS init  //
	///////////////
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

	trace_debug("", field_s(ovi.graphics_module));

	if(obs_reset_video(&ovi) != OBS_VIDEO_SUCCESS) {
		trace_error("obs_reset_video failed");
		return -1;
	}

	oai.samples_per_sec  = 48000;
	oai.speakers         = SPEAKERS_STEREO; // TODO to settings

	if (obs_reset_audio(&oai) != true) {
		trace_error("obs_reset_audio failed");
		return -1;
	}

	// Load modules
	// For color_source
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "image-source.so", LIBOBS_PLUGINS_DATA_PATH "image-source")) {
        trace_error("failed to load lib image-source.so");
		return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-ffmpeg.so", LIBOBS_PLUGINS_DATA_PATH "obs-ffmpeg")) {
        trace_error("failed to load lib obs-ffmpeg.so");
        return -1;
	}
	
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-transitions.so", LIBOBS_PLUGINS_DATA_PATH "obs-transitions")) {
        trace_error("failed to load lib obs-transitions.so");
        return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "rtmp-services.so", LIBOBS_PLUGINS_DATA_PATH "rtmp-services")) {
        trace_error("failed to load lib rtmp-services.so")
        return -1;
	}
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-x264.so", LIBOBS_PLUGINS_DATA_PATH "obs-x264")) {
        trace_error("failed to load lib obs-x264.so")
        return -1;
	}
	
	// For fdk-aac
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-libfdk.so", LIBOBS_PLUGINS_DATA_PATH "obs-libfdk")) {
        trace_error("failed to load lib obs-libfdk.so");
        return -1;
	}
	// For rtmp-output
	if(0 != loadModule(LIBOBS_PLUGINS_PATH "obs-outputs.so", LIBOBS_PLUGINS_DATA_PATH "obs-outputs")) {
        trace_error("failed to load lib obs-outputs.so");
        return -1;
	}

	obs_post_load_modules();


	// output and service	
	/*service = obs_service_create("rtmp_common", "rtmp service", nullptr, nullptr);
	if (!service) {
		return Status(grpc::INTERNAL, "Couldn't create service");
	}

	rtmp_settings = obs_data_create();
	if (!rtmp_settings) {
		return Status(grpc::INTERNAL, "Couldn't create rtmp settings");
	}
	obs_data_release(rtmp_settings);

	obs_data_set_string(rtmp_settings, "server", settings->server.c_str());
	obs_data_set_string(rtmp_settings, "key", settings->key.c_str());
	obs_service_update(service, rtmp_settings);
	obs_data_release(rtmp_settings);

	output = obs_output_create("rtmp_output", "RTMP output", NULL, nullptr);
	if (!output) {
		return Status(grpc::INTERNAL, "Couldn't create output");
	}*/

	ffmpeg_mux_settings = obs_data_create();
	if (!ffmpeg_mux_settings) {
		return -1;
	}

	obs_data_set_string(ffmpeg_mux_settings, "path", "/root/out.mkv");
	

    output = obs_output_create("ffmpeg_muxer", "Ffmpeg Muxer", ffmpeg_mux_settings, nullptr);
	if (!output) {
        trace_error("Couldn't create output")
        return -1;
	}

	obs_data_release(ffmpeg_mux_settings);

	// Audio encoder
	enc_a = obs_audio_encoder_create("libfdk_aac", "aac enc", NULL, 0, nullptr);
	if (!enc_a) {
        trace_error("Couldn't create enc_a");
        return -1;
	}

	enc_a_settings = obs_encoder_get_settings(enc_a);
	if (!enc_a_settings) {
        trace_error("Failed to create enc_a_settings");
        return -1;
	}

	obs_data_set_int(	enc_a_settings, "bitrate",	192);
	obs_data_set_bool(	enc_a_settings, "afterburner",	true);
	obs_encoder_update(enc_a, enc_a_settings);
	obs_data_release(enc_a_settings);

	// Video encoder
	std::string encoder = "obs_x264";
	enc_v = obs_video_encoder_create(encoder.c_str(), "h264 enc", NULL, nullptr);
	if (!enc_v) {
        trace_error("Couldn't create enc_v");
        return -1;
	}

	enc_v_settings = obs_encoder_get_settings(enc_v);
	if (!enc_v_settings) {
        trace_error("Failed to create enc_v_settings");
        return -1;
	}

	obs_data_set_int(	enc_v_settings, "bitrate",		4000);
	obs_data_set_int(	enc_v_settings, "keyint_sec",	1);
	obs_data_set_string(enc_v_settings, "rate_control",	"CBR");
	
    obs_data_set_int(	enc_v_settings, "width",		1920);
    obs_data_set_int(	enc_v_settings, "height",		1080);
    obs_data_set_int(	enc_v_settings, "fps_num",		25);
    obs_data_set_int(	enc_v_settings, "fps_den",		1);
    // TODO sw encoder settings
    // obs_data_set_int(enc_v_settings, "buffer_size",		settings->videoBitrateKbps);
    obs_data_set_string(enc_v_settings, "preset",		"ultrafast");
    obs_data_set_string(enc_v_settings, "profile",		"main");
    obs_data_set_string(enc_v_settings, "tune",			"zerolatency");
    obs_data_set_string(enc_v_settings, "x264opts",		"");
    // obs_data_set_bool(enc_v_settings, "use_bufsize",		false);
    // obs_data_set_int(enc_v_settings, "crf",		0);
    // #ifdef ENABLE_VFR
    // obs_data_set_bool(enc_v_settings, "vfr",		true);
    // #endif
	
	obs_encoder_update(enc_v, enc_v_settings);
	obs_data_release(enc_v_settings);

	trace_info("All is well");

#if 1
	///////////////
	// Show init //
	///////////////
	obs_source_t* out_scene = setSceneAndSource();
	

	obs_set_output_source(0, out_scene);
	obs_encoder_set_video(enc_v, obs_get_video());
	obs_encoder_set_audio(enc_a, obs_get_audio());
	obs_output_set_video_encoder(output, enc_v);
	obs_output_set_audio_encoder(output, enc_a, 0);
	
    //obs_output_set_service(output, service);

	if(obs_output_start(output) != true) {
		trace_error("obs_output_start failed");
	}

	
#endif //0
	return 0;
}


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	studioInit();

	// SLeep for 1000 seconds
	for(int i = 0; i < 100; i++) {
		usleep(10000000);
	}

}