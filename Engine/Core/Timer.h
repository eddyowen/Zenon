#pragma once

#include <chrono>

namespace zn
{
    class Timer
    {
    public:
        using clock_type = std::chrono::high_resolution_clock;
        using timePoint_type = std::chrono::time_point<clock_type>;
        using duration_type = std::chrono::duration<double>;

        Timer() :
            m_startTime(clock_type::now()), m_running(false) {}

        // Start or restart the timer
        void start()
        {
            m_startTime = clock_type::now();
            m_running = true;
        }

        void Stop()
        {
            if (m_running)
            {
                duration_type elapsed = clock_type::now() - m_startTime;
                m_accumulatedTime += elapsed.count();
                m_running = false;
            }
        }

        void Resume()
        {
            if (!m_running)
            {
                m_startTime = clock_type::now();
                m_running = true;
            }
        }

        void Reset()
        {
            m_startTime = clock_type::now();
            m_accumulatedTime = 0.0;
            m_running = false;
        }

        // In seconds
        double GetElapsedTime() const
        {
            if (m_running)
            {
                duration_type elapsed = clock_type::now() - m_startTime;
                return m_accumulatedTime + elapsed.count();
            }
            
            return m_accumulatedTime;
        }

    private:
        timePoint_type m_startTime;
        double m_accumulatedTime = 0.0; // Accumulated time in seconds when stopped
        bool m_running;
    };
}
