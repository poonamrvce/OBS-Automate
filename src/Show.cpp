
#include "Show.hpp"
#include "Trace.hpp"

std::string Show::defaultTransitionType = "cut_transition";

Show::Show(std::string name) {
    name_ = name;
    std::string transition_name = std::string("transition_"+ name_);
	obs_transition_ = obs_source_create(defaultTransitionType.c_str(),
                 transition_name.c_str(), NULL, nullptr);
	if (!obs_transition_) {
		trace_error("Error while creating obs_transition");
        throw "Unable to create obs_transition";
	}
}


void Show::SetActiveScene(std::string  scene_name) {
    active_scene_ = scene_name;
    Scene *active_scene = scenes_[active_scene_];
    obs_transition_set(obs_transition_, 
        obs_scene_get_source(active_scene->GetScene()));
    return;
}

void Show::SceneSwitch(std::string scene_name) {
    Scene *to_scene = scenes_[scene_name];
    obs_transition_start(obs_transition_, OBS_TRANSITION_MODE_AUTO, 1, to_scene->GetSceneSource());
}

int Show::AddScene(Scene *scene) {
	scenes_[scene->GetName()] = scene;
}