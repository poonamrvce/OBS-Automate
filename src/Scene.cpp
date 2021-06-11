#include "obs/obs.h"
#include "Scene.hpp"

vec2* Scene::default_bounds = new vec2;

obs_source_t* Scene::GetSceneSource() {
	return obs_scene_get_source(_obs_scene);
}

Scene::Scene(SceneParams *params) {
	_name = params->name;
    _obs_scene = obs_scene_create(params->name.c_str());
	if (!_obs_scene) {
		LOG(ERROR)<<"Error while creating obs_scene"<<std::endl;
        throw "Error while creating obs_scene";
	}
}

int Scene::add_source(Source *source, struct vec2 *bounds) {
    std::string transition_name = std::string("transition_"+ name_);
	obs_source_t* obs_transition_ = obs_source_create(defaultTransitionType.c_str(),
                 transition_name.c_str(), NULL, nullptr);
	if (!obs_transition_) {
		LOG(ERROR)<<"Error while creating obs_transition"<<std::endl;
        throw "Unable to create obs_transition";
	}
    obs_source_t   *obs_source;
    _sources[source->get_source_name()] = source;
    obs_sceneitem_t* obs_scene_item = obs_scene_add(_obs_scene, source->get_obs_source());
	if (!obs_scene_item) {
		LOG(ERROR)<<"Error while adding scene item"<<std::endl;
        return -1;
	}

	// Scale source to output size by setting bounds
	uint32_t align = OBS_ALIGN_CENTER;
	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_bounds(obs_scene_item, bounds);
	obs_sceneitem_set_bounds_alignment(obs_scene_item, align);
    return 0;
}

int Scene::add_source(Source *source, struct vec4 *bounds) {

	// a and b top left corner
	// c and d resolution

	struct vec2* top_left_corner = new vec2;
	top_left_corner->x = bounds->x;
	top_left_corner->y = bounds->y;

	struct vec2* resolution = new vec2;
	resolution->x = bounds->w;
	resolution->y = bounds->z;


    obs_source_t   *obs_source;
    _sources[source->get_source_name()] = source;
    obs_sceneitem_t* obs_scene_item = obs_scene_add(_obs_scene, source->get_obs_source());
	if (!obs_scene_item) {
		LOG(ERROR)<<"Error while adding scene item"<<std::endl;
        return -1;
	}

	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_pos(obs_scene_item, top_left_corner);
	obs_sceneitem_set_bounds(obs_scene_item, resolution);
	
	return 0;
}

vec2 Scene::get_bounds(){
	vec2 bounds;
	bounds.x=obs_source_get_width(GetSceneSource());
	bounds.y=obs_source_get_height(GetSceneSource());
	return bounds;
}


