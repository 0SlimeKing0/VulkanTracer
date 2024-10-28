struct Ray {
    vec3 origin;
    vec3 dir;
};

struct HitInfo {
    bool didHit;
    float dist;
    vec3 pos;
    vec3 normal;
    uint materialIdx;
};

HitInfo RaySphere(Ray ray, Sphere sphere) {
    HitInfo hitInfo = HitInfo(false, 0.0, vec3(0.0), vec3(0.0), 0);
    vec3 offsetOrigin = ray.origin - sphere.pos;

    float r = sphere.radius;

    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(offsetOrigin, ray.dir);
    float c = dot(offsetOrigin, offsetOrigin) - r * r;

    float d = b * b - 4 * a * c;

    if (d >= 0) {
        float dist = (-b - sqrt(d)) / (2 * a);

        if (dist > 0) {
            hitInfo.didHit = true;
            hitInfo.dist = dist;
            hitInfo.pos = ray.origin + ray.dir * dist;
            hitInfo.normal = normalize(hitInfo.pos - sphere.pos);
            hitInfo.materialIdx = sphere.materialIdx;
        }
    }

    return hitInfo;
}