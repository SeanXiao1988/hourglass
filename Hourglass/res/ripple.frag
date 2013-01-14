//uniform vec2 resolution; // Screen resolution
const vec2 resolution = vec2(800.0, 600.0);
uniform float time; // time in seconds
uniform sampler2D tex0; // scene buffer
void main(void)
{
    vec2 tc = gl_TexCoord[0].xy;
    vec2 p = -1.0 + 2.0 * tc;
    float len = length(p);
    vec2 uv = tc + (p/len)*cos(len*12.0-time*4.0)*0.03;
    vec3 col = texture2D(tex0,uv).xyz;
    
    vec4 color = texture2D(tex0, gl_TexCoord[0].st);
    gl_FragColor = vec4(col, color.a);
}