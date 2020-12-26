#ifndef __SHOW_HPP__
#define __SHOW_HPP__

#include <map>
#include <string>
#include "Scene.hpp"



class Show {
    public:
    static std::string defaultTransitionType;
    Show(std::string   name); 
    int AddScene(Scene *scene);
    void SetActiveScene(std::string scene_name);
    obs_source_t* GetShowSource() { return obs_transition_;} 
    void SceneSwitch(std::string scene_name);
    private:
    std::string   name_;
    std::string   active_scene_;
    obs_source_t* obs_transition_;
    std::map<std::string, Scene*>   scenes_; /**< Collection of scenes belonging to a show */

};


#endif //__SHOW_HPP__