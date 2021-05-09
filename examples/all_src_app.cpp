#include <QApplication>
#include <QPushButton>
#include <bits/stdc++.h>
#include <unistd.h>
#include "obs/obs.h"
#include "Utils.hpp"
#include "ObsUtils.hpp"
#include "Encoder.hpp"
#include "Show.hpp"
#include "Scene.hpp"
#include "Source.hpp"
#include "Output.hpp"
#include <thread>
#include <iostream>


using namespace std;

map<Scene*,list<Source*>> scenes;

int main(int argc, char *argv[]) {

    string   show_name("SceneSwitchShow");
    obs_output_t*   output;
	obs_data_t*     ffmpeg_mux_settings;
    
	QApplication app(argc, argv);
	ObsInit();


    Show *show = new Show(show_name);

    if(argc>1){
        filebuf fb;
            
        if (fb.open ("input.txt",ios::in))
        {
            istream is(&fb);
            input_scene(scenes,is);
            fb.close();
        }else{
            cout<<"couldn't open\n";
            exit(0);
        }
    }else
        input_scene(scenes,cin);

    for(auto scene:scenes){
        show->AddScene(scene.first);
        show->SetActiveScene((*scenes.begin()).first->GetName());
    }

    RTMPInfo info={"rtmp://localhost:1935/live","app4"};

    Output myoutput(OutputType::RTMP,&info);
    myoutput.set_show(show);
    myoutput.start_output(false);
	// SLeep for 1000 seconds
    unordered_set<string> scene_names;
    cout<<"Scenes: ";
    for(auto scene :scenes){
        string s=scene.first->GetName();
        cout<<s<<", ";
        scene_names.insert(s);
    }
    cout<<"\n";
    string cmd;
	for(;;) {
        
        getline(cin,cmd);
        if(scene_names.find(cmd)==scene_names.end())
            cout<<"can't find source";
        else
            show->SceneSwitch(cmd);
        
	}
    

}