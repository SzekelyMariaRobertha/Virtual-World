#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;
in vec4 fPosEye;
out vec4 fColor;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;

//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform float fogDensity;

//components
vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;

vec3 positionLight = vec3(-0.8359f, -0.45874f, 3.9297f);
vec3 colorLight = vec3(0.92,0.69,0.15);//yellow-orange

//components pointlight
float constant = 1.0f;
float linear =7.0f;
float quadratic = 18.0f;

vec3 CalcPointLight(){
        vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
        vec3 normalEye = normalize(normalMatrix * fNormal);
 
         //normalize light direction
        vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));
 
         //compute view direction (in eye coordinates, the viewer is situated at the origin
         vec3 viewDir = normalize(- fPosEye.xyz);

          //compute distance to light
          vec4 lightPosEye = (view * model * vec4(positionLight, 1.0f));
          float dist = length(lightPosEye.xyz - fPosEye.xyz);        
	
                //compute attenuation
                float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));

	//compute color
	vec3 ambient = att * ambientStrength * colorLight;
	vec3 diffuse = att * max(dot(normalEye, lightDirN), 0.0f) * colorLight;
	vec3 reflectDir = reflect(-lightDirN, normalEye);
    	float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    	vec3 specular = att * specularStrength * specCoeff * colorLight;

	vec3 colorFinalPoint = min((ambient + diffuse) + specular, 1.0f);

	return colorFinalPoint;
}

void computeDirLight()
{
    //compute eye space coordinates
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = ambientStrength * lightColor;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;
}


float computeFog()
{
   float fragmentDistance = length(fPosEye);
   float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));
 
   return clamp(fogFactor, 0.0f, 1.0f);
}

void main() 
{
    computeDirLight();
    vec3 totalColor;
    //compute final vertex color
    vec3 color = min((ambient + diffuse) * texture(diffuseTexture, fTexCoords).rgb + specular * texture(specularTexture, fTexCoords).rgb, 1.0f);

    vec4 colorFromTexture = texture(diffuseTexture, fTexCoords);
    if (colorFromTexture.a < 0.1) 
          discard;

    float fogFactor = computeFog();    
    fColor = vec4(color, 1.0f);
    vec4 fogColor = vec4(0.9f, 0.5f, 0.5f, 0.5f);

    if(fPosition.y < positionLight.y)
	totalColor= color + CalcPointLight();
    else totalColor = color;
	fColor = fogColor * (1-fogFactor) + vec4(totalColor, texture(diffuseTexture, fTexCoords).a)*fogFactor;
    //fColor = fogColor * (1-fogFactor) + vec4(color, texture(diffuseTexture, fTexCoords).a)*fogFactor;
}
