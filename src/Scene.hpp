#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <string>
#include <map>
#include "obs/obs.h"
#include "Source.hpp"

struct SceneParams {
    std::string   name;
};

class Scene {
    public:
    Scene(SceneParams *params);
    obs_source_t* GetSceneSource() {
        return obs_scene_get_source(obs_scene_);
    }
    obs_scene_t *GetScene() {
        return obs_scene_;
    }
    std::string GetName() {
        return name_;
    }
    int AddSource(Source *source, struct vec2 *bounds);
    private:
    std::string  name_;
    std::map<std::string, Source*>  sources_; /**< Sources belonging to the scene */
    obs_scene_t* obs_scene_;

};

#endif //__SCENE_HPP__