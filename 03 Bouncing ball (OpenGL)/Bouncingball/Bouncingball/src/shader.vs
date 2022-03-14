#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 pos;
void main()
{
    gl_Position = vec4((aPos.x + pos.x) / 50.0, (aPos.y + pos.y) / 50.0, 0.0, 1.0);
}