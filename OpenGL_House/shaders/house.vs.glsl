#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 fullTransformMatrix;

out vec3 ourColor;

void main() {
    vec4 v = vec4(position, 1.0);
    gl_Position = fullTransformMatrix * v;
    ourColor = color;
}
