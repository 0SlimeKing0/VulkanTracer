#define NUM_SPHERES 3

struct Material {
    vec3 color;
    float emisionStrengh;
    vec3 emissionColor;
};

struct Sphere {
    vec3 pos;
    float radius;
    uint materialIdx;
};

Material materials[] = {
    {vec3(1.0), 1.0, vec3(1.0)},
    {vec3(1.0, 0.0, 0.0), 0.0, vec3(0.0)}
};

Sphere spheres[] = {
    {vec3(0.0, 0.0, 3.0), 1.0, 0},
    {vec3(3.0, 0.0, 3.0), 1.0, 1},
    {vec3(-3.0, 0.0, 3.0), 1.0, 1},
};