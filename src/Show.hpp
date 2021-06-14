#ifndef __SHOW_HPP__
#define __SHOW_HPP__

#include <map>
#include <string>
#include "Scene.hpp"
#include "Logging.hpp"
#include <bits/stdc++.h>
#include "Output.hpp"

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