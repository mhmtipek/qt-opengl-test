#version 330

in vec3 vertex;

uniform vec2 vertexOffset;
uniform mat4 perspectiveMatrix;

void main()
{
    vec4 cameraPos = vec4(vertex.x + vertexOffset.x,
                          vertex.y + vertexOffset.y,
                          vertex.z, 0.0f);

//    gl_Position = perspectiveMatrix * cameraPos;
    gl_Position = vec4(0.1f, 0.1f, 0.1f, 0.1f);
}
