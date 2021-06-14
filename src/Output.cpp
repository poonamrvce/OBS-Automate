#include "Output.hpp"


RTMPInfo Output::default_rtmp={
    .stream="rtmp://localhost:1935/live/",
    .key="app0"
};

LocalFileInfo Output::default_localFile={
    .url="/vol/sample/output0.mkv"
};

OutputStatus Output::set_show(Show *show){
    obs_set_output_source(0, show->GetShowSource());
}

OutputStatus Output::start_output(bool display){

	if(obs_output_start(output) != true) {
		LOG(ERROR)<<"obs_output_start failed"<<std::endl;
        return OutputStatus::ERROR;
	}    
    LOG(INFO)<<"streaming to "<<rtmp->stream +"/" +rtmp->key<<std::endl;
    if(display && !fork()){
        string command="ffplay ";

        switch(outputType){
            case OutputType::RTMP:
                command+=rtmp->stream + "/" +rtmp->key;break;
            case OutputType::LocalFile:
                command+=localFile->url;break;
        }
        system(command.c_str());
        exit(0);
    }
    return OutputStatus::OK;
}

OutputStatus Output::create_av_encoder(){

    //============= Create encoders ========================
    encoder = new Encoder();
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
}

OutputStatus Output::create_ffmpeg_output(){
    obs_data_t* ffmpeg_mux_settings = obs_data_create();
	if (!ffmpeg_mux_settings) {
        LOG(ERROR)<<"Couldn't create settings"<<std::endl;
        return OutputStatus::ERROR;
	}

	obs_data_set_string(ffmpeg_mux_settings, "path", localFile->url.c_str() );
	

    output = obs_output_create("ffmpeg_muxer", "Ffmpeg Muxer", ffmpeg_mux_settings, nullptr);
	if (!output) {
        LOG(ERROR)<<"Couldn't create output"<<std::endl;
        return OutputStatus::ERROR;
	}

	obs_data_release(ffmpeg_mux_settings);

}

OutputStatus Output::create_rtmp_output(){

        obs_service_t* service = obs_service_create("rtmp_common", "rtmp service", nullptr, nullptr);
        if (!service) {
            LOG(ERROR)<<"Couldn't create service"<<std::endl;
            return OutputStatus::ERROR;
        }else{
            LOG(INFO)<<"Created service."<<std::endl;
        }

        obs_data_t* rtmp_settings = obs_data_create();
        if (!rtmp_settings) {
            LOG(ERROR)<<"Couldn't create settings"<<std::endl;
            return OutputStatus::ERROR;
        }else{
            LOG(INFO)<<"Created service settings."<<std::endl;
        }

        obs_data_set_string(rtmp_settings, "server", rtmp->stream.c_str());
        obs_data_set_string(rtmp_settings, "key", rtmp->key.c_str());
        obs_service_update(service, rtmp_settings);
        obs_data_release(rtmp_settings);

        output = obs_output_create("rtmp_output", "RTMP output", nullptr, nullptr);
        if (!output) {
            LOG(ERROR)<<"Couldn't create output"<<std::endl;
            return OutputStatus::ERROR;
        }else{
            LOG(INFO)<<"Created output."<<std::endl;
        }

        switch(create_av_encoder()){
            case OutputStatus::ERROR:
                LOG(ERROR)<<"error creating Encoder."<<std::endl;
                break;
            case OutputStatus::OK:
                LOG(INFO)<<"created encoder."<<std::endl;
                break;
        }

        //obs_set_output_source(1, scene2->get_obs_scene_source());
        obs_encoder_set_video(encoder->GetVideoEncoder(), obs_get_video());
        obs_encoder_set_audio(encoder->GetAudioEncoder(), obs_get_audio());
        obs_output_set_video_encoder(output, encoder->GetVideoEncoder());
        obs_output_set_audio_encoder(output, encoder->GetAudioEncoder(), 0);
        
        obs_output_set_service(output, service);
        return OutputStatus::OK;
}

Output::Output(OutputType _outputType,void *infoPtr): outputType(_outputType){

    OutputStatus status;
    switch (outputType)
    {
        
        case OutputType::RTMP:
            if(infoPtr)
                rtmp=(RTMPInfo*)infoPtr;
            else
                rtmp=&default_rtmp;
            LOG(INFO)<<"Trying to create RTMP output..."<<std::endl;
            status=create_rtmp_output();
            break;

        case OutputType::LocalFile:
            if(infoPtr)
                localFile=(LocalFileInfo*)infoPtr;
            else
                localFile=&default_localFile;
            LOG(INFO)<<"Trying to create LocalFile output..."<<std::endl;
            status=create_ffmpeg_output();
            break;
        
        default:
            break;
    }

    // switch(status){
    //     case OutputStatus::ERROR:
    //         LOG(ERROR)<<"error creating output type.");
    //         break;
    //     case OutputStatus::OK:
    //         LOG(INFO)<<"careted output type.");
    //         break;
    // }

}