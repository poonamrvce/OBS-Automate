#include "../src/Logging.hpp"
#include <bits/stdc++.h>

using namespace std;
string get_file_name(string url){
	int i=url.find_last_of('/');
	auto j=url.find_last_of('.');
	return url.substr(i+1,j-i-1);
}

list<string> split(string s){
    regex pat("[^ ]+");
    list<string> vec;
    sregex_iterator it2;
    sregex_iterator it1(s.begin(),s.end(),pat);
    smatch mat;
    regex_search(s,mat,pat);
    cout<<"s="<<s<<" size ="<<mat.size()<<endl;
    for(;it1!=it2;it1++){
        vec.push_back((*it1).str());
    }
    return vec;
}
enum class SourceType {
	InvalidType = -1,
	LocalFile = 0,
	RTMP,
    Browser,
    Color,
    Image
};
unordered_map<string,pair<SourceType,string>> all_sources={
	{"im",{SourceType::Image,"Image"}},
	{"br",{SourceType::Browser,"Browser"}},
	{"co",{SourceType::Color,"Color"}},
	{"vi",{SourceType::LocalFile,"Video"}},
	{"rt",{SourceType::RTMP,"RTMP"}},
};
string allSourcePattern(){
	string s="(";
	for(auto i=all_sources.begin();i!=all_sources.end();i++){
		s.append((*i).first);
		s.append("|");
	}
	s.pop_back();
	s.append(")");
	return s;
}

//regex definitions
#define wsp "[ ]*"
#define keyword "image|color|video|text|browser"
#define identifier "[0-9a-zA-Z_]+"
#define digit "[0-9]"
#define decimal digit "+(\." digit "+)?"
#define time decimal "sec|" decimal "min"

regex nested_source_pat( wsp "(" identifier ")?" wsp "[(](.*)[)]" wsp "$" );
regex sources_list_pat("[^;]+");
regex source_pat( wsp "(" identifier ")?" wsp "(" keyword ")" "(.*)" );
regex options_pat("[^ ]+");

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

void get_source_from_srting(string source_string){
    smatch sm;
    regex_match (source_string,sm,source_pat);
    string options_string=sm.str(3);
    list<string> options;
    sregex_iterator b;
    for(auto a=sregex_iterator(options_string.begin(),options_string.end(),options_pat);a!=b;a++)
        options.push_back((*a).str());

    int i=0;
    cout<<"source_string_options "<<endl;
    for(auto s:options)
        cout<<i++<<':'<<s<<':'<<endl;
    cout<<endl;
    float x=1980,y=960;
    options.pop_front();
    if(!options.empty()){
        string bbox=options.front().substr(1,options.front().size()-2);
        vector<float> nums;
        bool xaxis=true;
        for(auto num:split(bbox,',')){
            if(num.back()=='%'){
                num.pop_back();
                auto temp=stof(num)/100;
                if(xaxis)
                    nums.push_back(x*temp);
                else
                    nums.push_back(y*temp);
            }else
                nums.push_back(stof(num));
        }
        cout<<"x:"<<nums[0]<<endl;
        cout<<"y:"<<nums[1]<<endl;
        cout<<"w:"<<nums[2]<<endl;
        cout<<"z:"<<nums[3]<<endl;
    }
}

void get_scene_from_string(string scene_name,string scene_items_string){
    sregex_iterator itr2;
    // map<string, s
    for(sregex_iterator 
        itr1(scene_items_string.begin(),scene_items_string.end(),sources_list_pat);
        itr1!=itr2;
        itr1++){
        get_source_from_srting((*itr1).str());
        
    }
}

void parse_input_file()
{
    // unordered_map<string, Source *> RootItems;
    string inp="A ( image1 image /path/to/img/ (0,9,8,7) ;  color1 color 0x672632 (0,0,33%,33%) ; video1 video /path/jsk/ldkmkd )";
    string input_strings[]={inp};
    map<string, string> scenes;
    map<string, string> sources;
    smatch sm;
    for (auto input_string : input_strings){
        
        if(regex_match(input_string,sm,nested_source_pat)){
            get_scene_from_string(sm.str(1),sm.str(2));
            // scenes[temp->getSceneName()]=temp;
        }else{
            // auto temp=get_source_from_srting(input_string);
            // sources[temp->getSourceName()]=temp;
        }
    }
}



int main(){
    
    // int maxl=100;
    //     char buf[maxl];
    //     // fgets(buf,100,stdin);
    //     // string input_string=string(buf);
    //     string input_string="-sc sc1 -im img1 /vol/sample/raama.jpg -co 0x13131313";
        
    //     regex 
    //         scene_names("-sc [^ ]+"),
    //         src_names("-"+allSourcePattern()+" [^-]+");
    //     smatch match;
    //     string name;
    //     if(regex_search(input_string,match,scene_names))
    //     name = match.str(0);
    //     else name="scene "+1;

    //     sregex_iterator itr2;
    //     for(sregex_iterator 
    //             itr1(input_string.begin(),input_string.end(),src_names);
    //             itr1!=itr2;
    //             itr1++){
    //         string temp=(*itr1).str();
    //         LOG(INFO)<<temp.substr(1,2)<<endl;
    //         string src_type=temp.substr(1,2);
    //         LOG(INFO)<<temp.substr(4)<<endl;
    //         list<string> lis=split(temp.substr(4));
    //         string url,localFile;
    //         int color;
    //         switch(all_sources[src_type].first) {

    //             case SourceType::LocalFile:
    //                 if(lis.size()>1){
    //                     name = lis.front();
    //                     lis.pop_front();
    //                 }else
    //                     name = get_file_name(lis.front());
    //                 localFile=lis.front().c_str();
    //                 LOG(INFO)<<"Source "<<name<<" localFile "<<localFile<<endl;
    //                 break;

    //             case SourceType::Browser:
    //                 if(lis.size()>1){
    //                     name = lis.front();
    //                     lis.pop_front();
    //                 }else
    //                     name = get_file_name(lis.front());
    //                 url=lis.front().c_str();
    //                 LOG(INFO)<<"Source "<<name<<" brows "<<url<<endl;
    //                 break;

    //             case SourceType::Color:
    //                 if(lis.size()>1){
    //                     name = lis.front();
    //                     lis.pop_front();
    //                 }else
    //                     name = "C"+lis.front();
    //                 color=strtoul(lis.front().c_str(),NULL,16);
    //                 LOG(INFO)<<"Source "<<name<<" color "<<hex<<color<<endl;
    //                 break;

    //             case SourceType::Image:
    //                 if(lis.size()>1){
    //                     name = lis.front();
    //                     lis.pop_front();
    //                 }else
    //                     name = get_file_name(lis.front());
    //                 localFile=lis.front().c_str();
    //                 LOG(INFO)<<"Source "<<name<<" localFile "<<localFile<<endl;
    //                 break;

    //             default:
    //             LOG(ERROR)<<"Unknown SourceType received"<<endl;
    //         }
    //     }

    // parse_input_file();
    regex item_pat( wsp "(" identifier ")" " " "(" time ")?" wsp "(.*)" );
    regex show_scene_items_pat( "(" "(" identifier ")" " " "(" time ")?:(" time ")" wsp ")" );
    string str1="A 10.3min color1 3sec:5sec video1 7sec:9sec";
    string str2="B 12sec";
    smatch sm;
    cout<<regex_match(str1,sm,item_pat)<<endl;
    for(int i=0;i<sm.size();i++)
        cout<<i<<": "<<sm.str(i)<<endl;
    if(!sm.str(5).empty()){
        auto text=sm.str(5);
        sregex_iterator b;
        for(auto a=sregex_iterator(text.begin(),text.end(),show_scene_items_pat);a!=b;a++)
            cout<<((*a).str())<<endl;
    }
    
    cout<<regex_match(str2,sm,item_pat)<<endl;
    for(int i=0;i<sm.size();i++)
        cout<<i<<": "<<sm.str(i)<<endl;
    // cout<<str1.substr(0,str1.size()-3);
    
}

