## OBS automation using command line

The intent of this project is to implement command line tools to be able to automate OBS usage. Some initial goals are as follows:

1. Setup OBS client server applications
2. Use the client application to do the following:
    - Add a file source and output to another file
    - Render two files one followed by the other to a concatenated output file
    - Play a file and then switch to another file even when the first file playing. Sink is again another file.
    - Overlay graphics on a media file.

## Getting started with OBS experimentation

1. Use the [dockerfile](https://github.com/shashi-banger/usefull_dockers/blob/master/obs-studio/Dockerfile) to create obs-base image
2. Clone the [obs-studio](https://github.com/obsproject/obs-studio.git) repository in home directory
3. Run the following sequence of commands to start the obs-base docker container
```
xhost +local:
docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -h $HOSTNAME  -v `pwd`:/data/obs-studio/  obs-base:1.0.0 bash
```
4. Once inside the docker run the following to build the obs studio. [Reference](https://github.com/obsproject/obs-studio/wiki/install-instructions#debian-based-build-directions)
```
cd /data/obs-studio/
mkdir build && cd build
cmake -DUNIX_STRUCTURE=1 -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
make install
# Now obs can be run from with the docker
obs
```

## Running applications ins examples

1. Here are a sequence of commad to run sampleapplication
```
docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -h $HOSTNAME  -v `pwd`:/data/obs-studio/  -v /home/shashi/sb_media/:/data/sb_media/  -v /home/shashi/sb_work/GithubRepos/obs-automate/:/data/obs_automate shashibanger/obs-studio-base:1.0.0 bash

# Inside docker
cd /data/obs-studio/
mkdir build
cd build
cmake -DUNIX_STRUCTURE=1 -DCMAKE_INSTALL_PREFIX=/usr ..
make -j 4 
make install
cd /data/obs_automate/build
cmake ../
make
# The following application reads a file from /data/sb_media/ and decodes and creates an output file in /root/out.mkv
./simple_obs_app
```

## References

1. https://github.com/a-rose/obs-headless
2. https://github.com/muesli/obs-cli