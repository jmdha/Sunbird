#include <chess/board.hpp>
#include <chess/import.hpp>
#include <cstdlib>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    std::string iCol = std::string(argv[1]);
    Column col;
    if (iCol == "a")
        col = Column::A;
    else if (iCol == "b")
        col = Column::B;
    else if (iCol == "c")
        col = Column::C;
    else if (iCol == "d")
        col = Column::D;
    else if (iCol == "e")
        col = Column::E;
    else if (iCol == "f")
        col = Column::F;
    else if (iCol == "g")
        col = Column::G;
    else if (iCol == "h")
        col = Column::H;
    else if (iCol == "none")
        col = Column::None;

    Position pos;
    pos.SetEP(col);
    Column actualCol = pos.GetEP();

    if (col == actualCol)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected %lu actual %lu\n", (uint64_t)col, (uint64_t)actualCol);
        exit(EXIT_FAILURE);
    }
}
