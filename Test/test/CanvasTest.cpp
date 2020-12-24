#include <catch.hpp>
#include <Canvas.h>
#include <Color.h>
#include <PPMInterface.h>

#include <sstream>

TEST_CASE("Canvas working properly", "[canvas]") {
    SECTION("Creating a canvas") {
        Canvas c = Canvas::Canvas(10, 20);
        REQUIRE(c.width == 10);
        REQUIRE(c.height == 20);

        bool isEveryPixelBlack = true;
        for (int i = 0; i < c.width; i++) {
            for (int j = 0; j < c.height; j++) {
                if (!(c.at(i, j) == Color::Color(0, 0, 0))) {
                    isEveryPixelBlack = false;
                }
            }
        }
        REQUIRE(isEveryPixelBlack);
    }

    SECTION("Writing pixels to a canvas") {
        Canvas c = Canvas::Canvas(10, 20);
        Color red = Color::Color(1, 0, 0);
        c.set(2, 3, red);
        REQUIRE((c.at(2, 3) == Color::Color(1, 0, 0)));
    }

    SECTION("Constructing the PPM header") {
        Canvas c = Canvas::Canvas(5, 3);
        std::string ppm = PPMInterface::canvasToPPM(c);

        std::istringstream ppmstream(ppm);
        std::string line;
        std::getline(ppmstream, line);
        REQUIRE(line.compare("P3") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("5 3") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("255") == 0);
    }
    
    
    SECTION("Constructing the PPM pixel data") {
        Canvas c = Canvas::Canvas(5, 3);
        Color c1 = Color::Color(1.5, 0, 0);
        Color c2 = Color::Color(0, 0.5, 0);
        Color c3 = Color::Color(-0.5, 0, 1);

        c.set(0, 0, c1);
        c.set(2, 1, c2);
        c.set(4, 2, c3);

        std::string ppm = PPMInterface::canvasToPPM(c);
        std::istringstream ppmstream(ppm);
        std::string line;
        
        //Skipping header
        std::getline(ppmstream, line);
        std::getline(ppmstream, line);
        std::getline(ppmstream, line);

        std::getline(ppmstream, line);
        REQUIRE(line.compare("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("0 0 0 0 0 0 0 128 0 0 0 0 0 0 0") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("0 0 0 0 0 0 0 0 0 0 0 0 0 0 255") == 0);
    }
    
    SECTION("Splitting long lines in PPM files") {
        Canvas c = Canvas::Canvas(10, 2);
        Color color = Color::Color(1, 0.8, 0.6);
        for (int i = 0; i < c.width; i++) {
            for (int j = 0; j < c.height; j++) {
                c.set(i, j, color);
            }
        }
        std::string ppm = PPMInterface::canvasToPPM(c);
        std::istringstream ppmstream(ppm);
        std::string line;

        //Skipping header
        std::getline(ppmstream, line);
        std::getline(ppmstream, line);
        std::getline(ppmstream, line);

        std::getline(ppmstream, line);
        REQUIRE(line.compare("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153") == 0);
        std::getline(ppmstream, line);
        REQUIRE(line.compare("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153") == 0);
    }

    SECTION("PPM files are terminated by a newline character") {
        Canvas c = Canvas::Canvas(5, 3);
        std::string ppm = PPMInterface::canvasToPPM(c);
        REQUIRE(ppm.back() == '\n');
    }

    SECTION("Reading a file with the wrong magic number") {
        std::string ppm = "";
        ppm.append("P32\n");
        ppm.append("1 1\n");
        ppm.append("255\n");
        ppm.append("0 0 0\n");
        REQUIRE_THROWS(PPMInterface::canvasFromPPM(ppm));
    }

    SECTION("Reading a PPM returns a canvas of the right size") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("10 2\n");
        ppm.append("255\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        Canvas c = PPMInterface::canvasFromPPM(ppm);
        REQUIRE(c.width == 10);
        REQUIRE(c.height == 2);
    }

    SECTION("Reading pixel data from a PPM file") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("4 3\n");
        ppm.append("255\n");
        ppm.append("255 127 0 0 127 255 127 255 0 255 255 255\n");
        ppm.append("0 0 0 255 0 0 0 255 0 0 0 255\n");
        ppm.append("255 255 0 0 255 255 255 0 255 127 127 127\n");
        Canvas c = PPMInterface::canvasFromPPM(ppm);
        REQUIRE(c.at(0, 0) == Color(1, 0.498, 0));
        REQUIRE(c.at(1, 0) == Color(0, 0.498, 1));
        REQUIRE(c.at(2, 0) == Color(0.498, 1, 0));
        REQUIRE(c.at(3, 0) == Color(1, 1, 1));
        REQUIRE(c.at(0, 1) == Color(0, 0, 0));
        REQUIRE(c.at(1, 1) == Color(1, 0, 0));
        REQUIRE(c.at(2, 1) == Color(0, 1, 0));
        REQUIRE(c.at(3, 1) == Color(0, 0, 1));
        REQUIRE(c.at(0, 2) == Color(1, 1, 0));
        REQUIRE(c.at(1, 2) == Color(0, 1, 1));
        REQUIRE(c.at(2, 2) == Color(1, 0, 1));
        REQUIRE(c.at(3, 2) == Color(0.498, 0.498, 0.498)); 
    }

    SECTION("PPM parsing ignores comment lines") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("# asdf\n");
        ppm.append("10 2\n");
        ppm.append("255\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("# random comment\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        ppm.append("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        Canvas c = PPMInterface::canvasFromPPM(ppm);
        REQUIRE(c.width == 10);
        REQUIRE(c.height == 2);
    }

    SECTION("PPM parsing allows an RGB triple to span lines") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("1 1\n");
        ppm.append("255\n");
        ppm.append("51\n");
        ppm.append("153\n");
        ppm.append("\n");
        ppm.append("204\n");
        Canvas c = PPMInterface::canvasFromPPM(ppm);
        REQUIRE(c.at(0, 0) == Color(0.2, 0.6, 0.8));
    }

    SECTION("PPM parsing respects the scale setting") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("2 2\n");
        ppm.append("100\n");
        ppm.append("100 100 100 50 50 50\n");
        ppm.append("75 50 25 0 0 0\n");
        Canvas c = PPMInterface::canvasFromPPM(ppm);
        REQUIRE(c.at(0, 1) == Color(0.75, 0.5, 0.25));
    }
}