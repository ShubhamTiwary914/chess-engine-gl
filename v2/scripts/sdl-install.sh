#!/usr/bin/bash

#SDL2 for rendering
setup_sdl2(){
    git clone https://github.com/libsdl-org/SDL.git -b SDL2
    cd SDL
    mkdir build
    cd build
    ../configure   
    make 
    cd ../..
    rm -r ./SDL
}

setup(){
    # sdl not installed?
    if [[ -z $(command -v sdl2-config) ]]; then
        setup_sdl2
    fi
    if [[ -d "./SDL" ]]; then
        sudo rm -r ./SDL   
    fi
}


if [[ $(uname) == "Linux" ]]; then
    setup
else
    echo "Not a linux machine, run setup-<os> command for your OS type"
fi