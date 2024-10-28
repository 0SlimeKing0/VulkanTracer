#version 450

#define MAX_BOUNCE 10

#include "scene.glsl"
#include "rayIntersect.glsl"

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    mat4 rotMat;
    float aspectRatio;
    vec3 camPos;
} push;

uint NextRandom(inout uint state) {
	state = state * uint(747796405) + uint(2891336453);
	uint result = ((state >> ((state >> 28) + uint(4))) ^ state) * uint(277803737);
	result = (result >> 22) ^ result;
	return result;
}

float RandomValue(inout uint state) {
	return NextRandom(state) / 4294967295.0; // 2^32 - 1
}

float RandomValueNormalDistribution(inout uint state)
{
	// Thanks to https://stackoverflow.com/a/6178290
	float theta = 2 * 3.1415926 * RandomValue(state);
	float rho = sqrt(-2 * log(RandomValue(state)));
	return rho * cos(theta);
}

vec3 RandomDirection(inout uint state)
{
	// Thanks to https://math.stackexchange.com/a/1585996
	float x = RandomValueNormalDistribution(state);
	float y = RandomValueNormalDistribution(state);
	float z = RandomValueNormalDistribution(state);
	return normalize(vec3(x, y, z));
}

vec3 RandomHemisphereDirection(vec3 normal, inout uint state)
{
	vec3 dir = RandomDirection(state);
    return dir * sign(dot(normal, dir));
}

HitInfo CalcRayCollision(Ray ray) {
    HitInfo closest = HitInfo(false, 0.0, vec3(0.0), vec3(0.0), 0);
    closest.dist = 1000000.0;

    for (int i = 0; i < NUM_SPHERES; i++) {
        Sphere sphere = spheres[i];
        HitInfo hitInfo = RaySphere(ray, sphere);

        if (hitInfo.didHit && hitInfo.dist < closest.dist) {
            closest = hitInfo;
        }
    }

    return closest;
}

vec3 Trace(Ray ray, inout uint rngState) {
    vec3 rayColor = vec3(1.0);
    vec3 light = vec3(0.0);

    for (int i = 0; i <= MAX_BOUNCE; i++) {
        HitInfo hitInfo = CalcRayCollision(ray);

        if (hitInfo.didHit) {
            ray.origin = hitInfo.pos;
            ray.dir = RandomHemisphereDirection(hitInfo.normal, rngState);

            Material material = materials[hitInfo.materialIdx];
            vec3 emittedLight = material.emissionColor * material.emisionStrengh;

            light += emittedLight * rayColor;
            rayColor *= material.color;
        }
        else {
            break;
        }
    }

    return light;
}

void main() {
    vec2 nonCenteredUV = uv + vec2(1.0, 1.0);
    uint rngState = uint(nonCenteredUV.x * 35132.0 + nonCenteredUV.y * 12859.0) * uint(719393);

    Ray ray;
    ray.origin = push.camPos;
    ray.dir = normalize((vec4(uv * vec2(push.aspectRatio, -1.0), 2.5, 0.0) * push.rotMat).xyz);

    vec3 col = Trace(ray, rngState);

    outColor = vec4(col, 1.0);
}