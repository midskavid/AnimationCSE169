////////////////////////////////////////
// Model.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 ViewProjMtx=mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position=ViewProjMtx * vec4(Position,1);

	fragPosition=Position;
	fragNormal=Normal;
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;
in vec3 fragNormal;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 color;
};  

uniform DirLight dirLight1 = DirLight(vec3(1,5,2), vec3(0.2), vec3(0.5), vec3(0,0,1));
uniform DirLight dirLight2 = DirLight(vec3(-1,-5,-2), vec3(0.2), vec3(0.5), vec3(1,0,0));
uniform DirLight dirLight3 = DirLight(vec3(-1,-5,2), vec3(0.2), vec3(0.5), vec3(0,1,0));

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

vec4 CalcColor(DirLight lt, vec3 fragNormal) {
	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance=lt.ambient + lt.color * max(0,dot(lt.direction,fragNormal));

	// Diffuse reflectance
	vec3 reflectance=irradiance * lt.diffuse;

	return vec4(sqrt(reflectance),1);
}


void main() {

	gl_FragColor=CalcColor(dirLight1, fragNormal) + CalcColor(dirLight2, fragNormal);
}

#endif

////////////////////////////////////////////////////////////////////////////////
