#include <bits/stdc++.h>
#include "Utils.hpp"
#include "ObsUtils.hpp"


using namespace std;

int main(int argc, char *argv[]) {
    
    if(argc==1){
        LOG(ERROR)<<"input file required";
    }

    ObsInit();
    
    auto input_file=string(argv[1]);
    
    auto show=InputParser().parse_input_file(input_file);

    show.play_show_items();

}