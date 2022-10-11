#ifndef CA_P1_SOA_HPP
#define CA_P1_SOA_HPP

#include <vector>

struct Color {  // Represents the color of a specific pixel
    float r, g, b;  // Color channels

    Color();    // Default constructor
    Color(float r, float g, float b);   // Constructor that takes the three different color channels
    ~Color();   // Destructor
};

class Image {   // Represents an image
public:
    Image(int width, int height);   // Constructor that takes the width and height of the image
    ~Image();   // Destructor

    [[nodiscard]] Color GetColor(int x, int y) const; // Getter for Color

    void SetColor(const Color &color, int x, int y);    // Setter for Color

    void Export(const char *path) const;    // Method for exporting an image

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};


#endif //CA_P1_SOA_HPP
