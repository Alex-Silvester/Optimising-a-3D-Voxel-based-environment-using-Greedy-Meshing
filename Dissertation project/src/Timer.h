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

	long double End()
	{
		m_end_time = std::chrono::high_resolution_clock::now();

		Acc time = std::chrono::duration_cast<Acc>(m_end_time - m_start_time);

		return (long double)time.count() / den;
	}

	long double time()
	{
		Acc time = std::chrono::duration_cast<Acc>(m_end_time - m_start_time);

		return (long double)time.count() / den;
	}

	static const int den = Acc::period::den;

private:

	using time_p = std::chrono::steady_clock::time_point;

	time_p m_start_time;
	time_p m_end_time;

};