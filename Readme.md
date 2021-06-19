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
{
    installing ptotoc : https://github.com/protocolbuffers/protobuf/blob/master/src/README.md
}
2. https://github.com/muesli/obs-cli

# installing obs with browser
apt install libnss3-dev
wget https://cdn-fastly.obsproject.com/downloads/cef_binary_3770_linux64.tar.bz2
tar -xjf ./cef_binary_3770_linux64.tar.bz2
git clone --recursive https://github.com/obsproject/obs-studio.git
cd obs-studio
git checkout 26.0.2
git submodule update --recursive
mkdir build && cd build

cmake -DUNIX_STRUCTURE=1 -DENABLE_PIPEWIRE=OFF -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_BROWSER=ON -DCEF_ROOT_DIR="../../cef_binary_3770_linux64" ..
ldconfig
make -j4
checkinstall --default --pkgname=obs-studio --fstrans=no --backup=no --pkgversion="$(date +%Y%m%d)-git" --deldoc=yes


# docker dns problem
sudo apt-get install bridge-utils
sudo pkill docker
sudo iptables -t nat -F
sudo ifconfig docker0 down
sudo brctl delbr docker0
sudo service docker restart

# rtmp
/usr/local/nginx/conf/nginx.conf
have to set "record"
/usr/local/nginx/sbin/nginx -t
for testing


# docker with sound

docker run -it -v  /tmp/.X11-unix:/tmp/.X11-unix  -e  DISPLAY=$DISPLAY -h ubuntu --device /dev/snd -e PULSE_SERVER=unix:${XDG_RUNTIME_DIR}/pulse/native  -v ${XDG_RUNTIME_DIR}/pulse/native:${XDG_RUNTIME_DIR}/pulse/native  -v `pwd`:/vol/ ubuntu_obs:04.21  bash

# git push
git push <remote-name> <local-branch-name>:<remote-branch-name>
git push official master:automate-input