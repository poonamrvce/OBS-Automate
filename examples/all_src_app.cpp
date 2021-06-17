#include <bits/stdc++.h>
#include "Utils.hpp"
#include "ObsUtils.hpp"


using namespace std;

int main(int argc, char *argv[]) {
    
    if(argc==1){
        LOG(ERROR)<<"input file required";
        exit(0);
    }

    ObsInit(argc,argv);
    
    auto input_file=string(argv[1]);
    
    auto show=InputParser().parse_input_file(input_file);
    LOG(DEBUG)<<"Done Parsing! "<<endl;

    show.play_show_items();

    // obs_source_t * root_transition;
	// root_transition = obs_source_create("cut_transition","root_transition", NULL, nullptr);
    // obs_transition_set_alignment(root_transition,OBS_ALIGN_CENTER);
    // obs_transition_set_scale_type(root_transition,OBS_TRANSITION_SCALE_STRETCH);
    // obs_transition_set_size(root_transition,1920,1080);


    // obs_scene_t * scene1 = obs_scene_create("scene1");
	// obs_transition_set(root_transition,obs_scene_get_source(scene1) );
    // LOG(DEBUG)<<"width:"<<obs_source_get_width(obs_scene_get_source(scene1))<<endl;
    // LOG(DEBUG)<<"height:"<<obs_source_get_height(obs_scene_get_source(scene1))<<endl;

	// obs_source_t * transition1 = obs_source_create("cut_transition","transition1", NULL, nullptr);
    // obs_sceneitem_t* obs_scene_item1 = obs_scene_add(scene1, transition1);
    // // vec2 bounds1;
    // // bounds1.x=500;
    // // bounds1.y=400;
    // // obs_sceneitem_set_bounds(obs_scene_item1,&bounds1);
    // // vec2 pos1;
    // // pos1.x=200;
    // // pos1.y=100;
    // // obs_sceneitem_set_pos(obs_scene_item1,&pos1);
    // SourceParams params;
    // params.localFile="/vol/sample/raama.jpg";
    // params.sourceType=SourceType::Image;
    // obs_source_t* source1= Source(&params).get_obs_source();
	// obs_transition_start(transition1,OBS_TRANSITION_MODE_AUTO, 1,source1);


	// obs_source_t * transition2 = obs_source_create("cut_transition","transition2", NULL, nullptr);
    // obs_sceneitem_t* obs_scene_item2 = obs_scene_add(scene1, transition2);
    // vec2 bounds2;
    // bounds2.x=1200;
    // bounds2.y=900;
    // obs_sceneitem_set_bounds(obs_scene_item2,&bounds2);
    // obs_sceneitem_set_bounds_type(obs_scene_item2,OBS_BOUNDS_SCALE_INNER);
    // vec2 pos2;
    // pos2.x=100;
    // pos2.y=100;
    // obs_sceneitem_set_pos(obs_scene_item2,&pos2);
    // params.localFile="/vol/sample/krishna.mp4";
    // params.sourceType=SourceType::LocalFile;
    // obs_source_t* source2= Source(&params).get_obs_source() ;
	// obs_transition_start(transition2,OBS_TRANSITION_MODE_AUTO, 1,source2);

    // Output myoutput(OutputType::RTMP,NULL);
    // myoutput.set_output_source(root_transition);
    // myoutput.start_output();

    // while(1);
}