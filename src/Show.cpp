
#include "Show.hpp"

string Show::defaultTransitionType = "cut_transition";

Show::Show(map<string,Source*> sources,
            map<string, Scene*> scenes,
            list<ShowItem> show_items){
            
    _sources=sources;
    _scenes=scenes;
    _show_items=show_items;
	_root_transition = obs_source_create(defaultTransitionType.c_str(),
                 "root_transition", NULL, nullptr);
    obs_transition_set_alignment(_root_transition,OBS_ALIGN_CENTER);
    obs_transition_set_scale_type(_root_transition,OBS_TRANSITION_SCALE_STRETCH);
    obs_transition_set_size(_root_transition,1920,1080);
	if (!_root_transition) {
		LOG(ERROR)<<"Error while creating obs_transition"<<std::endl;
        throw "Unable to create obs_transition";
	}
}

void Show::play_show_items(){

    LOG(DEBUG)<<"playing "<<_show_items.size()<<" items\n";

    Output myoutput(OutputType::RTMP,NULL);
    myoutput.set_output_source(_root_transition);
    myoutput.start_output();
    
    for(auto item:_show_items){
        LOG(INFO)<<"now playing :";
        if(item.is_source){

                cout<<"Source ,"<<item.showSource.source->get_source_name()<<endl;
                obs_transition_start(_root_transition,OBS_TRANSITION_MODE_AUTO, 1, item.showSource.source->get_obs_source());
                sleep(item.duration);
        }else{

                cout<<"Scene ,"<<item.showScene.scene->get_scene_name()<<endl;
                item.showScene.scene->set_scene_from_flags(item.showScene.flags);
                obs_transition_start(_root_transition,OBS_TRANSITION_MODE_AUTO, 1, item.showScene.scene->get_obs_scene_source());
                
                LOG(DEBUG)<<"ShowSceneItem "<<endl;;

                int time_elapsed=0;
                ShowSceneItem* cur=item.showScene.head;

                while (cur)
                {
                    LOG(INFO)<<"now doing item: "<<cur->source->get_source_name()<<" is_start:"<<cur->is_start<<endl ;

                    sleep( cur->time_step - time_elapsed);

                    if(cur->is_start)
                        item.showScene.scene->start_scene_item(cur->source);
                    else
                        item.showScene.scene->stop_scene_item(cur->source);
                    
                    time_elapsed=cur->time_step;
                    auto temp=cur;
                    cur = cur->next;
                    delete temp;
                }

                LOG(DEBUG)<<"scene duration is "<<item.duration<< " and elapsed time is "<< time_elapsed<<"\n";
                sleep(item.duration - time_elapsed );


                LOG(DEBUG)<<"finish6"<<endl;
        }
    }
}
