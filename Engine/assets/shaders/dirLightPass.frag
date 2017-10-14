#version 330 core

uniform vec2 screenSize;
uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;

out vec4 finalColor;

vec2 calcTexCoord(){
   return gl_FragCoord.xy / gScreenSize;
}

void main(){
    vec2 texCoord = calcTexCoord();
    vec3 worldPos = texture(gPositionMap, TexCoord).xyz;
    vec3 color = texture(gColorMap, TexCoord).xyz;
    vec3 normal = texture(gNormalMap, TexCoord).xyz;
    normal = normalize(normal);
    finalColor = vec4(color, 1.0f) * calcDirectionalLight(worldPos, normal);
} 