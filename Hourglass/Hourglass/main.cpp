// TODO:
// rotation with y/x axis in orthographic projection
// using glm frustum to achieve that effect then do normal matrix tricks

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
    
    GLuint rt = RENDER.rtargetCreate(800, 600);
    Quad q;
    quad_set_default(&q);
    q.tex = RENDER.rtargetGetTexture(rt);
    quad_set_texture_rect(&q, 0, 0, 800, 600, 800, 600);
    quad_set_coord(&q, 0, 0, 800, 600);
    
    SceneNode* node = SCENEMANAGER.createNode("2ndNode");//("test");
    SCENEMANAGER.getRoot()->addChild(node);
    
    ParticleEmitter* emitter = new ParticleEmitter();
    emitter->setTotalParticles(200);
    emitter->setParticlePositionType(PPositionTypeRelative);
    emitter->setTexture(RENDER.textureLoad("test.png"));
    emitter->setTextureRect(0, 0, 32, 32);
    
    emitter->setEmitterMode(EMITTER_GRAVITY);
    emitter->getEmitterModeGravity()->gravity = Point2f(0.0f, 0.0f);
    emitter->getEmitterModeGravity()->speed = 60;
    emitter->getEmitterModeGravity()->speedVar = 20;
    emitter->setAngle(90);
    emitter->setAngleVar(10);
    emitter->setPositionVar(Point2f(40.0f, 20.0f));
    emitter->setLifeTime(3.0f);
    emitter->setLifeTimeVar(0.25f);
    emitter->setStartSize(64);
    emitter->setStartSizeVar(10);
    emitter->setStartColor(color4f(0.76f, 0.25f, 0.12f, 0.1f));
    emitter->setEndColor(color4f(1.0f, 1.0f, 1.0f, 0.2f));
    
    emitter->setDuration(-1.0f);
    emitter->setEmissionRate(40.0f);
    node->attachEntity(emitter);
    emitter->fireEmitter();
    
    SceneNode* node3 = SCENEMANAGER.createNode("3rdNode");
    SCENEMANAGER.getRoot()->addChild(node3);
    QuadEntity* entity = new QuadEntity();
    entity->setTexture(RENDER.textureLoad("test.png"));
    entity->setTextureRect(0, 0, 256, 256);
    entity->setWidth(256);
    entity->setHeight(256);
    node3->attachEntity(entity);
    node3->setXY(200, 200);
    
    while (running)
    {
        realTime = glfwGetTime();
        
        while (simulationTime/timeScale < realTime)
        {
            simulationTime += 0.016;
            SCENEMANAGER.update(0.016);
        }
        
        RENDER.calculateFPS();
        static float fpstimer = 0.0f;
        fpstimer += 0.016;
        if (fpstimer > 1.0f)
        {
            fpstimer = 0.0f;
            printf("%.1f\n", RENDER.getFPS());
        }
        
        //*/ render normal stuff
        RENDER.beginScene(0);
        RENDER.clear();
        
        SCENEMANAGER.render();
        
        CONSOLE.render(0.016f);
        
        RENDER.endScene();
        //*/

        /* render screen ,GUI and console
        RENDER.beginScene();
        RENDER.clear();
        
        RENDER.renderQuad(&q);
        
        CONSOLE.render(0.016);
        
        RENDER.endScene();
        //*/
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
    
    delete entity;
    delete emitter;
    delete node;
    delete node3;
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{
    APPLICATION.initialize();
    APPLICATION.setMainLoopFunction(mainloop);
    return APPLICATION.run();
}