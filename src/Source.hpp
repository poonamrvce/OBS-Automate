#ifndef __SOURCE_HPP__
#define __SOURCE_HPP__

#include "obs/obs.h"
#include "Logging.hpp"
#include "ObsUtils.hpp"
#include <bits/stdc++.h>

using namespace std;

enum class SourceType {
	InvalidType = -1,
	LocalFile = 0,
    Browser,
    Color,
    Image,
    Text,
    Scene,
	// RTMP,
};

struct SourceParams {
    string   name;
    SourceType    sourceType;
    union{
        const char *localFile;
        const char *url;
        unsigned int color;
        const char *text;
    };
};

class Source {
    public:
    static set<SourceType> loaded_modules;
    Source(SourceParams *params);
    obs_source_t *get_obs_source() { return obs_source_;}
    string  get_source_name() { return _name;}

    private:
    string   _name;
    obs_source_t   *obs_source_;
    obs_source_t* get_local_file_source(string name,  const char* localFile, bool looping);
    obs_source_t* get_browser_source(string name,  const char* url);
    obs_source_t* get_color_source(string name, int color);
    obs_source_t* get_image_source(string name,  const char* localFile);
    obs_source_t* get_text_source(string name,  const char* text);


};

/**
 * Source signal callbacks
 */
void SourceShowCb(void *data, calldata_t *cd);
void SourceHideCb(void *data, calldata_t *cd);
void SourceActivateCb(void *data, calldata_t *cd);
void SourceTransitionStartCb(void *data, calldata_t *cd);
void SourceTransitionVideoStopCb(void *data, calldata_t *cd);
void SourceTransitionStopCb(void *data, calldata_t *cd);


#endif //__SOURCE_HPP__