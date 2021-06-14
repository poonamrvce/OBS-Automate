#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <bits/stdc++.h>
#include "obs/obs.h"
#include "Source.hpp"
#include "Logging.hpp"
#include "Show.hpp"

using namespace std;

struct SceneParams {
    string   name;
};

class Scene {
    
    private:
        string  _name;
        map<string, pair<Source*,int>>  _sources; /**< Sources belonging to the scene */
        map<string, obs_source_t*>  _transitions; /**< Sources belonging to the scene */
        obs_scene_t* _obs_scene;

    public:
        static vec2 *default_bounds ;

        Scene(SceneParams *params);
        obs_scene_t *get_obs_scene() {return _obs_scene;}
        string get_scene_name() {return _name;}
        obs_source_t* get_obs_scene_source();
        vec2 get_bounds();
        Source* get_source(string source_name);
        int add_source(Source *source, int index, vec4 *bounds);
        int get_source_index(string source_name);
        void set_scene_from_flags(int flags);
        void start_scene_item(Source* scene_item);
        void stop_scene_item(Source* scene_item);

        int add_source(Source *source, int index, vec2 *bounds);
};

#endif //__SCENE_HPP__