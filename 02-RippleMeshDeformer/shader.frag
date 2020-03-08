#version 330 core
layout(location=0) out vec4 vFragColor;
in vec4 color;
void main() {
    vFragColor = color;//vec4(1,1,1,1);
}
