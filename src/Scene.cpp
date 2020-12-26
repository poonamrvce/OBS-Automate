
#include "obs/obs.h"
#include "Scene.hpp"
#include "Trace.hpp"

Scene::Scene(SceneParams *params) {
	name_ = params->name;
    obs_scene_ = obs_scene_create(params->name.c_str());
	if (!obs_scene_) {
		trace_error("Error while creating obs_scene");
        throw "Error while creating obs_scene";
	}
}

int Scene::AddSource(Source *source, struct vec2 *bounds) {
    obs_source_t   *obs_source;
    sources_[source->GetName()] = source;
    obs_sceneitem_t* obs_scene_item = obs_scene_add(obs_scene_, source->GetObsSource());
	if (!obs_scene_item) {
		trace_error("Error while adding scene item");
        return -1;
	}

	// Scale source to output size by setting bounds
	uint32_t align = OBS_ALIGN_TOP + OBS_ALIGN_LEFT;
	obs_sceneitem_set_bounds_type(obs_scene_item, OBS_BOUNDS_SCALE_INNER);
	obs_sceneitem_set_bounds(obs_scene_item, bounds);
	obs_sceneitem_set_bounds_alignment(obs_scene_item, align);
    return 0;
}


