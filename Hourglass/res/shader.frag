/*
uniform sampler2D tex;
uniform float time;
float totaltime = 0.0;

void main()
{
    vec4 color = texture2D(tex, gl_TexCoord[0].st);
    
    totaltime+=time;
    if (totaltime > 1.0)
    {
		totaltime = 1.0;
	}
	
	float current_red		= totaltime;//1.0 + totaltime * (0.299 - 1.0);
	float current_green		= totaltime;//1.0 + totaltime * (0.587 - 1.0);
	float current_blue		= totaltime;//1.0 + totaltime * (0.114 - 1.0);
	
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    gl_FragColor = vec4(gray, gray, gray, color.a);
    //gl_FragColor = color;
}
*/

const vec3 lumCoeff = vec3(0.2125, 0.7154, 0.0721);
uniform sampler2D tex;
uniform float time;

void main()
{	
	vec4 texColor = texture2D(tex, gl_TexCoord[0].st);
	vec3 intensity = vec3(dot(texColor.rgb, lumCoeff));
	vec3 color = mix(intensity, texColor.rgb, time);
	gl_FragColor = vec4(color, texColor.a);
}