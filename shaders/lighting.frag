#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float     shininess;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 viewPos;

/**

   float theta     = dot(lightDir, normalize(-spotLight.direction));
  float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

*/

vec3 CalculationDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculationPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 ViewDir);

void main(){
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result = CalculationDirectionalLight(directionalLight, norm, viewDir);

  for(int i = 0; i < NR_POINT_LIGHTS; i++) {
      result += CalculationPointLight(pointLights[i], norm, FragPos, viewDir);
  }

  FragColor = vec4(result, 1.0);
}

vec3 CalculationDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(-light.direction);
  float diff = max(dot(normal, lightDirection), 0.0);

  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  
  return (ambient + diffuse + specular);
}

vec3 CalculationPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}
