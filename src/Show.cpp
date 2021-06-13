
#include "Show.hpp"

string Show::defaultTransitionType = "cut_transition";

Show::Show(std::string name) {
    name_ = name;
    std::string transition_name = std::string("transition_"+ name_);
	obs_transition_ = obs_source_create(defaultTransitionType.c_str(),
                 transition_name.c_str(), NULL, nullptr);
	if (!obs_transition_) {
		LOG(ERROR)<<"Error while creating obs_transition"<<std::endl;
        throw "Unable to create obs_transition";
	}
}

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


void Show::SetActiveScene(std::string  scene_name) {
    active_scene_ = scene_name;
    Scene *active_scene = scenes_[active_scene_];
    obs_transition_set(obs_transition_, 
        obs_scene_get_source(active_scene->get_scene()));
    return;
}

void Show::SceneSwitch(std::string scene_name) {
    Scene *to_scene = scenes_[scene_name];
    obs_transition_start(obs_transition_, OBS_TRANSITION_MODE_AUTO, 1, to_scene->get_scene_source());
}

int Show::AddScene(Scene *scene) {
	scenes_[scene->get_scene_name()] = scene;
}