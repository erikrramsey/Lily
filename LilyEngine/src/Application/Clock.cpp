#include "Application/Clock.h"

namespace Lily {

    Clock::Clock() {
        m_currentTimeMillis = GetCurrentTimeMillis();
    }

    unsigned int Clock::getDT() {
        long long TimeNowMillis = GetCurrentTimeMillis();
        unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
        m_currentTimeMillis = TimeNowMillis;
        return DeltaTimeMillis;
    }

    long long Clock::GetCurrentTimeMillis() {
        // timeval t;
        // gettimeofday(&t, nullptr);
        // long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        return t;
    }

}
