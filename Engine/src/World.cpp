#include <World.h>
#include <Shapes/Sphere.h>
#include <algorithm>

World::World()
{
}

World World::makeDefaultWorld()
{
    World w = World();
    w.addLight(PointLight(Tuple::point(-10, 10, -10), Color(1, 1, 1)));
    std::shared_ptr<Sphere> s1 = Sphere::createSphere();
    s1->setMaterial(Material(Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2, 200));
    std::shared_ptr<Sphere> s2 = Sphere::createSphere();
    s2->setTransform(Matrix::scaling(0.5, 0.5, 0.5));
    w.addObject(s1);
    w.addObject(s2);
    return w;
}

int World::getLightCount() const
{
    return (int) (pointLights.size() + areaLights.size());
}

void World::addLight(const PointLight &pointLight)
{
    pointLights.push_back(pointLight);
}

void World::addLight(const AreaLight &areaLight)
{
    areaLights.push_back(areaLight);
}

PointLight& World::getPointLight(int index)
{
    return pointLights[index];
}

PointLight World::getPointLight(int index) const
{
    return pointLights[index];
}

AreaLight& World::getAreaLight(int index)
{
    return areaLights[index];
}

AreaLight World::getAreaLight(int index) const
{
    return areaLights[index];
}

int World::getObjectCount() const
{
    return (int) objects.size();
}

void World::addObject(const std::shared_ptr<Shape> &object)
{
    objects.push_back(object);
}
std::shared_ptr<Shape> World::getObject(int index) const
{
    return objects[index];
}

std::vector<Intersection> World::intersects(const Ray & r) const
{
    std::vector<Intersection> result;
    for (const auto &s : objects) 
    {
        for (Intersection i : s->intersects(r)) {
            result.push_back(i);
       }
    }

    sort(result.begin(), result.end(), [](const Intersection i1, const Intersection i2){
        return i1.getT() < i2.getT();
    });

    return result;;
}

Color World::shadeHit(Hit hit) const
{
    Color result = Color(0, 0, 0);

    for (const auto &pointLight : pointLights) {
        double intensity = intensityAt(pointLight, hit.overPoint);
        result = result + hit.getObject()->getMaterial().lighting(
            hit.getObject()->getInverseTransform(), pointLight, hit.overPoint, hit.eyev, hit.normalv, intensity
        );
    }

    
    for (const auto &areaLight : areaLights) {
        double intensity = intensityAt(areaLight, hit.overPoint);
        result = result + hit.getObject()->getMaterial().lighting(
            hit.getObject()->getInverseTransform(), areaLight, hit.overPoint, hit.eyev, hit.normalv, intensity
        );
    }
    
    
    return result;
}

bool World::isShadowed(const Tuple &lightPosition, const Tuple &point) const
{
    Tuple v = lightPosition - point;
    double distance = Tuple::magnitude(v);
    Tuple direction = Tuple::normalize(v);

    Ray ray = Ray(point, direction);
    std::vector<Intersection> intersections = intersects(ray);
    std::optional<Intersection> hit = Intersection::hit(intersections);
    
    return hit.has_value() && (hit.value().getT() < distance);
}

double World::intensityAt(const PointLight & light, const Tuple & point) const
{
    return isShadowed(light.position, point) ? 0.0 : 1.0;
}

double World::intensityAt(const AreaLight & light, const Tuple & point) const
{
    double total = 0.0;

    for (int u = 0; u < light.uSteps; u++) {
        for (int v = 0; v < light.vSteps; v++) {
            Tuple lightPosition = light.sample(u, v);
            total = isShadowed(lightPosition, point) ? total : (total + 1.0);
        }
    }

    return total / light.samples;
}

Color World::colorAt(const Ray & r) const
{
    std::vector<Intersection> intersections = intersects(r);
    std::optional<Intersection> hit = Intersection::hit(intersections);
    
    if (hit.has_value()) {
        Hit h = hit.value().prepareHit(r);
        return shadeHit(h);
    }
    else {
        return Color::black;
    }
}