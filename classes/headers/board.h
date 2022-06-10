#ifndef BOARD
#define BOARD

#include <string>

class Board {
public:
    virtual void Initialize() = 0;
    virtual void ImportFEN(std::string FEN) = 0;
    virtual void MovePiece(const int startX, const int startY, const int endX, const int endY) = 0;
private:
};
#endif