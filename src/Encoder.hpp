#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <string>
#include "obs/obs.h"

struct VideoEncParams {
    std::string   encoder_type; /**< Video code e.g. obs_x264 */
    int           bitrate; /**< Bitrake in kbps */
    int           keyint_sec;
    std::string   rate_control;
    int           width;
    int           height;
    int           fps_num;
    int           fps_den;
    std::string   preset;
    std::string   profile;
    std::string   tune;
    std::string   x264opts;
};

struct AudioEncParams {
    std::string    encoder_type; /**< Audio codec e.g. libfdk_aac */
    int            bitrate; /**< Audio bitrate in kbps e.g. 192 */
};

class Encoder {
    public:
    obs_encoder_t* GetVideoEncoder() { return enc_v_; }
    obs_encoder_t* GetAudioEncoder() { return enc_a_; }
    int CreateVideoEncoder(VideoEncParams *vidParams);
    int CreateAudioEncoder(AudioEncParams *audParams);
    private:
    obs_encoder_t  *enc_v_;
    obs_encoder_t *enc_a_;
};

#endif //__ENCODER_HPP__
