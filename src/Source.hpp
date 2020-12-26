#ifndef __SOURCE_HPP__
#define __SOURCE_HPP__

#include <string>
#include "obs/obs.h"


enum SourceType {
	InvalidType = -1,
	LocalFile = 0,
	RTMP
};

struct SourceParams {
    std::string   name;
    SourceType    sourceType;
    std::string   url;
};

class Source {
    public:
    Source(SourceParams *params);
    obs_source_t *GetObsSource() { return obs_source_;}
    std::string  GetName() { return name_;}
    private:
    std::string   name_;
    obs_source_t   *obs_source_;
    private:
    obs_source_t* localFileSourceCreate(std::string name, std::string url, bool looping);

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