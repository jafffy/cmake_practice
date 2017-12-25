#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 world;
uniform mat4 camera;

void main() {
    gl_Position = camera * (world * vec4(vertex, 1.0));
}