#pragma begin:vertex
#version 330
in vec3 position;
in vec3 normal;

uniform mat4 mvp;

out vec3 fnormal;

void main() {
    fnormal = normal;
    gl_Position = mvp * vec4(position, 1.0);
}
#pragma end:vertex

#pragma begin:fragment
#version 330

in vec3 fnormal;

out vec4 outColour;

void main(void) {
    outColour = vec4(fnormal, 1.0);
    //outColour = vec4(vec3(0.5), 1.0);
}

#pragma end:fragment
