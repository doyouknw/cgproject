#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 mvp;

out vec3 fPosition;
out vec2 fTexcoord;

void main()
{
  gl_Position = mvp * vec4(position, 1.0f);

  fPosition = vec3(gl_Position);
  fTexcoord = texcoord;
}
