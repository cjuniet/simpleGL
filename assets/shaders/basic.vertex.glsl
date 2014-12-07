#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 Color;

uniform mat4 proj;
uniform mat4 model;

void main()
{
    gl_Position = proj * model * vec4(position, 0.0f, 1.0f);
    Color = color;
}
