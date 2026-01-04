#pragma once

#include <chrono>

template<typename Acc>
class Timer
{
public:

	Timer() = default;

	void Start()
	{
		m_start_time = std::chrono::high_resolution_clock::now();
	}

	double End()
	{
		m_end_time = std::chrono::high_resolution_clock::now();

		time_t time = std::chrono::duration<time_t, Acc>(m_end_time - m_start_time).count();

		return (double)time / den;
	}

	double time()
	{
		time_t time = std::chrono::duration<time_t, Acc>(m_end_time - m_start_time).count();

		return (double)time / den;
	}

	static const int den = Acc::den;

private:

	using time_p = std::chrono::steady_clock::time_point;

	time_p m_start_time;
	time_p m_end_time;

};