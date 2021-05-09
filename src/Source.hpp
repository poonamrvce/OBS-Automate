#ifndef __SOURCE_HPP__
#define __SOURCE_HPP__

#include <string>
#include "obs/obs.h"
#include "Logging.hpp"
#include <unordered_map>

using namespace std;

enum class SourceType {
	InvalidType = -1,
	LocalFile = 0,
	RTMP,
    Browser,
    Color,
    Image
};

struct SourceParams {
    string   name;
    SourceType    sourceType;
    union{
        const char *localFile;
        const char *url;
        unsigned int color;
    };
};

class Source {
    public:
    static unordered_map<string,pair<SourceType,string>> all_sources;
    static string allSourcePattern();
    Source(SourceParams *params);
    obs_source_t *GetObsSource() { return obs_source_;}
    string  GetName() { return name_;}

    private:
    string   name_;
    obs_source_t   *obs_source_;
    private:
    obs_source_t* localFileSourceCreate(string name, string localFile, bool looping);
    obs_source_t* browserSourceCreate(string name, string url);
    obs_source_t* colorSourceCreate(string name, int color);
    obs_source_t* imageSourceCreate(string name, string localFile);

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