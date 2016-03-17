#version 330

layout (location=0) in vec3 posAttr;
layout (location=1) in vec3 colAttr;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(posAttr,1.0f);
  color = colAttr;
}
