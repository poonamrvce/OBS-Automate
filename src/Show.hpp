#ifndef __SHOW_HPP__
#define __SHOW_HPP__

#include <map>
#include <string>
#include "Scene.hpp"
#include "Logging.hpp"
#include <bits/stdc++.h>

using namespace std;

enum class ShowItemType
{
    Source,
    Scene,
    SceneItem
};

struct ShowSource{

    Source *source;
};

struct ShowScene{

    Scene *scene;
    int flags;
};

struct ShowSceneItem{

    Scene *scene;
    Source *sceneItem;
    bool start;
};


struct ShowItem
{
    ShowItemType itemType;
    union
    {
        ShowSource showSource;
        ShowScene showScene;
        ShowSceneItem showSceneItem;
    };
    int duration;
};

class Show {
    public:
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

    private:
        map<string,Source*> _sources;
        map<string, Scene*> _scenes;
        list<ShowItem> _show_items;
        obs_source_t* _root_transition;
    public:
        static string defaultTransitionType;
        Show(map<string,Source*> sources,
            map<string, Scene*> scenes,
            list<ShowItem> show_items);
        void play_show_items();
};


#endif //__SHOW_HPP__