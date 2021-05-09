#include <QApplication>
#include <QPushButton>
#include <bits/stdc++.h>
#include <unistd.h>
#include "obs/obs.h"
#include "ObsUtils.hpp"
#include "Utils.hpp"
#include "Encoder.hpp"
#include "Show.hpp"
#include "Scene.hpp"
#include "Source.hpp"
#include "Output.hpp"
#include <thread>

#define app_output_file "/vol/sample/app2.mkv"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 3) {
        // cout << "Usage: ./scene_switch_app <file1> <file2>";
        // exit(1);
        argv[1]="/vol/sample/raama.jpg";
        argv[2]="/vol/sample/krishna.mp4";
    }
    string   local_file1(argv[1]);
    string   local_file2(argv[2]);
    string   show_name("SceneSwitchShow");
    obs_output_t*   output;
	obs_data_t*     ffmpeg_mux_settings;
    
	QApplication app(argc, argv);
	ObsInit();

    // Create a file source
    SourceParams sourceParams = {
        .name = "local_file_source_1",
        .sourceType = SourceType::Image,
        {
            .localFile = (char *)local_file1.c_str()
        }
    };
    SceneParams  sceneParams = {
        .name = get_file_name(sourceParams.url)
    };
    SourceParams sourceParams2 = {
        .name = "local_file_source_2",
        .sourceType = SourceType::LocalFile,
        {
            .localFile = (char *)local_file2.c_str()
        }
    };
    SceneParams  sceneParams2 = {
        .name = get_file_name(sourceParams2.url)
    };
    Source *source1 = new Source(&sourceParams); 
    Scene *scene1 = new Scene(&sceneParams);
    Source *source2 = new Source(&sourceParams2); 
    Scene *scene2 = new Scene(&sceneParams2);

    Show *show = new Show(show_name);
    struct vec2 bounds = {1920, 1080};
    scene1->AddSource(source1,  &bounds);
    
    scene2->AddSource(source2,  &bounds);

    show->AddScene(scene1);
    show->AddScene(scene2);
    show->SetActiveScene(get_file_name(sourceParams.url));

    RTMPInfo info={"rtmp://localhost:1935/live","app2"};

    Output myoutput(OutputType::RTMP,&info);
    myoutput.set_show(show);
    myoutput.start_output(true);
    unordered_set<string> scene_names={sceneParams.name,sceneParams2.name};
	// SLeep for 1000 seconds
    cout<<"Sources: ";
    for(auto scene_name :scene_names)
        cout<<scene_name<<", ";
    cout<<"\n";
    char cmd[100];
	for(;;) {
        
        cin>>cmd;
        if(scene_names.find(cmd)==scene_names.end())
            cout<<"can't find source";
        else
            show->SceneSwitch(cmd);
        
	}
    

}