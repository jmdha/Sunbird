#ifndef CHESS_ENGINE_SEARCH_LIMIT
#define CHESS_ENGINE_SEARCH_LIMIT

#include <chrono>
#include <csetjmp>
#include <cstdlib>

namespace Search {
class SearchLimit {
public:
    SearchLimit(std::jmp_buf &jmpBuf, size_t searchTime);
    bool Reached();
    void Exit();

private:
    std::jmp_buf &_jmpBuf;
    std::chrono::steady_clock::time_point _endTime;
};

inline SearchLimit::SearchLimit(std::jmp_buf &jmpBuf, size_t searchTime)
    : _jmpBuf(jmpBuf),
      _endTime(std::chrono::steady_clock().now() + std::chrono::milliseconds(searchTime)) {}

inline bool SearchLimit::Reached() { return _endTime < std::chrono::steady_clock::now(); }

inline void SearchLimit::Exit() { longjmp(_jmpBuf, 1); }
}; // namespace Search

#endif
