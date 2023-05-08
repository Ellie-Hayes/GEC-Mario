#pragma once
#include "Commons.h"
#include <iostream>
#include <list>

class TileTypes
{
};

struct Direction2D
{
    std::list<Vector2D> cardinalDirectionList =
    {
        Vector2D(0,1), //Up
        Vector2D(1,0), //Right
        Vector2D(0,-1),//Down
        Vector2D(-1,0),//Left
    };

    std::list<Vector2D> diagonalDirectionList =
    {
        Vector2D(1,1), //Up - right
        Vector2D(1,-1), //Right - down
        Vector2D(-1,-1),//Down - left
        Vector2D(-1,1),//Left - up
    };

    std::list<Vector2D> eightDirectionList =
    {
         Vector2D(0,1), //Up
         Vector2D(1,1), //Up - right
         Vector2D(1,0), //Right
         Vector2D(1,-1), //Right - down
         Vector2D(0,-1),//Down
         Vector2D(-1,-1),//Down - left
         Vector2D(-1,0),//Left
         Vector2D(-1,1),//Left - up
    };

};

struct WallBinaryTypes
{
    //Mains

    std::list<std::string> wallTop =
    {
        "00100010",
        "01100010",
        "11100010",
        "11100011",
        "10100011",
        "00100011",
        "00100110",
        "00110010",
        "10100010",
        "01100011",
        "01110010",
        "01100110",
        "11010011",
        "11100110",
        "00100111",
        "00010110",
        "11110011",
        "11100111",
        "11110111",
        "10100111",
        "00110011",
        "00110110",
        "00000000", //SINGLE TILE MAYBE CHANGE
        "00000101",
        "01000000",
        "00010001",
        "00010101",
        "01000101",
        "01000100",
        "01010100",
        "01010001",
        "11010101",
        "01010101",
        "00000001",
        "00010000",
        "01010000",
        "00010100",
        "01000001",
        "01110011",
        "01110111",
        "00000100",
        "11110010",
        "10110011",
    };

    std::list<std::string> wallRight =
    {
       "10011111",
       "10001111",
       "11001110",
       "11001111",
       "10001100",
       "10001001",
       "11011010",
       "10001101",
       "10001011",
       "11011111",
       "11011011",
       "11001101",
       "11001100",
       "10001010",
       "10001110",
       "10011010",
       "10011011",
       "01001100",
       "11011110",
       "10010001",
       "01001010",
    };

    std::list<std::string> wallBottom =
    {
        "00111110",
        "01111110",
        "00111111",
        "01111111",
        "00111101",
    };

    std::list<std::string> wallLeft =
    {
        "11111100",
        "11111000",
        "11111001",
        "10011100",
        "10111101",
        "11001000",
        "10111001",
        "00101010",
        "11011001",
        "11111101",
        "11011100",
        "10111100",
        "11001001",
        "11011101",
        "11101000",
        "11011000",
        "00101000",
        "10101000",
        "10011000",
        "10111000",
        "10011101",
        "10001000",
        "10101100",
        "11101100",
        "10101101",
    };

    std::list<std::string> wallFull =
    {
        "11111111",
        "00110101",
    };




    //Corners

    std::list<std::string> wallCornerLeftTop =
    {
        "00100000",
        "11100000",
        "00100100",
        "01110001",
        "11110001",
        "01100000",
        "00100001",
        "11110000",
        "10100000",
        "11100001",
        "10100001",
        "11110100",
        "10010000",
        "10100101",
        "00110001",
        "11000001",
        "10110000",
        "10000000",
        "11110101",
        "11000000",
        "10000001",
        "00110000",
        "01110000",
        "11100101",
        "11010000",
        "01100001",
        "01110101",
        "00110100",
        "10110101",
    };

    std::list<std::string> wallCornerLeftBottom =
    {
        "00111000",
        "00111100",
        "01111100",
        "01101000",
        "01111001",
        "01101100",
        "01111000",
        "01001000",
        "01011000",
        "00111001",
        "00011000",
        "01011100",
        "01110100",
        "00001000",
        "01011001",
        "00011001",
    };

    std::list<std::string> wallCornerRightTop =
    {
        "00000010",
        "01000010",
        "11000010",
        "00010010",
        "10000010",
        "11000011",
        "10000011",
        "10000111",
        "11000111",
        "00000011",
        "10000101",
        "11010111",
        "10010111",
        "00000110",
        "10000110",
        "01000111",
        "01000011",
        "10000100",
        "11010110",
        "01000110",
        "11000110",
    };

    std::list<std::string> wallCornerRightBottom =
    {
        "00011111",
        "00011110",
        "00001110",
        "00001111",
        "01001110",
        "01001111",
        "00000111",
        "00011100",
        "00001100",
        "01011110",
        "01011111",
        "01011101",
        "01010111",
        "00010111",
        "00011101",
        "00001001",
        "00001101",
        "00001011",
    };

    

    //Diagonals

    std::list<std::string> wallDiagonalLeftBottom =
    {
        "11111110",
        "10111110",
        "11110110",
    };

    std::list<std::string> wallDiagonalRightBottom =
    {
        "10111111",
        "10111011",
        "10110111",
    };

    std::list<std::string> wallDiagonalLeftTop =
    {
       "11111010",
       "00111010",
       "11111011",
       "11101011",
       "00011010",
       "01011011",
       "01111011",
       "00001010",
       "10101010",
       "11101010",
       "00111011",
       "10101011",
       "10111010",
       "01111010",
    };

    std::list<std::string> wallDiagonalRightTop =
    {
        "00101100",
        "00101110",
        "00101111",
        "10101111",
        "11101111",
        "11101101",
        "11101110",
        "11101001",
        "01101110",
        "01101111",
    };

    //195
    
};

struct FourBitBinaryTypes
{
    std::list<std::string> BitStart =
    {
        "1000",
    };

    std::list<std::string> BitMiddle =
    {
        "1010",
    };

    std::list<std::string> BitTop =
    {
       "1100",
       "1101",
       "1001",
    };

    std::list<std::string> BitFull =
    {
        "1111",
        "1110",
        "1011",
        "0110",
        "0111",
        "0011",
        "0001",
        "0100",
        "0101",
        "0010",
    };
    
};

struct PlatformTypes
{
    std::list<std::string> PlatformMiddle =
    {
        "1000",
    };

    std::list<std::string> PlatformEndLeft =
    {
        "1000",
    };

    std::list<std::string> PlatformEndRight =
    {
        "1000",
    };

    std::list<std::string> PlatformSupport =
    {
        "1000",
    };
};