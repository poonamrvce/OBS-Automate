#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <bits/stdc++.h>
#include "Scene.hpp"
#include "Source.hpp"
#include "Show.hpp"

//regex definitions
#define wsp "[ ]*"
#define keyword "image|color|video|text|browser"
#define identifier "[0-9a-zA-Z_]+"
#define digit "[0-9]"
#define decimal digit "+[.]" digit "+"
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
        string unit=time_string.substr(time_string.size()- 3,3);
        if(unit=="sec")
            return stof(time_string.substr(0,time_string.size()-3));
        else
            return 60*stof(time_string.substr(0,time_string.size()-3));
    }

    Source *get_source_from_string(string source_string)
    {

        smatch sm;
        regex_match(source_string, sm, source_pat);

        string source_id = sm.str(1),
               source_type = sm.str(2),
               options_string = sm.str(3);

        //parse options into list
        list<string> options = split(options_string, ' ');

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id == "" ? (source_type + to_string(count[source_type])) : source_id;
        if (source_type == "image")
        {
            sourceParams.sourceType = SourceType::Image;
            sourceParams.localFile = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "video")
        {
            sourceParams.sourceType = SourceType::LocalFile;
            sourceParams.localFile = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "browser")
        {
            sourceParams.sourceType = SourceType::Browser;
            sourceParams.url = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "color")
        {
            sourceParams.sourceType = SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
            options.pop_front();
        }
        return new Source(&sourceParams);
    }

    void get_scene_source_from_string(string source_string, Scene *scene, int index)
    {

        smatch sm;
        regex_match(source_string, sm, source_pat);

        string source_id = sm.str(1),
               source_type = sm.str(2),
               options_string = sm.str(3);

        //parse options into list
        list<string> options = split(options_string, ' ');

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id == "" ? (source_type + to_string(count[source_type])) : source_id;
        if (source_type == "image")
        {
            sourceParams.sourceType = SourceType::Image;
            sourceParams.localFile = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "video")
        {
            sourceParams.sourceType = SourceType::LocalFile;
            sourceParams.localFile = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "browser")
        {
            sourceParams.sourceType = SourceType::Browser;
            sourceParams.url = options.front().c_str();
            options.pop_front();
        }
        else if (source_type == "color")
        {
            sourceParams.sourceType = SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
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

        scene->add_source(source, index, &bbox_struct);
    }

    Scene *get_scene_from_string(string scene_name, string scene_items_string)
    {
        sregex_iterator itr2;
        SceneParams sceneParams = {
            .name = scene_name};
        Scene *scene = new Scene(&sceneParams);
        int scene_item_index=0;
        for (auto source_string : split(scene_items_string, ';'))
        {
            get_scene_source_from_string(source_string, scene, scene_item_index);
            scene_item_index++;
        }
        return scene;
    }

    list<ShowItem> get_show_items_from_string(string item_string){
        
        smatch sm;
        regex_match(item_string, sm, show_item_pat);
        ShowItem item;
        item.duration=get_time(sm.str(2));
        if(scenes.find(sm.str(1))!=scenes.end()){
            item.itemType=ShowItemType::Scene;
            item.showScene={
                .scene=scenes[sm.str(1)],
                .flags=0
            };
        }else if(sources.find(sm.str(1))!=sources.end()){
            item.showSource={
                .source= sources[sm.str(1)]};
            item.itemType=ShowItemType::Source;
        }else
            return {};
        
        if(item.itemType==ShowItemType::Scene && !sm.str(5).empty()){
            list<ShowItem> items;
            auto scene_items=sm.str(5);
            sregex_iterator b;
            for(auto a=sregex_iterator(scene_items.begin(),scene_items.end(),show_scene_items_pat);a!=b;a++){
                string str=(*a).str();
                auto lis=split(str,' ');
                ShowItem subitem;
                subitem.itemType=ShowItemType::SceneItem;
                subitem.showSceneItem={
                    .scene=item.showScene.scene,
                    .sceneItem=item.showScene.scene->get_source(lis.front()),
                    .start=true
                };

                auto index=item.showScene.scene->get_source_index(lis.front());
                lis=split(lis.back(),':');

                if(!lis.front().empty()){

                    item.showScene.flags = item.showScene.flags | 1<<index;
                    subitem.duration=get_time(lis.front());
                    subitem.showSceneItem.start=true;
                    if(subitem.duration<item.duration)
                        items.push_back(subitem);
                }

                if(!lis.back().empty()){

                    subitem.duration+=get_time(lis.back());
                    subitem.showSceneItem.start=false;
                    if(subitem.duration<item.duration)
                        items.push_back(subitem);
                }
            }

            //sort by time
            sort(items.begin(),items.end(),[](ShowItem &a, ShowItem &b)-> bool {return a.duration<b.duration;});
            items.push_back(item);

            //convert scene-item's time from (relative to parent-scene) to (relative to previous-scene-item)
            int time_elapsed=0;
            for(auto i=items.begin();i!=items.end();i++){
                i->duration-=time_elapsed;
                time_elapsed+=i->duration;
            }
            return items;
        }else
            return {item};
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
        ifstream file(file_name);
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
        while (getline(file, input_string))
        {
            auto items = get_show_items_from_string(input_string);
            global_items.splice(global_items.end(), items);
        }

        this->InputParser::~InputParser();
        return Show(sources,scenes,global_items);
    }
};

    
#endif