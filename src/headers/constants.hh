#ifndef CONSTANTS
#define CONSTANTS

#include <array>

#define U8  unsigned short
#define U64 unsigned long long int
#define C8(x) (U8)1 << ((U8) x)
#define C64(x) (U64)1 << ((U64) x)

#define WIDTH 8
#define HEIGHT 8
#define SQUARECOUNT 64

// 218 I believe to be the max number of moves, however such an amount of moves is unrealistic, as such, it's set to a more realistic 128.
// Should still cover almost all positions reachable in a normal game of chess.
#define MAXMOVECOUNT 128

#define PIECECOUNT 6
#define COLORCOUNT 2
#define DIRECTIONCOUNT 8

#define PAWNROWWHITE 1
#define PAWNROWBLACK 6

#define BACKROWWHITE 0
#define BACKROWBLACK 7

#define CASTLINGKING 1
#define CASTLINGQUEEN 0

#define EDGE 0xff818181818181ff
#define CORNER 0x8100000000000081

//#define DEBUGGING true
//#define STATS true
#define OPENINGBOOK true

enum class Color : U8 {
    White,
    Black,
    None
};

enum class PieceType : U8 {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None
};

constexpr PieceType pieceTypes[PIECECOUNT] { PieceType::Pawn, PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen, PieceType::King };

// Found through tune.cpp
enum class PieceValue : int {
    Pawn = 111,
    Knight = 306,
    Bishop = 309,
    Rook = 574,
    Queen = 1127,
    //King = 20000,
    Inf = 300000,
    None
};

constexpr std::array<int, PIECECOUNT> pieceValues { (int) PieceValue::Pawn, (int) PieceValue::Knight, (int) PieceValue::Bishop, (int) PieceValue::Rook, (int) PieceValue::Queen };

enum class PawnStructureValue : int {
	Doubled = -50, // If one pawn is in front of another
	Connected = 50 // If another pawn is being attacked by the given pawn
};

enum class PieceChar : char {
    PawnWhite = 'P',
    PawnBlack = 'p',
    KnightWhite = 'N',
    KnightBlack = 'n',
    BishopWhite = 'B',
    BishopBlack = 'b',
    RookWhite = 'R',
    RookBlack = 'r',
    QueenWhite = 'Q',
    QueenBlack = 'q',
    KingWhite = 'K',
    KingBlack = 'k',
    None = 'O'
};

enum class Square : U8 {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};

constexpr Square initRookPos[2][2] = { { Square::H1, Square::A1 }, { Square::H8, Square::A8 } };


enum class Row : U64 {
    Row1 = 0xff,
    Row2 = 0xff00,
    Row3 = 0xff0000,
    Row4 = 0xff000000,
    Row5 = 0xff00000000,
    Row6 = 0xff0000000000,
    Row7 = 0xff000000000000,
    Row8 = 0xff00000000000000,
    None = 0x0
};

enum class Column : U64 {
    A = 0x101010101010101,
    B = 0x202020202020202,
    C = 0x404040404040404,
    D = 0x808080808080808,
    E = 0x1010101010101010,
    F = 0x2020202020202020,
    G = 0x4040404040404040,
    H = 0x8080808080808080,
    None = 0x0
};

enum class Direction : int {
    North = 8,
    East = 1,
    South = -North,
    West = -East,

    NorthEast = North + East,
    NorthWest = North + West,
    SouthEast = South + East,
    SouthWest = South + West,

    None = 0
};

enum class DirectionIndex : U8 {
    North,
    East,
    South,
    West,

    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest,
    None
};

constexpr Direction directions[8] = {Direction::North, Direction::East, Direction::South, Direction::West,
                                     Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
constexpr Direction oppDirections[8] = {Direction::South, Direction::West, Direction::North, Direction::East,
                                     Direction::SouthWest, Direction::SouthEast, Direction::NorthWest, Direction::NorthEast };
constexpr Direction rookDirections[4]   = { Direction::North, Direction::East, Direction::South, Direction::West };
constexpr Direction bishopDirections[4] = { Direction::NorthWest, Direction::NorthEast, Direction::SouthWest, Direction::SouthEast };
constexpr Direction queenDirections[8]  = { Direction::North, Direction::East, Direction::South, Direction::West, 
                                            Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };

enum class NotEdge : U64 {
    North = 0xffffffffffffff,
    East = 0x7f7f7f7f7f7f7f7f,
    South = 0xffffffffffffff00,
    West = 0xfefefefefefefefe,
    NorthEast = North & East,
    NorthWest = North & West,
    SouthEast = South & East,
    SouthWest = South & West,
};

constexpr NotEdge notEdges[8] = { NotEdge::North, NotEdge::East, NotEdge::South, NotEdge::West,
                                  NotEdge::NorthEast, NotEdge::NorthWest, NotEdge::SouthEast, NotEdge::SouthWest };

enum class NotEdgeKnight : U64 {
    North = 0xffffffffffff,
    East = 0x3f3f3f3f3f3f3f3f,
    South = 0xffffffffffff0000,
    West = 0xfcfcfcfcfcfcfcfc
};

enum class MoveType : U8 {
    Quiet = 0,
    DoublePawnPush = 1,
    KingCastle = 2,
    QueenCastle = 3,
    Capture = 4,
    EPCapture = 5,
    NPromotion = 8,
    BPromotion = 9,
    RPromotion = 10,
    QPromotion = 11,
    NPromotionCapture = 12,
    BPromotionCapture = 13,
    RPromotionCapture = 14,
    QPromotionCapture = 15,
    SPECIAL_DRAW = 16,
    SPECIAL_CHECKMATE = 17
};

#define PromotionBit 0x8
#define CaptureBit 0x4
#define CastlingBit 0x2

enum class Castling : U8 {
    King,
    Queen
};

enum class CastlingSided : U8 {
    WK = 0,
    WQ = 1,
    BK = 2,
    BQ = 3
};

enum class CastlingBlockSquares : U64 {
    KSideWhite = 0x60,
    QSideWhite = 0xe,
    KSideBlack = 0x6000000000000000,
    QSideBlack = 0xe00000000000000
};

enum class CastlingAttackSquares : U64 {
    KSideWhite = 0x60,
    QSideWhite = 0xc,
    KSideBlack = 0x6000000000000000,
    QSideBlack = 0xc00000000000000
};

// These are non-sliding piece moves
constexpr U64 KingMoves[64] = { 770LLU, 1797LLU, 3594LLU, 7188LLU, 14376LLU, 28752LLU, 57504LLU, 49216LLU, 197123LLU, 460039LLU, 920078LLU, 1840156LLU, 3680312LLU, 7360624LLU, 14721248LLU, 12599488LLU, 50463488LLU, 117769984LLU, 235539968LLU, 471079936LLU, 942159872LLU, 1884319744LLU, 3768639488LLU, 3225468928LLU, 12918652928LLU, 30149115904LLU, 60298231808LLU, 120596463616LLU, 241192927232LLU, 482385854464LLU, 964771708928LLU, 825720045568LLU, 3307175149568LLU, 7718173671424LLU, 15436347342848LLU, 30872694685696LLU, 61745389371392LLU, 123490778742784LLU, 246981557485568LLU, 211384331665408LLU, 846636838289408LLU, 1975852459884544LLU, 3951704919769088LLU, 7903409839538176LLU, 15806819679076352LLU, 31613639358152704LLU, 63227278716305408LLU, 54114388906344448LLU, 216739030602088448LLU, 505818229730443264LLU, 1011636459460886528LLU, 2023272918921773056LLU, 4046545837843546112LLU, 8093091675687092224LLU, 16186183351374184448LLU, 13853283560024178688LLU, 144959613005987840LLU, 362258295026614272LLU, 724516590053228544LLU, 1449033180106457088LLU, 2898066360212914176LLU, 5796132720425828352LLU, 11592265440851656704LLU, 4665729213955833856LLU };
constexpr U64 KnightMoves[64] = { 132096LLU, 329728LLU, 659712LLU, 1319424LLU, 2638848LLU, 5277696LLU, 10489856LLU, 4202496LLU, 33816580LLU, 84410376LLU, 168886289LLU, 337772578LLU, 675545156LLU, 1351090312LLU, 2685403152LLU, 1075839008LLU, 8657044482LLU, 21609056261LLU, 43234889994LLU, 86469779988LLU, 172939559976LLU, 345879119952LLU, 687463207072LLU, 275414786112LLU, 2216203387392LLU, 5531918402816LLU, 11068131838464LLU, 22136263676928LLU, 44272527353856LLU, 88545054707712LLU, 175990581010432LLU, 70506185244672LLU, 567348067172352LLU, 1416171111120896LLU, 2833441750646784LLU, 5666883501293568LLU, 11333767002587136LLU, 22667534005174272LLU, 45053588738670592LLU, 18049583422636032LLU, 145241105196122112LLU, 362539804446949376LLU, 725361088165576704LLU, 1450722176331153408LLU, 2901444352662306816LLU, 5802888705324613632LLU, 11533718717099671552LLU, 4620693356194824192LLU, 288234782788157440LLU, 576469569871282176LLU, 1224997833292120064LLU, 2449995666584240128LLU, 4899991333168480256LLU, 9799982666336960512LLU, 1152939783987658752LLU, 2305878468463689728LLU, 1128098930098176LLU, 2257297371824128LLU, 4796069720358912LLU, 9592139440717824LLU, 19184278881435648LLU, 38368557762871296LLU, 4679521487814656LLU, 9077567998918656LLU };
constexpr U64 PawnAttacks[2][64] = {
    { 512LLU, 1280LLU, 2560LLU, 5120LLU, 10240LLU, 20480LLU, 40960LLU, 16384LLU, 131072LLU, 327680LLU, 655360LLU, 1310720LLU, 2621440LLU, 5242880LLU, 10485760LLU, 4194304LLU, 33554432LLU, 83886080LLU, 167772160LLU, 335544320LLU, 671088640LLU, 1342177280LLU, 2684354560LLU, 1073741824LLU, 8589934592LLU, 21474836480LLU, 42949672960LLU, 85899345920LLU, 171798691840LLU, 343597383680LLU, 687194767360LLU, 274877906944LLU, 2199023255552LLU, 5497558138880LLU, 10995116277760LLU, 21990232555520LLU, 43980465111040LLU, 87960930222080LLU, 175921860444160LLU, 70368744177664LLU, 562949953421312LLU, 1407374883553280LLU, 2814749767106560LLU, 5629499534213120LLU, 11258999068426240LLU, 22517998136852480LLU, 45035996273704960LLU, 18014398509481984LLU, 144115188075855872LLU, 360287970189639680LLU, 720575940379279360LLU, 1441151880758558720LLU, 2882303761517117440LLU, 5764607523034234880LLU, 11529215046068469760LLU, 4611686018427387904LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU },
    { 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 0LLU, 2LLU, 5LLU, 10LLU, 20LLU, 40LLU, 80LLU, 160LLU, 64LLU, 512LLU, 1280LLU, 2560LLU, 5120LLU, 10240LLU, 20480LLU, 40960LLU, 16384LLU, 131072LLU, 327680LLU, 655360LLU, 1310720LLU, 2621440LLU, 5242880LLU, 10485760LLU, 4194304LLU, 33554432LLU, 83886080LLU, 167772160LLU, 335544320LLU, 671088640LLU, 1342177280LLU, 2684354560LLU, 1073741824LLU, 8589934592LLU, 21474836480LLU, 42949672960LLU, 85899345920LLU, 171798691840LLU, 343597383680LLU, 687194767360LLU, 274877906944LLU, 2199023255552LLU, 5497558138880LLU, 10995116277760LLU, 21990232555520LLU, 43980465111040LLU, 87960930222080LLU, 175921860444160LLU, 70368744177664LLU, 562949953421312LLU, 1407374883553280LLU, 2814749767106560LLU, 5629499534213120LLU, 11258999068426240LLU, 22517998136852480LLU, 45035996273704960LLU, 18014398509481984LLU }
};

constexpr int PosValuePawn[64] = {
	  0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5, 10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int PosValueKnight[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

constexpr int PosValueBishop[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr int PosValueRook[64] = {
	 0,  0,  0,  5,  5,  0,  0,  0
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int PosValueQueen[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

constexpr int PosValueKing_Early[64] = {
	20, 30, 10,  0,  0, 10, 30, 20,
    20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

constexpr int PosValueKing_Late[64] = {
	-50,-30,-30,-30,-30,-30,-30,-50,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -50,-40,-30,-20,-20,-30,-40,-50
};

constexpr std::array<int, 64> IPosValuePawn = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValuePawn[63 - i];
    return s;
}();
constexpr std::array<int, 64> IPosValueBishop  = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValueBishop[63 - i];
    return s;
}();
constexpr std::array<int, 64> IPosValueKnight  = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValueKnight[63 - i];
    return s;
}();
constexpr std::array<int, 64> IPosValueRook  = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValueRook[63 - i];
    return s;
}();
constexpr std::array<int, 64> IPosValueQueen  = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValueQueen[63 - i];
    return s;
}();
constexpr std::array<int, 64> IPosValueKing  = [] {
    auto s = decltype(IPosValuePawn){};
    for (int i = 0; i < 64; i++)
        s[i] = PosValueKing_Early[63 - i];
    return s;
}();

#endif // CONSTANTS
