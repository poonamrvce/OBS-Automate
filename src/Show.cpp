
#include "Show.hpp"

string Show::defaultTransitionType = "cut_transition";

Show::Show(map<string,Source*> sources,
            map<string, Scene*> scenes,
            list<ShowItem> show_items){
	_root_transition = obs_source_create(defaultTransitionType.c_str(),
                 "root_transition", NULL, nullptr);
	if (!_root_transition) {
		LOG(ERROR)<<"Error while creating obs_transition"<<std::endl;
        throw "Unable to create obs_transition";
	}
}

void Show::play_show_items(){
    for(auto item:_show_items){
        switch (item.itemType)
        {
            case ShowItemType::Scene:
                item.showScene.scene->set_scene_from_flags(item.showScene.flags);
                obs_transition_start(_root_transition,OBS_TRANSITION_MODE_AUTO, 1, item.showScene.scene->get_obs_scene_source());
                break;
            case ShowItemType::Source:
                obs_transition_start(_root_transition,OBS_TRANSITION_MODE_AUTO, 1, item.showSource.source->get_obs_source());
                break;
            case ShowItemType::SceneItem:
                if(item.showSceneItem.start)
                    item.showSceneItem.scene->start_scene_item(item.showSceneItem.sceneItem);
                else
                    item.showSceneItem.scene->stop_scene_item(item.showSceneItem.sceneItem);
                break;
        }
        sleep(item.duration);
    }
}
