#version 330

in vec3 vertex;
in vec4 color;

out vec4 theColor;

uniform vec3 vertexOffset;
uniform mat4 perspectiveMatrix;

void main()
{
    vec4 cameraPos = vec4(vertex.x + vertexOffset.x,
                          vertex.y + vertexOffset.y,
                          vertex.z + vertexOffset.z,
                          0.0f);

    gl_Position = perspectiveMatrix * cameraPos;

    theColor = color;
}
