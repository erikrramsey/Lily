#include <ctime>
#include <chrono>

namespace Lily {

    class Clock {
    public:
        Clock();
        unsigned int getDT();
    private:
        long long GetCurrentTimeMillis();
        long long m_currentTimeMillis;

    };

}