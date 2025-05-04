#pragma once

#include "Core/Base.hpp"

#include <chrono>

namespace zn
{
    namespace Time
    {
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration = std::chrono::duration<f64>;

        static TimePoint GetCurrentTime()
        {
            return Clock::now();
        }
        
        class Timer
        {
        public:
            Timer() :
                m_startTime(Clock::now()), m_running(false) {}

            // Start or restart the timer
            void Start()
            {
                m_startTime = Clock::now();
                m_running = true;
            }

            void Stop()
            {
                if (m_running)
                {
                    Duration elapsed = Clock::now() - m_startTime;
                    m_accumulatedTime += elapsed.count();
                    m_running = false;
                }
            }

            void Resume()
            {
                if (!m_running)
                {
                    m_startTime = Clock::now();
                    m_running = true;
                }
            }

            void Reset()
            {
                m_startTime = Clock::now();
                m_accumulatedTime = 0.0;
                m_running = false;
            }

            // In seconds
            f64 GetElapsedTime() const
            {
                if (m_running)
                {
                    Duration elapsed = Clock::now() - m_startTime;
                    return m_accumulatedTime + elapsed.count();
                }
            
                return m_accumulatedTime;
            }

        private:
            TimePoint m_startTime;
            f64 m_accumulatedTime = 0.0; // Accumulated time in seconds when stopped
            b8 m_running;
        };
    }
}
