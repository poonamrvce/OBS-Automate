#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <string>
#include <map>
#include "obs/obs.h"
#include "Source.hpp"
#include "Logging.hpp"

struct SceneParams {
    std::string   name;
};

class Scene {
    
    private:
    std::string  _name;
    std::map<std::string, Source*>  _sources; /**< Sources belonging to the scene */
    obs_scene_t* _obs_scene;

    public:
    obs_scene_t *GetScene() {
        return _obs_scene;
    }
    std::string get_source_name() {
        return _name;
    }
    static vec2 *default_bounds ;
    Scene(SceneParams *params);
    obs_source_t* GetSceneSource();
    int AddSource(Source *source, vec2 *bounds);
    int AddSource(Source *source, vec4 *bounds);
    vec2 get_bounds();
};

#endif //__SCENE_HPP__