#include <string>
/*sound for physgame*/

/*forward refferences*/
class SDL_Quit;
class SDL_Init;
class SDL_GetError;
class Mix_Chunk;
class Mix_OpenAudio;
class Mix_LoadWAV;
class Mix_GetError;
class Mix_LoadWAV;
class Mix_PlayChannel;
class Mix_Playing;
class Mix_FreeChunk;
class Mix_CloseAudio;

int channel;

//initialize audio
int PhysAudioInit(int audio_rate = 22050, Uint16 audio_format = AUDIO_S16SYS, int audio_channels = 2, int audio_buffers = 2048);

Mix_Chunk *PhysLoadSound(std::string sound);

void PhysPlayAudio();

void PhysCloseAudio();
