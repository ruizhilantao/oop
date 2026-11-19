// 仅用于本地自测：记录并统计仍然存活的对象，检查程序退出时是否有对象没有释放。
// 该文件放在 tools/ 目录，不参与正式工程的编译（正式工程只编译 src/ 下的文件）。
#include <cstdio>
#include <cstdlib>
#include <new>

namespace {

const int kMaxTracked = 1 << 16;  // 最多同时跟踪 65536 个对象
void* g_alive[kMaxTracked];
long long g_aliveCount = 0;
long long g_newCount = 0;

struct Reporter {
    ~Reporter() {
        std::printf("\n[leak-probe] new=%lld outstanding=%lld\n", g_newCount, g_aliveCount);
        if (g_aliveCount != 0) {
            std::printf("[leak-probe] 有 %lld 个对象未释放，说明存在内存泄漏\n", g_aliveCount);
        }
    }
};

Reporter g_reporter;

}  // namespace

void* operator new(std::size_t size) {
    void* memory = std::malloc(size);
    if (memory == nullptr) {
        throw std::bad_alloc();
    }
    ++g_newCount;
    if (g_aliveCount < kMaxTracked) {
        g_alive[g_aliveCount] = memory;
        ++g_aliveCount;
    }
    return memory;
}

void* operator new[](std::size_t size) {
    return operator new(size);
}

void operator delete(void* memory) noexcept {
    if (memory == nullptr) {
        return;
    }
    for (long long i = g_aliveCount - 1; i >= 0; --i) {
        if (g_alive[i] == memory) {
            g_alive[i] = g_alive[g_aliveCount - 1];
            --g_aliveCount;
            break;
        }
    }
    std::free(memory);
}

void operator delete[](void* memory) noexcept {
    operator delete(memory);
}

void operator delete(void* memory, std::size_t) noexcept {
    operator delete(memory);
}

void operator delete[](void* memory, std::size_t) noexcept {
    operator delete(memory);
}
