#include "timer_number.h"

TimerNumber::TimerNumber(ResourceManager& resourceMgr, RID texNumbers, unsigned int digitPrecision) :
	m_resourceMgr(resourceMgr),
	m_texNumbers(texNumbers),
	m_digitPrecision(digitPrecision),
	m_time(0.0f)
{
	const Texture& t = resourceMgr.Get<Texture>(texNumbers);
	m_texWidth = t.GetWidth();
	m_texHeight = t.GetHeight();
	m_anchor = Vec2(0.5f, 0.0f);
}

void TimerNumber::SetTime(float time)
{
	if (time < 0.0f)
		time = -time;
	m_time = time;
}

void TimerNumber::UpdateInternal(float dt)
{

}

void TimerNumber::RenderInternal(Renderer& renderer, Vec2 origin, Vec2 scale)
{
	Texture& numTexture = m_resourceMgr.Get<Texture>(m_texNumbers);
	
	Transform2D numTF;
	numTF.position = origin;
	numTF.scale = SCALE;

	// We need to render each number in series
	// This is kinda a hacky but working way of doing it using math but it works
	int intPart = static_cast<int>(m_time);
	float fracPart = m_time - intPart;
	
	// Process integer part
	int divisor = 1; // determine the biggest pow of 10 (leftmost digit place)
	while (intPart / divisor >= 10)
		divisor *= 10;
	while (divisor > 0)
	{
		int digit = intPart / divisor; // get the digit
		intPart %= divisor;				// move to the next
		divisor /= 10;

		numTexture.SetFrame(digit);
		renderer.DrawTexture(numTF, m_texNumbers);
		numTF.position.x += m_texWidth * SCALE;
	}

	// Draw dot
	numTF.position += Vec2(-m_texWidth * SCALE * 0.5f, 5.0f);
	numTexture.SetFrame(10);
	renderer.DrawTexture(numTF, m_texNumbers);
	numTF.position += Vec2(m_texWidth * SCALE * 0.5f, -5.0f);

	// Process fractional part
	for (unsigned int i = 0; i < m_digitPrecision; i++) 
	{
		fracPart *= 10; // push the first fractional digit past the decimal
		int digit = static_cast<int>(fracPart); // get it
		fracPart -= digit; // then chop it off

		numTexture.SetFrame(digit);
		renderer.DrawTexture(numTF, m_texNumbers);
		numTF.position.x += m_texWidth * SCALE;
	}

	// Reset size, since its dynamic according to
	// the precision & texutre size
	float sizeX = numTF.position.x - origin.x;
	float sizeY = m_texHeight * SCALE;
	m_size = Dim2(0.0f, 0.0f, sizeX, sizeY);
}
