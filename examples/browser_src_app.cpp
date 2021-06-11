#include <QApplication>
#include <QPushButton>
#include <string>
#include <unistd.h>
#include "obs/obs.h"
#include "ObsUtils.hpp"
#include "Encoder.hpp"
#include "Show.hpp"
#include "Scene.hpp"
#include "Source.hpp"

#define app_output_file "/data/sample/app3.mkv"

using namespace std;

int main(int argc, char *argv[]) {
    // if(argc < 3) {
    //     cout << "Usage: ./scene_switch_app <file1> <file2>";
    //     exit(1);
    // }
    string url="https://www.google.com/";
    if(argc>1)
        url=string(argv[1]);
    string show_name="browserSource";
    // obs_output_t*   output;
	// obs_data_t*     ffmpeg_mux_settings;
    
	QApplication app(argc, argv);
	ObsInit();

    // Create a file source
    SourceParams sourceParams = {
        .name = "Browser",
        .sourceType = SourceType::Browser,
        {
            .url = (char *)url.c_str()
        }
    };
    SceneParams  sceneParams = {
        .name = "scene"
    };
    Source *source = new Source(&sourceParams); 
    Scene *scene = new Scene(&sceneParams);

    Show *show = new Show(show_name);
    struct vec2 bounds = {1920, 1080};
    scene->add_source(source,  &bounds);
    show->AddScene(scene);
    show->SetActiveScene("scene");


    ////////////
    //  RTMP  //
    ////////////
    

	// output and service	
	obs_service_t* service = obs_service_create("rtmp_common", "rtmp service", nullptr, nullptr);
	if (!service) {
		LOG(ERROR)<<"obs_service_create"<<std::endl;
        exit(1);
	}

	obs_data_t* rtmp_settings = obs_data_create();
	if (!rtmp_settings) {
		LOG(ERROR)<<"obs_data_create"<<std::endl;
        exit(1);
	}

	obs_data_set_string(rtmp_settings, "server", "rtmp://localhost:1935/live");
	obs_data_set_string(rtmp_settings, "key", "app3");
	obs_service_update(service, rtmp_settings);
	obs_data_release(rtmp_settings);


	obs_output_t* output = obs_output_create("rtmp_output", "RTMP output", NULL, nullptr);
	if (!output) {
		LOG(ERROR)<<"obs_output_create"<<std::endl;
        exit(1);
	}

    Encoder *encoder = new Encoder();
    VideoEncParams videoParams = {
        .encoder_type = "obs_x264",
        .bitrate = 4000,
        .keyint_sec = 1,
        .rate_control = "CBR",
        .width = 1920,
        .height = 1080,
        .fps_num = 25,
        .fps_den = 1,
        .preset = "ultrafast",
        .profile = "high",
        .tune = "zerolatency",
        .x264opts = ""
    };
    encoder->CreateVideoEncoder(&videoParams);

    AudioEncParams audioParams = {
        .encoder_type = "libfdk_aac",
        .bitrate = 192
    };
    encoder->CreateAudioEncoder(&audioParams);


    obs_set_output_source(0, show->GetShowSource());
	obs_encoder_set_video(encoder->GetVideoEncoder(), obs_get_video());
	obs_encoder_set_audio(encoder->GetAudioEncoder(), obs_get_audio());
	obs_output_set_video_encoder(output, encoder->GetVideoEncoder());
	obs_output_set_audio_encoder(output, encoder->GetAudioEncoder(), 0);
	obs_output_set_service(output, service);

	if(obs_output_start(output) != true) {
		LOG(ERROR)<<"obs_output_start failed"<<std::endl;
	}

    // if(!fork()){
    //     system("ffplay rtmp://localhost:1935/live/app3");
    //     exit(0);
    // }

    while(true);

}