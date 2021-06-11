#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <bits/stdc++.h>
#include "Scene.hpp"
#include "Source.hpp"

//regex definitions
#define wsp "[ ]*"
#define keyword "image|color|video|text|browser"
#define identifier "[0-9a-zA-Z_]+"


using namespace std;

string get_file_name(string url)
{
    int i = url.find_last_of('/');
    auto j = url.find_last_of('.');
    return url.substr(i + 1, j - i - 1);
}

list<string> split1(string s)
{
    regex pat("[a-zA-Z0-9_./]+");
    list<string> vec;
    sregex_iterator it2;
    for (sregex_iterator it1(s.begin(), s.end(), pat); it1 != it2; it1++)
    {
        vec.push_back((*it1).str());
    }
    return vec;
}

// void input_scene(map<Scene *, list<Source *>> &scenes, istream &stream)
// {
//     cout << "The scenes:" << endl;
//     for (auto source : Source::all_sources)
//         cout << source.first << "-->" << source.second.second << endl;

//     string input_string;
//     while (true)
//     {

//         if (!getline(stream, input_string) || input_string.size() == 0)

//             if (scenes.size() == 0)
//             {
//                 cout << "At least one scene required!" << endl;
//                 continue;
//             }
//             else
//                 break;
//         regex
//             scene_name_pat("-sc [^ ]+"),
//             src_names("-" + Source::allSourcePattern() + " [^-]+");
//         smatch match;

//         SceneParams sceneParams = {
//             .name = regex_search(input_string, match, scene_name_pat) ? match.str(0).substr(4) : "scene " + scenes.size() + 1};
//         LOG(INFO) << "created Scene " << sceneParams.name << endl;

//         Scene *scene = new Scene(&sceneParams);

//         list<Source *> source_list;
//         sregex_iterator itr2;
//         for (sregex_iterator
//                  itr1(input_string.begin(), input_string.end(), src_names);
//              itr1 != itr2;
//              itr1++)
//         {
//             string temp = (*itr1).str();

//             SourceParams sourceParams;
//             sourceParams.sourceType = Source::all_sources[temp.substr(1, 2)].first;
//             list<string> lis = split1(temp.substr(4));
//             switch (sourceParams.sourceType)
//             {

//             case SourceType::LocalFile:
//                 if (lis.size() > 1)
//                 {
//                     sourceParams.name = lis.front();
//                     lis.pop_front();
//                 }
//                 else
//                     sourceParams.name = get_file_name(lis.front());
//                 sourceParams.localFile = lis.front().c_str();
//                 LOG(INFO) << "Source " << sourceParams.name << " localFile " << sourceParams.localFile << endl;
//                 break;

//             case SourceType::Browser:
//                 if (lis.size() > 1)
//                 {
//                     sourceParams.name = lis.front();
//                     lis.pop_front();
//                 }
//                 else
//                     sourceParams.name = get_file_name(lis.front());
//                 sourceParams.url = lis.front().c_str();
//                 LOG(INFO) << "Source " << sourceParams.name << " brows " << sourceParams.url << endl;
//                 break;

//             case SourceType::Color:
//                 if (lis.size() > 1)
//                 {
//                     sourceParams.name = lis.front();
//                     lis.pop_front();
//                 }
//                 else
//                     sourceParams.name = "C" + lis.front();
//                 sourceParams.color = strtoul(lis.front().c_str(), NULL, 16);
//                 LOG(INFO) << "Source " << sourceParams.name << " color " << hex << sourceParams.color << endl;
//                 break;

//             case SourceType::Image:
//                 if (lis.size() > 1)
//                 {
//                     sourceParams.name = lis.front();
//                     lis.pop_front();
//                 }
//                 else
//                     sourceParams.name = get_file_name(lis.front());
//                 sourceParams.localFile = lis.front().c_str();
//                 LOG(INFO) << "Source " << sourceParams.name << " localFile " << sourceParams.localFile << endl;
//                 break;

//             default:
//                 LOG(ERROR) << "Unknown SourceType received" << endl;
//             }

//             Source *source = new Source(&sourceParams);
//             struct vec2 bounds = {1920, 1080};
//             scene->add_source(source, &bounds);
//         }
//         scenes[scene] = source_list;
//     }
// }


class InputParser{
    private:
        regex nested_source_pat,source_pat;
        string inp="A ( image1 image /path/to/img/ (a,b,c,d) ;  color1 color 0x672632 (0,0,%33,%33) ; video1 video /path/jsk/ldkmkd )";
        map<string, Source*> scenes;
        map<string, Scene*> sources;
        map<string, int> count;

    InputParser(){
        nested_source_pat=regex( wsp "(" identifier ")?" wsp "[(](.*)[)]" wsp "$" );
        source_pat=regex( wsp "(" identifier ")?" wsp "(" keyword ")" "(.*)" );
    }

    list<string> split(string text, char delim){
        string pattern="[^ ]+";
        pattern[2]=delim;
        regex matcher(pattern);
        list<string> tokens;
        sregex_iterator b;
        for(auto a=sregex_iterator(text.begin(),text.end(),matcher);a!=b;a++)
            tokens.push_back((*a).str());
        return tokens;
    }

    Source* get_source_from_srting(string source_string){

        smatch sm;
        regex_match (source_string,sm,source_pat);

        string source_id=sm.str(1), 
                source_type=sm.str(2), 
                options_string=sm.str(3);

        //parse options into list
        list<string> options=split(options_string,' ');

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id==""?(source_type+to_string(count[source_type])):source_id;
        if(source_type=="image"){
            sourceParams.sourceType=SourceType::Image;
            sourceParams.localFile=options.front().c_str();
            options.pop_front();
            
        }else if(source_type=="video"){
            sourceParams.sourceType=SourceType::LocalFile;
            sourceParams.localFile=options.front().c_str();
            options.pop_front();

        }else if(source_type=="browser"){
            sourceParams.sourceType=SourceType::Browser;
            sourceParams.url=options.front().c_str();
            options.pop_front();
            
        }else if(source_type=="color"){
            sourceParams.sourceType=SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
            options.pop_front();
        }
        return new Source(&sourceParams);
    }

    void get_scene_source_from_srting(string source_string,Scene* scene){

        smatch sm;
        regex_match (source_string,sm,source_pat);

        string source_id=sm.str(1), 
                source_type=sm.str(2), 
                options_string=sm.str(3);

        //parse options into list
        list<string> options=split(options_string,' ');

        //create source
        SourceParams sourceParams;
        sourceParams.name = source_id==""?(source_type+to_string(count[source_type])):source_id;
        if(source_type=="image"){
            sourceParams.sourceType=SourceType::Image;
            sourceParams.localFile=options.front().c_str();
            options.pop_front();
            
        }else if(source_type=="video"){
            sourceParams.sourceType=SourceType::LocalFile;
            sourceParams.localFile=options.front().c_str();
            options.pop_front();

        }else if(source_type=="browser"){
            sourceParams.sourceType=SourceType::Browser;
            sourceParams.url=options.front().c_str();
            options.pop_front();
            
        }else if(source_type=="color"){
            sourceParams.sourceType=SourceType::Color;
            sourceParams.color = strtoul(options.front().c_str(), NULL, 16);
            options.pop_front();
        }

        Source *source=new Source(&sourceParams);
        string bbox=options.front();
        options.pop_front();
        bbox=bbox.substr(1,bbox.size()-2);
        auto scene_bounds=scene->get_bounds();
        vector<float> nums;
        bool xaxis=true;
        for(auto num:split(bbox,',')){
            if(num.back()=='%'){
                num.pop_back();
                auto temp=stof(num)/100;
                if(xaxis)
                    nums.push_back(scene_bounds.x*temp);
                else
                    nums.push_back(scene_bounds.y*temp);
            }else
                nums.push_back(stof(num));
        }
        vec4 bbox_struct;
        bbox_struct.x=nums[0];
        bbox_struct.y=nums[1];
        bbox_struct.w=nums[2];
        bbox_struct.z=nums[3];

        scene->add_source(source,&bbox_struct);
    }

    Scene* get_scene_from_string(string scene_name,string scene_items_string){
        sregex_iterator itr2;
        SceneParams sceneParams={
            .name=scene_name
        };
        Scene * scene=new Scene(&sceneParams);
        for(auto source_string:split(scene_items_string,';')){
            get_scene_source_from_srting(source_string,scene);
        }
        return scene;
    }

    public:

    void parse_input_file()
    {
        // unordered_map<string, Source *> RootItems;
        string input_strings[]={inp};
        smatch sm;
        for (auto input_string : input_strings){
            
            if(regex_match(input_string,sm,nested_source_pat)){
                auto temp=get_scene_from_string(sm.str(1),sm.str(2));
                // scenes[temp->getSceneName()]=temp;
            }else{
                auto temp=get_source_from_srting(input_string);
                // sources[temp->getSourceName()]=temp;
            }
        }

    }

};


#endif //__UTILS_HPP__