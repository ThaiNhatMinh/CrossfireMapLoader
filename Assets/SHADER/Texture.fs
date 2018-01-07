#version 140

struct Light // direction light
{
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
};

// surface reflectance
struct Material
{
	vec3 Ka;	// reflect ambient light
	vec3 Kd;	// diffuse surface reflectance
	vec3 Ks;	// reflect specular light
	vec3 exp;
};

struct SurfaceInfo
{
	vec3 pos;
	vec3 normal;
	vec4 color;
	Material prop;
};

#define MAX_LIGHT 10

in vec3 vNormal;
in vec2 vUV;
in vec3 vFragPos;

uniform sampler2D Tex;
uniform vec3 EyePos;
uniform Light gLight;
uniform Material gMaterial;

vec4 ParallelLight(SurfaceInfo v, Light L, vec3 eyePos);



out vec4 color;

void main()
{
	vec4 texel = vec4(1.0f);
	SurfaceInfo v;
	v.pos = vFragPos;
	v.normal = vNormal;
	v.color = texel;
	v.prop = gMaterial;

	color = ParallelLight(v, gLight, EyePos);
}

vec4 ParallelLight(SurfaceInfo v, Light L, vec3 eyePos)
{
	
	vec3 LightI = vec3(0,0,0);
	// The light vector aims opposite the direction the light rays travel.
	vec3 lightDir = normalize(-L.direction);
	vec3 viewDir = normalize(eyePos - v.pos);

	vec3 Ia = v.prop.Ka * L.La;
	LightI += Ia;
	float diffuseS = max(dot(v.normal, lightDir),0.0);
	
	vec3 Id = v.prop.Kd * L.Ld * diffuseS;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float exp = v.prop.exp.z;
	//if(exp==128.0) exp = 128;
	//else exp = 0;
	float specularS = pow(max(dot(v.normal, halfwayDir), 0.0), exp);

	vec3 Is = v.prop.Ks * L.Ls * specularS;

	LightI += Id + Is;
	

	vec4 litColor = v.color *vec4(LightI,1.0f);
	return litColor;
}