#include <bits/stdc++.h>
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

class InputParser
{
private:
    regex nested_source_pat, source_pat, show_item_pat, show_scene_items_pat;
    string inp = "A ( image1 image /path/to/img/ (a,b,c,d) ;  color1 color 0x672632 (0,0,%33,%33) ; video1 video /path/jsk/ldkmkd )";
    map<string, Source *> sources;
    map<string, Scene *> scenes;
    map<string, int> count;
    list<ShowItem> global_items;

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

    void get_scene_source_from_string(string source_string, Scene *scene)
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

    list<ShowItem> get_show_items_from_string(string item_string)
    {
        list<ShowItem> items;
        ShowItem item;

        smatch sm;
        regex_match(item_string, sm, show_item_pat);
        
        if (!sm.str(5).empty())
        { // A 10sec color2 :5sec video1 7sec:9sec
            item.duration=get_time(sm.str(2));
            item.scene=scenes[sm.str(1)];
            item.itemType=ShowItemType::Scene;
            items.push_back(item);

            if(!sm.str(5).empty()){
                auto text=sm.str(5);
                sregex_iterator b;
                for(auto a=sregex_iterator(text.begin(),text.end(),show_scene_items_pat);a!=b;a++){
                    string str=(*a).str();
                    auto lis=split(str,' ');
                    item.itemType=ShowItemType::SceneItem;
                    item.source=items.front().scene->get_source(lis.front());
                    lis.pop_front();
                    lis=split(lis.front(),':');
                    item.start=get_time(lis.front());
                    item.duration=get_time(lis.back());
                    items.push_back(item);
                }
            }

            sceneName = ;
            if (scenes.count(sceneName) == 0)
            {
                break;
            }
            sceneTime = ; // A color1 10sec
            if (not there)
            {
                scenetime = 99999;
            }
            // mp["color1"] = <"","">
            convert();
            vector<pair<int, pair<string, bool>>> arr;
            arr.sort();

            for (int i = 0; i < arr.size(); i++)
            {
                if (scenes[sceneName]._sources.count(arr[i].second.first) == 0)
                {
                    cout << "ERROR";
                    exit(0);
                }
                else
                {
                    sourcename = arr[i].second.first;
                    sourcetime = arr[i].first;
                    status = arr[i].second.second;
                }
            }
            else if (regex_match(input_string, Source))
            { // B 90sec // C 20sec // A 10sec
                sourceName = ;
                sourceTime = ;

                if (sources.count(sourceName) == 0)
                {
                    // source
                }
                else if (scenes.count(sourceName) == 0)
                {
                    // scene
                }
                else
                {
                    continue;
                }
            }
        }

    public:
        void parse_input_file(string file_name)
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
        }

        ~InputParser()
        {
        }
    };