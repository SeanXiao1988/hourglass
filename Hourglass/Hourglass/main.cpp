#include "HG.h"
#include "lua.hpp"
using namespace HG;

typedef std::map<uint32_t, Recti> RectMap;
#define RECT_COUNT 100

void mainloop()
{
    bool running = true;
    double simulationTime = glfwGetTime();
    double realTime = 0.0;
    
    double timeScale = 0.5;
    
    while (running)
    {
        realTime = glfwGetTime();
        
        while (simulationTime/timeScale < realTime)
        {
            simulationTime += 0.016;
            SCENEMANAGER.update(0.016);
        }
        
        RENDER.beginScene();
        RENDER.clear();

        SCENEMANAGER.render();
        
        CONSOLE.render(0.016);
        
        RENDER.endScene();
        
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain2 (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{
    APPLICATION.initialize();
    APPLICATION.setMainLoopFunction(mainloop);
    return APPLICATION.run();
}

#define BUFFER_SIZE 32768
using namespace std;

bool LoadOGG(char *name, vector<char> &buffer, ALenum &format, ALsizei &freq)  
{  
	int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian  
	int bitStream;  
	long bytes;  
	char array[BUFFER_SIZE];                // Local fixed size array  
	FILE *f;  

	f = fopen(name, "rb");  

	if (f == NULL)  
		return false;   

	vorbis_info *pInfo;  
	OggVorbis_File oggFile;  

	// Try opening the given file  
	if (ov_open(f, &oggFile, NULL, 0) != 0)  
		return false;   

	pInfo = ov_info(&oggFile, -1);  

	if (pInfo->channels == 1)  
		format = AL_FORMAT_MONO16;  
	else  
		format = AL_FORMAT_STEREO16;  

	freq = pInfo->rate;  

	do  
	{   
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);  

		if (bytes < 0)  
		{  
			ov_clear(&oggFile);  
			cerr << "Error decoding " << name << "..." << endl;  
			exit(-1);  
		}  

		buffer.insert(buffer.end(), array, array + bytes);  
	}  
	while (bytes > 0);  

	ov_clear(&oggFile);  
	return true;   
}  

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main2(int argc, const char * argv[])
#endif
{
	APPLICATION.initialize();
	APPLICATION.setMainLoopFunction(mainloop);

	ALCdevice* pDevice;  
	ALCcontext* pContext;  

	ALint state;                            // The state of the sound source  
	ALuint bufferID;                        // The OpenAL sound buffer ID  
	ALuint sourceID;                        // The OpenAL sound source  
	ALenum format;                          // The sound data format  
	ALsizei freq;                           // The frequency of the sound data  
	vector<char> bufferData;                // The sound buffer data from file  

	ALCdevice *device;  
	ALCcontext *context;   

	device = alcOpenDevice(0);  
	context = alcCreateContext(device,0);  
	ALboolean initStatus = alcMakeContextCurrent(context);      

	// Create sound buffer and source  
	alGenBuffers(1, &bufferID);  
	alGenSources(1, &sourceID);  

	// Set the source and listener to the same location  
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);  
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);  

	// Load the OGG file into memory  
	LoadOGG("audio.ogg", bufferData, format, freq);  

	// Upload sound data to buffer  
	alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);  

	// Attach sound buffer to source  
	alSourcei(sourceID, AL_BUFFER, bufferID);  

	alSourcef (sourceID, AL_GAIN, 1.0 );  

	// Finally, play the sound!!!  
	alSourcePlay(sourceID);  

	do  
	{  
		// Query the state of the souce  
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);  
	}  
	while (state != AL_STOPPED);  

	// Clean up sound buffer and source  
	alDeleteBuffers(1, &bufferID);  
	alDeleteSources(1, &sourceID);  

	alcDestroyContext(context);  
	alcCloseDevice(device); 

	return APPLICATION.run();
}