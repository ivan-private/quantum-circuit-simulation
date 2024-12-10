#include <chrono>

struct Timer
{
public:
    double duration;

    Timer() { }

    void start()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    double stop()
    {
        m_stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = m_stop - m_start;
        duration = diff.count();
        return duration;
    }


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_stop;
};