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
    
    while (running)
    {
        realTime = glfwGetTime();
        
        while (simulationTime < realTime)
        {
            simulationTime += 0.016;
            SCENEMANAGER.update(0.016);
        }
        
        RENDER.beginScene();
        RENDER.clear();

        SCENEMANAGER.render();
        
        RENDER.endScene();
        
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{
    APPLICATION.initialize();
    APPLICATION.setMainLoopFunction(mainloop);
    return APPLICATION.run();
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain2 (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main2(int argc, const char * argv[])
#endif
{
    AutoMemoryTag memLog;
	char   title[150];
    
    int running = GL_TRUE;
    
    // Render's initialization will also initialize glew and glfw
    // before that, system io might not work properly
    if (RENDER.initialize(1024, 768) != HG_OK)
    {
        exit(EXIT_FAILURE);
    }
    
    //test();
	OBJECTMANAGER.initialize();
    
    SCENEMANAGER.initialize();
    
    RENDER.setClearColor(0x808080FF);

    // Map
    MapData::GenerateTileMask();
    MapData * mapData = new MapData();
    if (!mapData->loadFromFile("map.dat"))
    {
        mapData->createMap(16, 16);
    }
    mapData->setTexture(RENDER.textureLoad("map.png"));
    
    PhysicalObject * collider = static_cast<PhysicalObject *>(OBJECTMANAGER.queryComponent(Hash("1stObject"), COMP_PHYSICAL_OBJECT));
    if (collider != NULL)
    {
        collider->setMap(mapData);
    }
    //*
    SPRITEANIMATIONCOMPILER.initialize();
    HG_ERROR err = HG_OK;
    sprite_frames_header_t* anime = SPRITEANIMATIONCOMPILER.parseXML("animation.xml", &err);
    
    SceneNode *node = SCENEMANAGER.getNode("shitNode");
    QuadEntity *testAnimator = new QuadEntity();
    SCENEMANAGER.getRoot()->addChild(node);
    node->attachEntity(testAnimator);
    testAnimator->setTexture(RENDER.textureLoad("map.png"));
    testAnimator->setTextureRect(0, 0, 32, 32);
    node->setXY(400, 300);
    testAnimator->initSpriteAnimation(anime);
    testAnimator->playSpriteAnimation(Hash("1stAnimation"));
    
    //*/
    FTGLTextureFont font("source_code_pro.ttf");
	if (font.Error())
		HGLog("shit happens: %d \n", font.Error());

	/* Set the font size and render a small text. */
	font.FaceSize(14);

    // Console
    INPUTMANAGER.initialize();
    CONSOLE.initialize();
    SCRIPTMANAGER.initialize();
    SCRIPTMANAGER.execScript("script.lua");
    SCRIPTMANAGER.engineInvocation("hg_begin");
    // Main loop
    double frametime = 0.0;
    double delay = 0.0;
    while( running )
    {
		double time = glfwGetTime();
        
		// Calculate and display the FPS
        if (RENDER.calculateFPS() != 0.0)
        {
            sprintf(title, "Hourglass Engine v0.4 (%.1f FPS)", RENDER.getFPS());
			glfwSetWindowTitle(title);
        }
        
        mapData->update();
        
        RENDER.beginScene();
		RENDER.clear();

        mapData->debug_draw(0, 0);
		collider->debug_draw();
        
        delay += frametime;
        if (delay > frametime * 0)
		{
            collider->update(frametime);
            delay = 0.0;
        }
        
        SCENEMANAGER.update(frametime);
        SCENEMANAGER.render();
        
        RENDER.renderText(0, 0, 0x000000FF, "FPS: %.1f", RENDER.getFPS());
        
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(10, 200, 1.0f);
        //glRotatef(45, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        //font.Render("English中文2009国家недели 1956работы.国家知識1729.産権局Nationalのオフィスの下に設定します。");
        glPopMatrix();
        
        CONSOLE.render(frametime);
        
		RENDER.endScene();
        
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
        
        // Debug
        static bool isF1triggered = false;
        
        if (glfwGetKey(GLFW_KEY_F1) == GLFW_RELEASE && isF1triggered)
        {
            bool debug = RENDER.isDebug();
			RENDER.setDebug(!debug);
            isF1triggered = false;
        }
        else if (glfwGetKey(GLFW_KEY_F1) == GLFW_PRESS)
        {
            isF1triggered = true;
        }
        //
        

		frametime = glfwGetTime() - time;
#if PLATFORM == PLATFORM_WINDOWS
		/*
		if (frametime < 1/61.0)
			glfwSleep(1/61.0 - frametime);
		*/
#endif
		
    }
    
    SCRIPTMANAGER.engineInvocation("hg_end");
    
    delete testAnimator;
    SCRIPTMANAGER.deInitialize();
    SPRITEANIMATIONCOMPILER.deInitialize();
    // Close window and terminate GLFW
    RENDER.deInitialize();
    
    mapData->saveToFile("map.dat");
    delete mapData;

	OBJECTMANAGER.deInitialize();
    SCENEMANAGER.deInitialize();
    
    CONSOLE.deInitialze();
    INPUTMANAGER.deInitialize();
    
    return 0;
}