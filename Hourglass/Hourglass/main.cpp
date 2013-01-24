#include "HG.h"
#include "lua.hpp"

#include "freetype-gl.h"

using namespace ftgl;

// ------------------------------------------------------- typedef & struct ---
typedef struct {
    float x, y, z;    // position
    float s, t;       // texture
    float r, g, b, a; // color
} vertex_t;


// ------------------------------------------------------- global variables ---
vertex_buffer_t *buffer;


// --------------------------------------------------------------- add_text ---
void add_text( vertex_buffer_t * buffer, texture_font_t * font,
              wchar_t * text, vec4 * color, vec2 * pen )
{
    size_t i;
    float r = color->red, g = color->green, b = color->blue, a = color->alpha;
    for( i=0; i<wcslen(text); ++i )
    {
        texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
        if( glyph != NULL )
        {
            int kerning = 0;
            if( i > 0)
            {
                kerning = texture_glyph_get_kerning( glyph, text[i-1] );
            }
            pen->x += kerning;
            int x0  = (int)( pen->x + glyph->offset_x );
            int y0  = (int)( pen->y + glyph->offset_y );
            int x1  = (int)( x0 + glyph->width );
            int y1  = (int)( y0 - glyph->height );
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;
            GLuint indices[6] = {0,1,2, 0,2,3};
            vertex_t vertices[4] = { { (float)x0,(float)y0,0,  s0,t0,  r,g,b,a },
                { (float)x0,(float)y1,0,  s0,t1,  r,g,b,a },
                { (float)x1,(float)y1,0,  s1,t1,  r,g,b,a },
                { (float)x1,(float)y0,0,  s1,t0,  r,g,b,a } };
            vertex_buffer_push_back( buffer, vertices, 4, indices, 6 );
            pen->x += glyph->advance_x;
        }
    }
}



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
    
    // freetype-gl
    size_t i;
    texture_font_t *font = 0;
    texture_atlas_t *atlas = texture_atlas_new( 512, 512, 1 );
    const char * filename = "source_code_pro.ttf";
    wchar_t *text = L"A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
    buffer = vertex_buffer_new( "v3f:t2f:c4f" );
    ftgl::vec2 pen = {{5,400}};
    ftgl::vec4 black = {{0,0,0,1}};
    for( i=7; i < 27; ++i)
    {
        font = texture_font_new( atlas, filename, i );
        pen.x = 5;
        pen.y -= font->height;
        texture_font_load_glyphs( font, text );
        add_text( buffer, font, text, &black, &pen );
        texture_font_delete( font );
    }
    
    while (running)
    {
        realTime = glfwGetTime();
        
        while (simulationTime/timeScale < realTime)
        {
            simulationTime += 0.016;
            SCENEMANAGER.update(0.016);
        }
        /*
        RENDER.beginScene(0);
        RENDER.clear();

        SCENEMANAGER.render();

        CONSOLE.render(0.016);

        RENDER.endScene();
        */
        RENDER.beginScene();
        RENDER.clear();
        glBindTexture( GL_TEXTURE_2D, atlas->id );
        vertex_buffer_render( buffer, GL_TRIANGLES, "vtc" );
        RENDER.endScene();
        
/*
        RENDER.beginScene();
        RENDER.clear();

        RENDER.renderQuad(&q);

        RENDER.endScene();
//*/
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }
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