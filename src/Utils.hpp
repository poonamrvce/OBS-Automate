#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include<bits/stdc++.h>
#include "Scene.hpp"
#include "Source.hpp"

using namespace std;

string get_file_name(string url){
	int i=url.find_last_of('/');
	auto j=url.find_last_of('.');
	return url.substr(i+1,j-i-1);
}

list<string> split(string s){
    regex pat("[a-zA-Z0-9_./]+");
    list<string> vec;
    sregex_iterator it2;
    for(sregex_iterator it1(s.begin(),s.end(),pat);it1!=it2;it1++){
        vec.push_back((*it1).str());
    }
    return vec;
}

void input_scene(map<Scene*,list<Source*>> &scenes, istream &stream){
    cout<<"The scenes:"<<endl;
    for(auto source:Source::all_sources)
        cout<<source.first<<"-->"<<source.second.second<<endl;
    
    string input_string;
    while(true){
    
        if( !getline(stream,input_string) || input_string.size()==0 )

            if(scenes.size()==0){
                cout<<"At least one scene required!"<<endl;
                continue;
            }else
                break; 
        regex 
            scene_name_pat("-sc [^ ]+"),
            src_names("-"+Source::allSourcePattern()+" [^-]+");
        smatch match;

        
        SceneParams sceneParams = {
            .name = regex_search(input_string,match,scene_name_pat)?match.str(0).substr(4):"scene "+scenes.size()+1
        };
        LOG(INFO)<<"created Scene "<<sceneParams.name<<endl;

        Scene *scene = new Scene(&sceneParams);

        list<Source*> source_list;
        sregex_iterator itr2;
        for(sregex_iterator 
                itr1(input_string.begin(),input_string.end(),src_names);
                itr1!=itr2;
                itr1++){
            string temp=(*itr1).str();

            SourceParams sourceParams;
            sourceParams.sourceType = Source::all_sources[temp.substr(1,2)].first;
            list<string> lis=split(temp.substr(4));
            switch(sourceParams.sourceType) {

                case SourceType::LocalFile:
                    if(lis.size()>1){
                        sourceParams.name = lis.front();
                        lis.pop_front();
                    }else
                        sourceParams.name = get_file_name(lis.front());
                    sourceParams.localFile=lis.front().c_str();
                    LOG(INFO)<<"Source "<<sourceParams.name<<" localFile "<<sourceParams.localFile<<endl;
                    break;

                case SourceType::Browser:
                    if(lis.size()>1){
                        sourceParams.name = lis.front();
                        lis.pop_front();
                    }else
                        sourceParams.name = get_file_name(lis.front());
                    sourceParams.url=lis.front().c_str();
                    LOG(INFO)<<"Source "<<sourceParams.name<<" brows "<<sourceParams.url<<endl;
                    break;

                case SourceType::Color:
                    if(lis.size()>1){
                        sourceParams.name = lis.front();
                        lis.pop_front();
                    }else
                        sourceParams.name = "C"+lis.front();
                    sourceParams.color=strtoul(lis.front().c_str(),NULL,16);
                    LOG(INFO)<<"Source "<<sourceParams.name<<" color "<<hex<<sourceParams.color<<endl;
                    break;

                case SourceType::Image:
                    if(lis.size()>1){
                        sourceParams.name = lis.front();
                        lis.pop_front();
                    }else
                        sourceParams.name = get_file_name(lis.front());
                    sourceParams.localFile=lis.front().c_str();
                    LOG(INFO)<<"Source "<<sourceParams.name<<" localFile "<<sourceParams.localFile<<endl;
                    break;

                default:
                LOG(ERROR)<<"Unknown SourceType received"<<endl;
            } 

            Source *source = new Source(&sourceParams); 
            struct vec2 bounds = {1920, 1080};
            scene->AddSource(source,  &bounds);
        }
        scenes[scene]=source_list;
    }
}

#endif //__UTILS_HPP__