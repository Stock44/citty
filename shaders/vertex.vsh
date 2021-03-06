#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 3) in vec4 modelMat1;
layout (location = 4) in vec4 modelMat2;
layout (location = 5) in vec4 modelMat3;
layout (location = 6) in vec4 modelMat4;

layout(location = 7) in vec4 inColor;

out vec4 color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = inColor;

    mat4 modelMat;
    modelMat[0] = modelMat1;
    modelMat[1] = modelMat2;
    modelMat[2] = modelMat3;
    modelMat[3] = modelMat4;

    gl_Position = projection * view * modelMat * vec4(vertex, 1.0);
}
