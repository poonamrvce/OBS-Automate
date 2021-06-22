#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <bits/stdc++.h>
#include "Scene.hpp"
#include "Source.hpp"
#include "Show.hpp"
#include "Logging.hpp"

//regex definitions
#define wsp "[ ]*"
#define keyword "image|color|video|text|browser"
#define identifier "[0-9a-zA-Z_]+"
#define digit "[0-9]"
#define decimal digit "+([.]" digit "+)?"
#define time decimal "sec|" decimal "min"

using namespace std;


class InputParser{

private:
    regex nested_source_pat, source_pat, show_item_pat, show_scene_items_pat;
    string inp = "A ( image1 image /path/to/img/ (a,b,c,d) ;  color1 color 0x672632 (0,0,%33,%33) ; video1 video /path/jsk/ldkmkd )";
    map<string, Source *> sources;
    map<string, Scene *> scenes;
    map<string, int> count;
    list<ShowItem> global_items;

    list<string> split(string text, char delim)
    {
        string pattern = "[^ ]+";
        pattern[2] = delim;
        regex matcher(pattern);
        list<string> tokens;
        sregex_iterator b;
        for (auto a = sregex_iterator(text.begin(), text.end(), matcher); a != b; a++)
            tokens.push_back((*a).str());
        return tokens;
    }
    float get_time(string time_string){
        if(time_string=="")
            return 0;
        string unit=time_string.substr(time_string.size()- 3,3);
        try{
            if(unit=="sec")
                return stof(time_string.substr(0,time_string.size()-3));
            else
                return 60*stof(time_string.substr(0,time_string.size()-3));
        }catch(...){

            LOG(ERROR)<<"error converting :"<<time_string<<endl;
        }
    }

    Source *get_source_from_string(string source_string)
    {

        smatch sm;
        regex_match(source_string, sm, source_pat);

        string source_id = sm.str(1),
               source_type = sm.str(2),
               options_string = sm.str(3);
        LOG(DEBUG)<<"source_id:"<<source_id<<", source_type:"<<source_type<<", options_string:"<<options_string<<endl;

        //parse options into list
        list<string> options = split(options_string, ' ');

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id == "" ? (source_type + to_string(count[source_type])) : source_id;
        if (source_type == "image")
        {
            sourceParams.sourceType = SourceType::Image;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.localFile = str;
            options.pop_front();
        }
        else if (source_type == "video")
        {
            sourceParams.sourceType = SourceType::LocalFile;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.localFile = str;
            options.pop_front();
        }
        else if (source_type == "browser")
        {
            sourceParams.sourceType = SourceType::Browser;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.url = str;
            options.pop_front();
        }
        else if (source_type == "color")
        {
            sourceParams.sourceType = SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
            options.pop_front();
        }else if (source_type == "text")
        {
            sourceParams.sourceType = SourceType::Text;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.text = str;
            options.pop_front();
        }
        return new Source(&sourceParams);
    }

    void get_scene_source_from_string(string source_string, Scene *scene)
    {

        smatch sm;
        regex_match(source_string, sm, source_pat);

        string source_id = sm.str(1),
               source_type = sm.str(2),
               options_string = sm.str(3);
        LOG(DEBUG)<<endl<<"source_id:"<<source_id<<", source_type:"<<source_type<<", options_string:"<<options_string<<endl;

        //parse hey sup can u is_start audio ya starte i pressed join, did i? no you are not showing up for me.. options into list
        list<string> options = split(options_string, ' ');
        LOG(DEBUG)<<"options:"<<endl;
        for(auto opt:options)
            cout<<opt<<", ";
        cout<<endl;

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id == "" ? (source_type + to_string(count[source_type])) : source_id;
        if (source_type == "image")
        {
            sourceParams.sourceType = SourceType::Image;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.localFile = str;
            options.pop_front();
        }
        else if (source_type == "video")
        {
            sourceParams.sourceType = SourceType::LocalFile;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.localFile = str;
            options.pop_front();
        }
        else if (source_type == "browser")
        {
            sourceParams.sourceType = SourceType::Browser;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.url = str;
            options.pop_front();
        }
        else if (source_type == "color")
        {
            sourceParams.sourceType = SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
            options.pop_front();

        }else if (source_type == "text")
        {
            sourceParams.sourceType = SourceType::Text;
            auto str=new char[options.front().size()];
            strcpy(str,options.front().c_str());
            sourceParams.text = str;
            options.pop_front();
        }

        Source *source = new Source(&sourceParams);
        string bbox = options.front();
        options.pop_front();
        bbox = bbox.substr(1, bbox.size() - 2);
        auto scene_bounds = scene->get_bounds();
        vector<float> nums;
        bool xaxis = true;
        for (auto num : split(bbox, ','))
        {
            if (num.back() == '%')
            {
                num.pop_back();
                auto temp = stof(num) / 100;
                if (xaxis)
                    nums.push_back(scene_bounds.x * temp);
                else
                    nums.push_back(scene_bounds.y * temp);
            }
            else
                nums.push_back(stof(num));
            xaxis = !xaxis;
        }
        vec4 bbox_struct;
        bbox_struct.x = nums[0];
        bbox_struct.y = nums[1];
        bbox_struct.w = nums[2];
        bbox_struct.z = nums[3];

        scene->add_source(source, &bbox_struct);
    }

    Scene *get_scene_from_string(string scene_name, string scene_items_string)
    {
        sregex_iterator itr2;
        SceneParams sceneParams = {
            .name = scene_name};
        Scene *scene = new Scene(&sceneParams);
        for (auto source_string : split(scene_items_string, ';'))
        {
            get_scene_source_from_string(source_string, scene);
        }
        return scene;
    }

    ShowItem get_show_items_from_string(string item_string){
        // LOG(DEBUG)<<"parsing item string :"<<item_string<<endl;
        smatch sm;
        regex_match(item_string, sm, show_item_pat);
        ShowItem item;
        // LOG(DEBUG)<<"got :"<<sm.str(1)<<", "<<sm.str(2)<<", "<<sm.str(5)<<endl;
        item.duration=get_time(sm.str(2));
        if(sources.find(sm.str(1))!=sources.end()){
            item.is_source=true;
            item.showSource={
                .source= sources[sm.str(1)]
            };
        }else if(scenes.find(sm.str(1))!=scenes.end()){
            item.is_source=false;
            item.showScene={
                .scene=scenes[sm.str(1)],
                .flags=0,
                .head=NULL
            };
        }
        // LOG(DEBUG)<<"looking at scenetems "<<endl;
        
        if(!item.is_source && !sm.str(5).empty()){

            auto scene_items=sm.str(5);
            // LOG(DEBUG)<<"Scene's duration"<<item.duration<<endl;
            
            struct TempStruct{
                Source * source;
                int time_step;
                bool is_start;
            };
            list<TempStruct> temp_list;

            sregex_iterator b;
            for(auto a=sregex_iterator(scene_items.begin(),scene_items.end(),show_scene_items_pat);a!=b;a++){
                
                string str=(*a).str();
                auto lis=split(str,' ');
                // LOG(DEBUG)<<"sceneitem :"<<lis.front()<<endl;
                TempStruct temp={
                    .source=item.showScene.scene->get_source(lis.front())
                };
                auto index=item.showScene.scene->get_source_index(lis.front());
                lis=split(lis.back(),':');
                // LOG(DEBUG)<<"from :"<<lis.front()<<endl;
                // LOG(DEBUG)<<"duration :"<<lis.back()<<endl;
                if(!lis.front().empty()){

                    item.showScene.flags = item.showScene.flags | 1<<index;
                    temp.is_start=true;
                    temp.time_step=get_time(lis.front());
                    if(temp.time_step<item.duration) temp_list.push_back(temp);
                }else
                    temp.time_step=0;

                if(!lis.back().empty()){

                    temp.is_start=false;
                    temp.time_step+=get_time(lis.back());
                    if(temp.time_step<item.duration) temp_list.push_back(temp);
                }
            }
            if(!temp_list.empty()){

                //sort by time
                temp_list.sort([](const TempStruct & item1, const TempStruct & item2){ return item1.time_step < item2.time_step;});
                

                //creating linked list of scene items inside ShowScene 
                int time_elapsed=0;
                ShowSceneItem* curitem=new ShowSceneItem;
                item.showScene.head=curitem;
                curitem ->is_start=temp_list.front().is_start;   
                curitem ->source=temp_list.front().source;
                curitem ->time_step=temp_list.front().time_step;
                time_elapsed = curitem->time_step;
                
                temp_list.pop_front();
                        
                while(temp_list.size()){
                    curitem ->next=new ShowSceneItem;
                    curitem = curitem ->next;
                    curitem ->is_start=temp_list.front().is_start;
                    curitem ->source=temp_list.front().source;
                    curitem ->time_step=temp_list.front().time_step;
                    time_elapsed = curitem->time_step;
                    temp_list.pop_front();
                }
                curitem ->next=nullptr;

                curitem=item.showScene.head;
                LOG(DEBUG)<<"Scene items :";
                while(curitem){
                    cout<<curitem->time_step<<", ";
                    curitem=curitem->next;
                }
                cout<<endl;
            }
        }
            
        return item;
    }

public:

    InputParser()
    {

        nested_source_pat = regex(wsp "(" identifier ")?" wsp "[(](.*)[)]" wsp "$");
        source_pat = regex(wsp "(" identifier ")?" wsp "(" keyword ")"
                               "(.*)");
        show_item_pat = regex(wsp "(" identifier ")"
                                  " "
                                  "(" time ")?" wsp "(.*)");
        show_scene_items_pat = regex("("
                                     "(" identifier ")"
                                     " "
                                     "(" time ")?:(" time ")" wsp ")");
    }

    Show parse_input_file(string file_name)
    {
        ifstream file;
        try{
            file=ifstream(file_name);
        }catch(...){
            LOG(ERROR)<<"error opening :"<<file_name<<endl;
            exit(0);
        }
        string input_string;
        smatch sm;
        while (getline(file, input_string))
        {

            if (input_string == "")
                continue;
            if (input_string == "show:")
            {
                break;
            }
            else if (regex_match(input_string, sm, nested_source_pat))
            {
                auto scene = get_scene_from_string(sm.str(1), sm.str(2));
                scenes[scene->get_scene_name()] = scene;
            }
            else
            {
                auto source = get_source_from_string(input_string);
                sources[source->get_source_name()] = source;
            };
        }
        LOG(DEBUG)<<"sources: ";
        for(auto source:sources)
            cout<<source.first<<", ";
        cout<<endl;
        LOG(DEBUG)<<"scenes: ";
        for(auto scene:scenes)
            cout<<scene.first<<", ";
        cout<<endl;
        while (getline(file, input_string))
        {
            cout<<"parsing show items "<<input_string<<endl;
            if (input_string == "")
                continue;
            auto item = get_show_items_from_string(input_string);
            global_items.push_back(item);
        }
        cout<<"returning parser "<<endl;

        return Show(sources,scenes,global_items);
    }
};

    
#endif
