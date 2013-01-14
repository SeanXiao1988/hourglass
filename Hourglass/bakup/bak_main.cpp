//
//  main.cpp
//  OpenGL
//
//  Created by xiaolingxi on 12-8-8.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include <math.h>
#include <iostream>

#include "lodepng.h"

#include "HGSystem.h"
#include "HGSceneNode.h"
#include "HGSceneManager.h"
#include "HGQuadEntity.h"
#include "HGRender.h"
#include "HGData.h"
#include "HGDistortionMesh.h"

#include "HGPhysicsManager.h"
#include "HGPhysicalObject.h"

#include "HGMapData.h"
#include "HGMapCollider.h"

#include "HGEventManager.h"
#include "HGIEventListener.h"

#include "HGInputManager.h"

using namespace HG;

//*/
const int nRows=32;
const int nCols=32;
const float cellw=512.0f/(nCols-1);
const float cellh=512.0f/(nRows-1);
const float meshx=144;
const float meshy=44;
//*/

void test_distortion_mesh()
{
    // http://relishgames.com/forum/index.php?p=/discussion/1475/distortion-mesh-tutorial/p1
    DistortionMesh* mesh = new DistortionMesh(nCols, nRows);
    mesh->setTexture(RENDER.textureLoad("test2.png"));
    mesh->setTextureRect(0, 0, 512, 512);
    
    //variables defined in the haff tutorial that I am using.
    float dt = 0.0f;//(float)delta / 100.0f;
    static float t = 0.0f;
    int i, j;
    t+=dt;
    
    //new variables declared and defined, see text at the end for some explanations on how
    //you can change them to modify the effect.
    float c1=0.07,c2=70.0,c3=10,xcenter=256,ycenter=256;
    float distort=0, distortx=0, distorty=0,
    distance=0, distancex=0, distancey=0;
    
    //xcenter = node2->getWorldX() - 144;
    //ycenter = node2->getWorldY() - 44;
    
    for(i=0;i<nRows;i++)
    {
        for(j=0;j<nCols;j++)
        {
            
            /*the receding sin wave function that makes up a ripple requires both distance from the center
             and time, so first calculating the distance */
            
            distancex=(j*cellw)-xcenter;
            distancey=(i*cellh)-ycenter;
            distance=sqrtf(pow(distancex,2)+pow(distancey,2));
            
            /*
             here is a calculation for the height of the sin wave at a particular time
             at a particular distance.  The first half makes the wave fade with distance
             from the center
             the second half is the sin wave itself.  Again, the constants here will be described later
             I use this distort value to move the node towards or away from the center
             of the ripple.  Positive away, negative towards.
             it makes a pretty good ripple effect through what are essentially compression waves.
             */
            
            
            distort=(c2/sqrtf(distance))*sinf((distance*c1)-(t*c3));
            
            /*
             since distort can be both positive and negative, and is stronger at the source
             there are problems with distoring nearby nodes beyond the center
             these three lines fix that problem for both positive and negative distort values.
             */
            
            if(abs(distort)>distance) {
                if(distort>0) distort=distance;
                if(distort<0) distort=-distance;
            }
            
            /*breaking the distortion into x and y.  I already had the x and y distance.
             the x and y distortion is proportional to that, so this is just a simple 
             proportion (distortx/distort)=(distancex/distance) solved for distortx.
             */
            
            distortx=distancex/distance*distort;
            distorty=distancey/distance*distort;
            //finally all done, now just set the mesh.
            mesh->setDisplacement(j,i,distortx,distorty,DISTORTION_NODE);
        }
    }
    
    mesh->render(400 - 256, 300 - 256);
    
    RENDER.textureFree(mesh->getTexture());
    delete mesh;
}

void test_physics_mgr()
{
    PHYSICSMANAGER.initialize(800, 600);
    
    PHYSICSMANAGER.setFriction(0.0f);
    PHYSICSMANAGER.setRestitution(0.0f);
    PHYSICSMANAGER.setGlue(0.0f);
    
    PHYSICSMANAGER.createBoxObject("platform", Vector(300.0f, 100.0f), 0.0f, 200.0f, 25.0f);
    //PhysicalObject * player = PHYSICSMANAGER.createBoxObject("player", Vector(650, 50), 1.0f, 100.0f, 100.0f);
    PHYSICSMANAGER.createTriangleObject("terrain", Vector(800, 500), 0.0f, Vector(0, 0), Vector(0, 100), Vector(-800, 100));
    
    PHYSICSMANAGER.update(0.0016f);
    PHYSICSMANAGER.render();
    
    PHYSICSMANAGER.deInitialize();
}

class TestEventListener : public IEventListener
{
public:
    TestEventListener(){}
    ~TestEventListener(){}
    
    EventResult HandleEvent(const Event& event)
    {
        InputInfo *info = (InputInfo *)event.getEventData();
        
        if (info->action == INPUT_TRIGGER)
        {
            const char* ass = (const char *)event.getEventData();
            HGLog("%p char[%s] received\n", this, ass);
        }
        else if (info->action == INPUT_MOUSE_MOVE)
        {
            const int *pos = (const int *)event.getEventData();
            HGLog("%p pos[%d|%d] received\n", this, pos[0], pos[1]);
        }

        return EVENT_RESULT_PROCESSED;
    }
};

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain2 (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main2(int argc, const char * argv[])
#endif
{
    AutoMemoryTag memLog;
	int    frames;
	double fps, time, t0;
	char   title[150];
    
	frames = 0;
	t0 = glfwGetTime();
    
    int running = GL_TRUE;
    
    if (RENDER.initialize(800, 600) != HG_OK)
    {
        exit(EXIT_FAILURE);
    }
    
    RENDER.setClearColor(0x808080FF);
    
    double delta = 0.0;
    
    // Map
    MapData * mapData = new MapData();
    if (!mapData->loadFromFile("map.dat"))
    {
        mapData->createMap(16, 16);
    }
    mapData->setTexture(RENDER.textureLoad("map.png"));

	MapCollider * collider = new MapCollider();
	collider->getMapCollider()->upper_slope_climber.x = 32;
	collider->getMapCollider()->upper_slope_climber.y = 0;
	collider->getMapCollider()->lower_slope_climber.x = 32;
	collider->getMapCollider()->lower_slope_climber.y = 60;

	collider->setMap(mapData);

    // Main loop
    while( running )
    {
		time = glfwGetTime();
		// Calculate and display the FPS
		if((time-t0) > 1.0 || frames == 0)
		{
			fps = (double)frames / (time-t0);
            delta = time - t0;
			sprintf(title, "Hourglass Engine v0.3 (%.1f FPS)", fps);
			glfwSetWindowTitle(title);
			t0 = time;
			frames = 0;
		}
		frames ++;
        
        mapData->update();
        
        RENDER.beginScene(/*rtag*/);
		RENDER.clear();
        
        mapData->debug_draw(0, 0);
		collider->debug_draw();
        
		RENDER.endScene();

		/*
		if (collider->getMapCollider()->status == STATUS_ON_GROUND || collider->getMapCollider()->status == STATUS_ON_SLOPE)
		{
			if (collider->getMapCollider()->lower_slope_climber.x < 15*TILE_SIZE)
			
			collider->getMapCollider()->upper_slope_climber.x++;
			collider->getMapCollider()->upper_slope_climber.y = 0;
			collider->getMapCollider()->lower_slope_climber.x++;
			collider->getMapCollider()->lower_slope_climber.y = 60;
		}
        */
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
        
		if (glfwGetKey(GLFW_KEY_F1))
		{
			bool debug = RENDER.isDebug();
			RENDER.setDebug(!debug);
		}
    }
    
    // Close window and terminate GLFW
    RENDER.deInitialize();
    
    mapData->saveToFile("map.dat");
    delete mapData;
    delete collider;
    
    return 0;
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain3 (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main3(int argc, const char * argv[])
#endif
{
    AutoMemoryTag memLog;
	int    frames;
	double fps, time, t0;
	char   title[150];

	frames = 0;
	t0 = glfwGetTime();
	fps = 0;

    int running = GL_TRUE;

    if (RENDER.initialize(800, 600) != HG_OK)
    {
        exit(EXIT_FAILURE);
    }

    SCENEMANAGER.initialize();
    SceneNode * node = SCENEMANAGER.createNode("node");
    node->setXY(400, 300);
    
    SCENEMANAGER.getRoot()->addChild(node);
    
    SceneNode * node1 = SCENEMANAGER.createNode("node1");
    node1->setXY(100, 100);
    node->addChild(node1);
    
    SceneNode * node2 = SCENEMANAGER.createNode("node2");
    node2->setXY(150, 150);
    node1->addChild(node2);
    
    QuadEntity * entity = new QuadEntity();
    entity->setWidth(128);
    entity->setHeight(128);
    entity->setTexture(RENDER.textureLoad("test.png"));
    entity->setTextureRect(0, 0, 128, 128);
    node2->attachEntity(entity);

	QuadEntity *bg = new QuadEntity();
	bg->setWidth(512);
	bg->setHeight(512);
	bg->setTexture(RENDER.textureLoad("test2.png"));
	bg->setTextureRect(0, 0, 512, 512);
	node->attachEntity(bg);
    
    float rot1 = 0.0f;
    float rot2 = 0.0f;
    
    RENDER.setClearColor(0x808080FF);
    
    HANDLE_RTARGET rtag = RENDER.rtargetCreate(800, 600);

	Quad quad;
	quad.blend = BLEND_DEFAULT;

	quad.v[0].u = 0.0f;
	quad.v[0].v = 0.0f;
	quad.v[1].u = 1.0f;
	quad.v[1].v = 0.0f;
	quad.v[2].u = 1.0f;
	quad.v[2].v = 1.0f;
	quad.v[3].u = 0.0f;
	quad.v[3].v = 1.0f;

	quad.v[0].x = 0.0f;
	quad.v[0].y = 0.0f;
	quad.v[1].x = 400.0f;
	quad.v[1].y = 0.0f;
	quad.v[2].x = 400.0f;
	quad.v[2].y = 300.0f;
	quad.v[3].x = 0.0f;
	quad.v[3].y = 300.0f;

	for (int i = 0; i < 4; i++)
	{
		RENDER.setVertexColor(&quad.v[i], 0xFFFFFFFF);
		quad.v[i].z = 1.0f;
	}

    quad.tex = RENDER.rtargetGetTexture(rtag);
    quad.tex = RENDER.textureLoad("map.png");
    
	// shader
	GLint timeParam = 0;
    GLint posParam = 0;
    GLint switchParam = 0;
    
	GLuint program = 0;
    ShaderList* shader = (ShaderList *)RENDER.shaderLoad("distortion");
	program = shader->program;
	timeParam = glGetUniformLocation(program, "time");
    posParam = glGetUniformLocation(program, "center");
    switchParam = glGetUniformLocation(program, "switcher");
    float livetime = 0.0f;

	float delta = 0.0f;
    
    RENDER.shaderActive((HANDLE_SHADER)shader);
    // Main loop
    while( running )
    {
		// Calculate and display the FPS
		time = glfwGetTime();
		if((time-t0) > 1.0 || frames == 0)
		{
			fps = (double)frames / (time-t0);
			sprintf(title, "Hourglass Engine v0.3 (%.1f FPS) %.3f", fps, delta);
			glfwSetWindowTitle(title);
			t0 = time;
			frames = 0;
		}
		frames ++;

		// shader
		fps = fps != 0 ? fps : 1;
		delta = 1.0f/fps;
		delta = delta >= 1.0 ? 0 : delta;
        
        livetime += delta;
		glUniform1f(timeParam, livetime/10);
        glUniform2f(posParam, 0.5, 0.5);

        RENDER.beginScene();//rtag);
		RENDER.clear();
        node->setRotation(rot1);
        //rot1+=0.5f;
        
        node1->setRotation(rot2);
        //rot2-=0.8f;
        
        glUniform1i(switchParam, 1);
        //RENDER.shaderActive((HANDLE_RTARGET)shader);
		SCENEMANAGER.run(delta);
        //RENDER.shaderDeActive();
        glUniform1i(switchParam, 0);
        RENDER.renderQuad(&quad);

		RENDER.endScene();

		int mouseX;
		int mouseY;
		glfwGetMousePos(&mouseX, &mouseY);
		node1->setWorldX(mouseX);
		node1->setWorldY(mouseY);
        //RENDER.setClipping();
        /*
		RENDER.beginScene();
        RENDER.clear();
        RENDER.renderQuad(&quad);
		RENDER.endScene();
        //*/

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );

        static bool isF1Pressed = false;
        
        if (glfwGetKey(GLFW_KEY_F1) == GLFW_RELEASE && isF1Pressed)
        {
            bool debug = RENDER.isDebug();
			RENDER.setDebug(!debug);
            //QueryMemoryUsage(1);
            
            isF1Pressed = false;
        }
        else if (glfwGetKey(GLFW_KEY_F1) == GLFW_PRESS)
        {
            isF1Pressed = true;
        }
    }

    // Close window and terminate GLFW
    RENDER.deInitialize();
    
	delete bg;
    delete entity;
    
    SCENEMANAGER.deInitialize();
    
    return 0;
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{
    AutoMemoryTag memLog;
	int    frames;
	double fps, time, t0;
	char   title[150];
    
	frames = 0;
	t0 = glfwGetTime();
	fps = 0;
    
    int running = GL_TRUE;
    
    if (RENDER.initialize(800, 600) != HG_OK)
    {
        exit(EXIT_FAILURE);
    }
    
    EVENTMANAGER.initialize();
	INPUTMANAGER.initialize();

	TestEventListener *dummy = new TestEventListener();
	EVENTMANAGER.addListener(dummy, EVENT_INPUT);
    
    SCENEMANAGER.initialize();

    SceneNode * node = SCENEMANAGER.createNode("node");
    node->setXY(400, 300);
    
    SCENEMANAGER.getRoot()->addChild(node);
    
    SceneNode * node1 = SCENEMANAGER.createNode("node1");
    node1->setXY(100, 100);
    node->addChild(node1);
    
    SceneNode * node2 = SCENEMANAGER.createNode("node2");
    node2->setXY(150, 150);
    node1->addChild(node2);
    
    QuadEntity * entity = new QuadEntity();
    entity->setWidth(128);
    entity->setHeight(128);
    entity->setTexture(RENDER.textureLoad("test.png"));
    entity->setTextureRect(0, 0, 128, 128);
    node2->attachEntity(entity);
    
	QuadEntity *bg = new QuadEntity();
	bg->setWidth(512);
	bg->setHeight(512);
	bg->setTexture(RENDER.textureLoad("test2.png"));
	bg->setTextureRect(0, 0, 512, 512);
	node->attachEntity(bg);
	node->setScale(0.3, 0.3);
    
    RENDER.setClearColor(0x808080FF);
    
    GLint timeParam = 0;
	GLuint program = 0;
    ShaderList* shader = (ShaderList *)RENDER.shaderLoad("shader");
	program = shader->program;
    
    timeParam = glGetUniformLocation(program, "time");
    
	float delta = 0.0f;
    float livetime = 1.0f;
    
    RENDER.shaderActive((HANDLE_SHADER)shader);

	Quad quad;
	quad.blend = BLEND_DEFAULT;

	quad.v[0].u = 0.0f;
	quad.v[0].v = 0.0f;
	quad.v[1].u = 1.0f;
	quad.v[1].v = 0.0f;
	quad.v[2].u = quad.v[1].u;
	quad.v[2].v = 1.0f;
	quad.v[3].u = 0.0f;
	quad.v[3].v = quad.v[2].v;

	quad.v[0].x = 0.0f;
	quad.v[0].y = 0.0f;
	quad.v[1].x = 800.0f;
	quad.v[1].y = 0.0f;
	quad.v[2].x = 800.0f;
	quad.v[2].y = 600.0f;
	quad.v[3].x = 0.0f;
	quad.v[3].y = 600.0f;

	for (int i = 0; i < 4; i++)
	{
		RENDER.setVertexColor(&quad.v[i], 0xFFFFFFFF);
		quad.v[i].z = 1.0f;
	}

	quad.tex = RENDER.textureLoad("test2.png");
    // Main loop
    while( running )
    {
		// Calculate and display the FPS
		time = glfwGetTime();
		if((time-t0) > 1.0 || frames == 0)
		{
			fps = (double)frames / (time-t0);
			sprintf(title, "Hourglass Engine v0.3 (%.1f FPS) %.3f", fps, delta);
			glfwSetWindowTitle(title);
			t0 = time;
			frames = 0;
		}
		frames ++;
        
		// shader
		fps = fps != 0 ? fps : 1;
		delta = 1.0f/fps;
		delta = delta >= 1.0 ? 0 : delta;
        
        livetime -= delta;
		if (livetime <= 0.0f)
			livetime = 0.0f;
        
        RENDER.beginScene();
		RENDER.clear();
        
		RENDER.shaderActive((HANDLE_RTARGET)shader);
		glUniform1f(timeParam, livetime);
		RENDER.renderQuad(&quad);
		RENDER.shaderDeActive();
        //
		SCENEMANAGER.run(delta);
        //
        
		RENDER.endScene();
        
		int mouseX;
		int mouseY;
		glfwGetMousePos(&mouseX, &mouseY);
		node1->setWorldX(mouseX);
		node1->setWorldY(mouseY);
        
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
        
        static bool isF1Pressed = false;
        
        if (glfwGetKey(GLFW_KEY_F1) == GLFW_RELEASE && isF1Pressed)
        {
            bool debug = RENDER.isDebug();
			RENDER.setDebug(!debug);
            QueryMemoryUsage(1);
            
            isF1Pressed = false;
        }
        else if (glfwGetKey(GLFW_KEY_F1) == GLFW_PRESS)
        {
            isF1Pressed = true;
        }
    }
    
    // Close window and terminate GLFW
    RENDER.deInitialize();
    
	delete bg;
    delete entity;
    
    SCENEMANAGER.deInitialize();

	INPUTMANAGER.deInitialize();
	EVENTMANAGER.deInitialize();
    
    delete dummy;
    
    return 0;
}
