#include <iostream>
#include <Tuple.h>
#include <Matrix.h>
#include <Color.h>
#include <Canvas.h>
#include <PPMInterface.h>
#include <Shapes/Sphere.h>
#include <Shapes/Plane.h>
#include <Ray.h>
#include <World.h>
#include <Camera.h>
#include <Texture/Patterns/Checkers.h>
#include <Texture/UVImage.h>
#include <fstream>
#include <sstream>
#include <algorithm>

static const double PI = 3.14159265;
int main()
{
    World world = World();

    std::shared_ptr<Plane> floor = Plane::createPlane();
    floor->getMaterial().color = Color(1, 1, 1);
    floor->getMaterial().ambient = 0.1;
    floor->getMaterial().specular = 0;
    floor->getMaterial().diffuse = 0.9;
    std::shared_ptr<Checkers> checkers = Checkers::createCheckers(2, 2, Color(0, 0.5, 0), Color::white);
    std::shared_ptr<PlanarMap> map = PlanarMap::createPlanarMap();
    std::shared_ptr<Texture> texture = Texture::createTexture(checkers, map);
    floor->getMaterial().texture = texture;

    std::shared_ptr<Plane> leftWall = Plane::createPlane();
    leftWall->setTransform((Matrix::translation(0, 0, 5) * Matrix::rotationY(PI / 4) * Matrix::rotationZ(PI / 2)));
    leftWall->getMaterial().color = Color(0.8, 0, 0);
    leftWall->getMaterial().specular = 0.8;

    std::shared_ptr<Plane> rightWall = Plane::createPlane();
    rightWall->setTransform((Matrix::translation(0, 0, 5) * Matrix::rotationY(-PI / 2) * Matrix::rotationZ(PI / 2)));
    rightWall->getMaterial().color = Color(0.9, 0, 0);
    rightWall->getMaterial().specular = 0;

    /*
    std::shared_ptr<Sphere> middleSphere = Sphere::createSphere();
    middleSphere->setTransform(Matrix::rotationY(1.9) * Matrix::translation(0, 1, 0));
    middleSphere->getMaterial().diffuse = 0.9;
    middleSphere->getMaterial().specular = 0.1;
    middleSphere->getMaterial().ambient = 0.1;
    middleSphere->getMaterial().shininess = 10;

    //std::shared_ptr<Checkers> checkers2 = Checkers::createCheckers(20, 10, Color::black, Color::white);


    std::ifstream myfile("earth.ppm");
    std::stringstream ss;
    std::string rawimage;
    if (myfile.is_open())
    {
        ss << myfile.rdbuf();
        rawimage = ss.str();
    }
    //std::cout << rawimage << std::endl;
    std::shared_ptr<Canvas> earthCanvas = std::make_shared<Canvas>(PPMInterface::canvasFromPPM(rawimage));
    std::shared_ptr<UVImage> earthImage = UVImage::createUVImage(earthCanvas);
    std::shared_ptr<SphericalMap> map2 = SphericalMap::createSphericalMap();
    std::shared_ptr<Texture> texture2 = Texture::createTexture(earthImage, map2);

    middleSphere->getMaterial().texture = texture2;

    middleSphere->getMaterial().color = Color(0.1, 1, 0.5);
    middleSphere->getMaterial().diffuse = 0.7;
    middleSphere->getMaterial().specular = 0.3;
    */
    std::shared_ptr<Sphere> rightSphere = Sphere::createSphere();
    rightSphere->setTransform(Matrix::translation(2.0, 0.5, -0.5) * Matrix::scaling(0.5, 0.5, 0.5));
    rightSphere->getMaterial().color = Color(0.2, 0.7, 0.8);
    rightSphere->getMaterial().diffuse = 0.7;
    rightSphere->getMaterial().specular = 0.3;

    std::shared_ptr<Sphere> leftSphere = Sphere::createSphere();
    leftSphere->setTransform(Matrix::translation(-1.5, 0.33, -0.75) * Matrix::scaling(0.33, 0.33, 0.33));
    leftSphere->getMaterial().color = Color(0.1, 0.1, 0.1);
    leftSphere->getMaterial().diffuse = 0.7;
    leftSphere->getMaterial().specular = 0.3;

    PointLight light = PointLight(Tuple::point(-100, 100, -100), Color(1, 1, 1));

    AreaLight areaLight = AreaLight(Color(1, 1, 1), Tuple::point(-10, 10, 2.0), Tuple::vector(0, 0, 1), 4, Tuple::vector(0, 1, 0), 4, true);
    AreaLight areaLight2 = AreaLight(Color(1, 1, 1), Tuple::point(10, 4, 2.0), Tuple::vector(0, 0, 1), 4, Tuple::vector(0, 1, 0), 4, true);

    std::shared_ptr<Sphere> sphereForAreaLight = Sphere::createSphere();
    sphereForAreaLight->setTransform(Matrix::translation(0, 1, 2.0));

    //world.addLight(areaLight);
    //world.addObject(floor);
    //world.addObject(leftWall);
    //world.addObject(rightWall);
    //world.addObject(middleSphere);
    world.addObject(rightSphere);
    world.addObject(leftSphere);
    world.addObject(floor);
    world.addObject(leftWall);
    world.addObject(rightWall);
    //world.addLight(areaLight);
    //world.addLight(areaLight2);
    world.addLight(light);


    Camera camera = Camera::makeCamera(200, 100, 0.8);
    camera.setTransform(Matrix::viewTransform(Tuple::point(1, 2, -10), Tuple::point(0, 1.1, 0), Tuple::vector(0, 1, 0)));

    Canvas image = camera.render(world);

    std::string ppm = PPMInterface::canvasToPPM(image);
    PPMInterface::PPMToDisk(ppm, "test.ppm");
}
