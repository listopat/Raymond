#include <PPMInterface.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

const std::string PPM_TYPE_HEADER = "P3\n";
const std::string COLOR_RANGE = "255\n";

std::string convertToString(Color color);
void removeCommentsFromPPM(std::string &ppm);

std::string PPMInterface::canvasToPPM(const Canvas &canvas) 
{
    std::string result = "";
    std::string dimensions = std::to_string(canvas.width) + " " + std::to_string(canvas.height) + std::string("\n");
    result.append(PPM_TYPE_HEADER + dimensions + COLOR_RANGE);

    
    for (int i = 0; i < canvas.height; i++) {
        std::string line = "";
        for (int j = 0; j < canvas.width; j++) {
            std::string values = convertToString(canvas.at(j, i));
            
            if ((line + values).length() > 70) {
                line.pop_back();
                result.append(line + "\n");
                line = "";
            }

            line.append(values);
            
            if (j != canvas.width - 1) {
                line.append(" ");
            }
            else {
                line.append("\n");
            }
        }
        result.append(line);
    }
    return result;
}

void PPMInterface::PPMToDisk(std::string ppm, std::string path)
{
    std::ofstream file(path);
    
    if (file.is_open()) {
        file << ppm;
        file.close();
    }
}

Canvas PPMInterface::canvasFromPPM(std::string &ppm)
{
    removeCommentsFromPPM(ppm);
    std::istringstream iss(ppm);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    if (results.front().compare("P3") != 0) 
    {
        throw std::invalid_argument("Incorrect magic number");
    };

    Canvas result = Canvas(std::stoi(results.at(1)), std::stoi(results.at(2)));
    double scale = std::stod(results.at(3));
    int currentIndex = 4;
    for(int y = 0 ; y < result.height; y++) { 
        for (int x = 0; x < result.width; x++) {
            Color color = Color(std::stoi(results.at(currentIndex)) / scale, std::stoi(results.at(currentIndex + 1)) / scale, std::stoi(results.at(currentIndex + 2)) / scale);
            result.set(x, y, color);
            currentIndex += 3;
        }
    }

    return result;
}

std::string convertToString(Color color)
{
    int r = (int) round(255.0 * std::clamp(color.red, 0.0, 1.0));
    int g = (int) round(255.0 * std::clamp(color.green, 0.0, 1.0));
    int b = (int) round(255.0 * std::clamp(color.blue, 0.0, 1.0));
    return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
}

void removeCommentsFromPPM(std::string &ppm) 
{
    size_t commentBeginning = ppm.find("#");
    while (commentBeginning != std::string::npos) {
        size_t lineEnd = ppm.find('\n', commentBeginning);
        size_t previousLineEnd = ppm.rfind('\n', lineEnd-1);
        ppm.erase(commentBeginning, lineEnd - previousLineEnd);
        commentBeginning = ppm.find("#");
    }
}


