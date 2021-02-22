#pragma once


namespace Becketron {
	
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		// this is so we can do ( x * ts) rather than ( x * ts.GetSeconds() ) evrtime 
		// So this like a static cast operator where you're adding the ability to cast implicitly (or cast at all)
		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}