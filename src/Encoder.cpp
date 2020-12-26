
#include "Encoder.hpp"
#include "Trace.hpp"

int Encoder::CreateVideoEncoder(VideoEncParams *vidParams) {
    obs_data_t*     enc_v_settings;
    // Video encoder
	std::string encoder = vidParams->encoder_type;
	enc_v_ = obs_video_encoder_create(encoder.c_str(), 
                    "videoEncoder", NULL, nullptr);
	if (!enc_v_) {
        trace_error("Couldn't create enc_v");
        return -1;
	}

	enc_v_settings = obs_encoder_get_settings(enc_v_);
	if (!enc_v_settings) {
        trace_error("Failed to create enc_v_settings");
        return -1;
	}

	obs_data_set_int(	enc_v_settings, "bitrate",	vidParams->bitrate);
	obs_data_set_int(	enc_v_settings, "keyint_sec",	vidParams->keyint_sec);
	obs_data_set_string(enc_v_settings, "rate_control",	
                                vidParams->rate_control.c_str());
	
    obs_data_set_int(	enc_v_settings, "width",	vidParams->width);
    obs_data_set_int(	enc_v_settings, "height",	vidParams->height);
    obs_data_set_int(	enc_v_settings, "fps_num",	vidParams->fps_num);
    obs_data_set_int(	enc_v_settings, "fps_den",	vidParams->fps_den);
    // TODO sw encoder settings
    // obs_data_set_int(enc_v_settings, "buffer_size",		settings->videoBitrateKbps);
    obs_data_set_string(enc_v_settings, "preset",	vidParams->preset.c_str());
    obs_data_set_string(enc_v_settings, "profile",	vidParams->profile.c_str());
    obs_data_set_string(enc_v_settings, "tune",		vidParams->tune.c_str());
    obs_data_set_string(enc_v_settings, "x264opts",	vidParams->x264opts.c_str());
	
	obs_encoder_update(enc_v_, enc_v_settings);
	obs_data_release(enc_v_settings);
    return 0;
}

int Encoder::CreateAudioEncoder(AudioEncParams *audParams) {
    obs_data_t*     enc_a_settings;
    // Audio encoder
	enc_a_ = obs_audio_encoder_create(audParams->encoder_type.c_str(), 
                    "audioEncoder", NULL, 0, nullptr);
	if (!enc_a_) {
        trace_error("Couldn't create enc_a");
        return -1;
	}

	enc_a_settings = obs_encoder_get_settings(enc_a_);
	if (!enc_a_settings) {
        trace_error("Failed to create enc_a_settings");
        return -1;
	}

	obs_data_set_int(	enc_a_settings, "bitrate",	audParams->bitrate);
	obs_data_set_bool(	enc_a_settings, "afterburner",	true);
	obs_encoder_update(enc_a_, enc_a_settings);
	obs_data_release(enc_a_settings);
    return 0;
}