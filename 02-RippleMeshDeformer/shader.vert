#version 330 core
layout(location = 0) in vec3 vVertex;
out vec4 color;
uniform mat4 MVP;
uniform float time;
const float amplitude=0.5;
const float frequency=2;
const float PI=3.14159;

void main() {
    float distance=length(vVertex);
    float y=amplitude*sin(-PI*distance*frequency+time);
    color=vec4(vVertex.x,y,vVertex.z,1);
    gl_Position=MVP*vec4(vVertex.x,vVertex.y,vVertex.z,1);

}
