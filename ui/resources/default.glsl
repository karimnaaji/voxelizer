#pragma begin:vertex
#version 330
in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 mvp;

out vec2 fuv;
out vec3 fnormal;

void main() {
    // flip uv on y axis
    fuv = vec2(uv.x, 1.0 - uv.y);
    gl_Position = mvp * vec4(position, 1.0);
}
#pragma end:vertex

#pragma begin:fragment
#version 330
uniform sampler2D ao;

in vec2 fuv;
in vec3 fnormal;

out vec4 outColour;

void main(void) {
    outColour = vec4(texture(ao, fuv).rgb, 1.0);
    //outColour = vec4(fuv, 0.0, 1.0);
}
#pragma end:fragment
