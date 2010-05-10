#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <physgameengine/src/sound.h>
using namespace std;

int main(){
	PhysAudioInit();
	PhysPlayAudio();	
	PhysCloseAudio();
}


int PhysAudioInit(int audio_rate, Uint16 audio_format, int audio_channels, int audio_buffers) {
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
		};
		
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
		};
	}

Mix_Chunk *PhysLoadSound(std::string sound) {
	if(sound.empty()) {
		printf("Unable to load WAV file: %s\n", Mix_GetError());
		return 0;
		}
	Mix_Chunk *load;
	load = Mix_LoadWAV(sound.c_str());
	return load;
	}	

void PhysPlayAudio() {
	Mix_Chunk *tempsound = PhysLoadSound("PlaceSoundHere.wav");
	if(tempsound!=0) {
		channel = Mix_PlayChannel(-1, tempsound, 0);
			if(channel == -1) {
				printf("Unable to play WAV file: %s\n", Mix_GetError());
			}
		delete tempsound;
		}
	}

void PhysCloseAudio(){
	while(Mix_Playing(channel) != 0);
	Mix_CloseAudio();
	SDL_Quit();	
	}

