#ifndef TRANSPOSITION_TABLE_HH
#define TRANSPOSITION_TABLE_HH

#include <array>

#include "constants.hh"
#include "move.hh"

#define TABLE_SIZE 8192
#define CLUSTER_SIZE 8

enum class TTFlag : U8 {
    Exact, // Searched all possible moves
    Alpha, // Alpha cut-off
    Beta,   // Beta cut-off
    Undefined
};

struct TTNode {
    U64 zobrist;
    U8 depth;
    TTFlag flag;
    int eval;
    U8 bestMoveIndex;
    Move bestMove;
    Color col;
    TTNode() : flag(TTFlag::Undefined) {};
    TTNode(U64 zobrist, U8 depth, TTFlag flag, int eval, U8 bestMoveIndex, Move bestMove, Color col)
            : zobrist(zobrist), depth(depth), eval(eval), bestMoveIndex(bestMoveIndex), bestMove(bestMove), flag(flag), col(col) {}
    inline bool IsDefined() const { return flag == TTFlag::Undefined; };
};

struct TTCluster {
    std::array<TTNode*, CLUSTER_SIZE> nodes { nullptr };
    ~TTCluster() {
        for (int i = 0; i < count; i++)
            delete nodes.at(i);
    }
    TTNode* Retrieve(U64 zobrist) {
        for (U8 i = 0; i < count; i++)
            if (nodes.at(i)->zobrist == zobrist)
                return nodes.at(i);
        return nullptr;
    }

    void Insert(U64 zobrist, U8 depth, TTFlag flag, int eval, U8 bestMoveIndex, Move bestMove, Color col) {
        if (count < CLUSTER_SIZE) {
            nodes.at(count++) = new TTNode(zobrist, depth, flag, eval, bestMoveIndex, bestMove, col);
            if (depth < lowestDepth) {
                lowestDepth = depth;
                lowestIndex = count - 1;
            }
            return;
        }
        TTNode *rNode = Retrieve(zobrist);
        if (rNode == nullptr) {
            rNode = nodes.at(lowestIndex);
            lowestDepth = depth;
        }

        rNode->zobrist = zobrist;
        rNode->depth = depth;
        rNode->flag = flag;
        rNode->eval = eval;
        rNode->bestMoveIndex = bestMoveIndex;
    }
    inline U8 GetCount() const { return count; };
private:
    U8 count = 0;
    U8 lowestDepth = 9999;
    U8 lowestIndex = 9999;
};

class TranspositionTable {
public:
    struct Stats {
        U64 hits;
        U64 misses;
    };
    ~TranspositionTable() {
#ifdef STATS
        if (stats.misses != 0)
            printf("TT: Hits %f %%\n", (double) stats.hits / (double) stats.misses * 100);
        U64 storedCount = 0;
        for (const auto & cluster : table)
            storedCount += cluster.GetCount();
        printf("TT: Storage used %f %%\n", (double) storedCount / (double) (TABLE_SIZE * CLUSTER_SIZE) * 100);
#endif
    }
    // Retrieves a node with corresponding zobrist, or a nullptr if no such node exists
    inline TTNode* Retrieve(U64 zobrist);
    inline void Insert(U64 zobrist, U8 depth, TTFlag flag, int eval, U8 bestMoveIndex, Move bestMove, Color col);
    inline void Clear();
private:
    Stats stats = Stats();
    std::array<TTCluster, TABLE_SIZE> table;

    static inline U64 GetTableKey(U64 zobrist) { return zobrist % TABLE_SIZE; };
};

TTNode *TranspositionTable::Retrieve(U64 zobrist) {
    const auto node = table.at(GetTableKey(zobrist)).Retrieve(zobrist);
    if (node != nullptr)
        stats.hits++;
    else
        stats.misses++;
    return node;
}

void TranspositionTable::Insert(U64 zobrist, U8 depth, TTFlag flag, int eval, U8 bestMoveIndex, Move bestMove, Color col) {
    table.at(GetTableKey(zobrist)).Insert(zobrist, depth, flag, eval, bestMoveIndex, bestMove, col);
}

void TranspositionTable::Clear() {
    for (auto & cluster : table)
        cluster = TTCluster();
}

#endif //TRANSPOSITION_TABLE_HH
