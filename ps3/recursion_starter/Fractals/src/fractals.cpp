/*
 * File: fractals.cpp
 * --------------------------
 * Name: Josh Sullivan
 * Section leader: Luciano Gonzalez
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */
const double COS_30 = 0.86602540378; // Cosine of 30 degrees
const double SIN_30 = 0.5;           //  Sine of 30 degrees

void drawEquilateralTriangle(GWindow& gw, GPoint ptUpperLeft, double size);

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {

    // Running through the exception handling
    if (x < 0 or y < 0){
        throw "Negative origin coordinates are not allowed.";
    }
    else if (size < 0){
        throw "Negative triangle side length is not allowed.";
    }
    else if (order < 0){
        throw "Negative fractal order is not allowed.";
    }
    // Return if order is equal to zero.
    if (order == 0){
        return;
    }

    // Base case for order 1 fractal
    if (order == 1){
        drawEquilateralTriangle(gw, GPoint(x, y), size);
    }

    // Recursive case for order > 1 fractals
    // Computing the midpoint of the upper horizontal line of the "order"-th order triangle
    double xUpperMid = x + (size / 2);
    double yUpperMid = y;

    // Computing the midpoint of the leftmost diagonal line of the "order"-th order triangle
    double xDiagMid = x + ((size / 2) * SIN_30);
    double yDiagMid = y + ((size / 2) * COS_30);

    // Drawing a "order - 1" -th order triangle with upper left point at "order"-th order triangle upper left
    drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
    // Drawing a "order - 1" -th order triangle with upper left point at "order"-th order triangle upper mid
    drawSierpinskiTriangle(gw, xUpperMid, yUpperMid, size / 2, order - 1);
    // Drawing a "order - 1" -th order triangle with upper left point at "order"-th order triangle left diagonal mid
    drawSierpinskiTriangle(gw, xDiagMid, yDiagMid, size / 2, order - 1);
}

void drawEquilateralTriangle(GWindow& gw, GPoint ptUpperLeft, double size){

    // Draws a downward pointing equilateral triangle, given the upper left point and side length

    GPoint ptUpperRight(ptUpperLeft.getX() + size, ptUpperLeft.getY());
    GPoint ptBottom(ptUpperLeft.getX() + (size * SIN_30), ptUpperLeft.getY() + (size * COS_30));

    gw.drawLine(ptUpperLeft, ptUpperRight);
    gw.drawLine(ptUpperRight, ptBottom);
    gw.drawLine(ptBottom, ptUpperLeft);

}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    // TODO: write this function
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors

    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    // Looping over the grid of pixels
    for (int col = 0; col< pixels.numCols(); col++){
        for (int row = 0; row < pixels.numRows(); row++){
            // Creating the complex number from the grid coordinate
            Complex c(minX + (incX * col), minY + (incY * row));

            // Enacting the set evaluation
            int numIters = mandelbrotSetIterations(c, maxIterations);

            // Checking conditions for plot color
            if (numIters == maxIterations){
                            pixels[row][col] = color;
                        }
            else if (color == 0){
                pixels[row][col] = palette[numIters % palette.size()];
            }
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {

    Complex z0(0., 0.);
    // Calling the recursive function
    int numIter2Div = mandelbrotSetIterations(z0, c, maxIterations);

    return numIter2Div; // Only here to make this compile
}
/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    // Base case
    if (remainingIterations == 0 || z.abs() > 4){
        return 0;
    }

    Complex z_np1;
    z_np1 = (z * z) + c;
    // Adding one more to the number of iterations
    return 1 + mandelbrotSetIterations(z_np1, c, remainingIterations - 1);
}


// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
