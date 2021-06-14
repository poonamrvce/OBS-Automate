#include "Scene.hpp"

vec2* Scene::default_bounds = new vec2;

obs_source_t* Scene::get_obs_scene_source() {
	return obs_scene_get_source(_obs_scene);
}

Scene::Scene(SceneParams *params) {
	_name = params->name;
    _obs_scene = obs_scene_create(params->name.c_str());
	if (!_obs_scene) {
		LOG(ERROR)<<"Error while creating obs_scene"<<endl;
        throw "Error while creating obs_scene";
	}
}

int Scene::add_source(Source *source, int index, struct vec2 *bounds) {

    _sources[source->get_source_name()] = make_pair(source,index);
    obs_sceneitem_t* obs_scene_item = obs_scene_add(_obs_scene, source->get_obs_source());
	if (!obs_scene_item) {
		LOG(ERROR)<<"Error while adding scene item"<<endl;
        return -1;
	}

	// Scale source to output size by setting bounds
	uint32_t align = OBS_ALIGN_CENTER;
	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_bounds(obs_scene_item, bounds);
	obs_sceneitem_set_bounds_alignment(obs_scene_item, align);
    return 0;
}



int Scene::add_source(Source *source, int index, struct vec4 *bounds) {

	//create transition for each scene item and add it as scene item
	obs_source_t* _obs_transition = obs_source_create(Show::defaultTransitionType.c_str(),
                 source->get_source_name().c_str(), NULL, nullptr);
	if (!_obs_transition) {
		LOG(ERROR)<<"Error while creating obs_transition"<<endl;
        throw "Unable to create obs_transition";
	}
	_transitions[source->get_source_name()]=_obs_transition;

    obs_sceneitem_t* obs_scene_item = obs_scene_add(_obs_scene, _obs_transition);
	if (!obs_scene_item) {
		LOG(ERROR)<<"Error while adding scene item"<<endl;
        return -1;
	}

    _sources[source->get_source_name()] = make_pair(source,index);


	// a and b top left corner
	// c and d resolution

	struct vec2* top_left_corner = new vec2;
	top_left_corner->x = bounds->x;
	top_left_corner->y = bounds->y;

	struct vec2* resolution = new vec2;
	resolution->x = bounds->w;
	resolution->y = bounds->z;

	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_pos(obs_scene_item, top_left_corner);
	obs_sceneitem_set_bounds(obs_scene_item, resolution);
	
	return 0;
}

vec2 Scene::get_bounds(){
	vec2 bounds;
	bounds.x=obs_source_get_width(get_obs_scene_source());
	bounds.y=obs_source_get_height(get_obs_scene_source());
	return bounds;
}

Source* Scene::get_source(string source_name){
	return _sources[source_name].first;
}

void Scene::start_scene_item(Source* scene_item){

    obs_transition_start(_transitions[scene_item->get_source_name()], OBS_TRANSITION_MODE_AUTO, 1, scene_item->get_obs_source());
}

void Scene::stop_scene_item(Source* scene_item){

    obs_transition_clear(_transitions[scene_item->get_source_name()]);
}
        
void Scene::set_scene_from_flags(int flags){
	int i=0;
	for(auto source:_sources){
		if(flags & 1<<i){
			auto source_name=source.first;
			obs_transition_set(_transitions[source_name],source.second.first->get_obs_source());
		}
		i++;
	}
}