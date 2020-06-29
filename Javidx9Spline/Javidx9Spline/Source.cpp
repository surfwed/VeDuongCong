#include<iostream>
#include<string>



using namespace std;

#include"olcConsoleGameEngine.h"
struct sPoint2D
{
	float x;
	float y;
};
struct sSpline
{
	vector<sPoint2D> points;

	sPoint2D GetSplinePoint(float t, bool bLooped = false)
	{
		// p0 = -x^3 + 2x^2 - x
		// p1 = 3x^3 - 5x^2 + 2
		// p2 = -3x^3 + 4x^2 + x
		// p3 = x^3 - x^2
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}
		
		t = t - (int)t;
		float tt = t * t;
		float ttt = tt * t;

		float q0 = -ttt + 2.0f * tt - t;
		float q1 = 3.0f * ttt - 5.f*tt + 2.0f;
		float q2 = -3.0f * ttt + 4.0f*tt + t;
		float q3 = ttt - tt;

		float tx = 0.5f *(q0 * points[p0].x + q1 * points[p1].x + q2 * points[p2].x + q3 * points[p3].x);
		float ty = 0.5f *(q0 * points[p0].y + q1 * points[p1].y + q2 * points[p2].y + q3 * points[p3].y);

		return { tx, ty };

	}
	sPoint2D GetSplineGradient(float t, bool bLooped = false)
	{
		// p0 = -x^3 + 2x^2 - x
		// p1 = 3x^3 - 5x^2 + 2
		// p2 = -3x^3 + 4x^2 + x
		// p3 = x^3 - x^2
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;
		float tt = t * t;
		float ttt = tt * t;

		float q0 = -3.0f*tt + 4.0f * t - 1;
		float q1 = 9.0f * tt - 10.0f*t ;
		float q2 = -9.0f * tt + 8.0f*t + 1;
		float q3 = 3.0f * tt - 2.0f * t;

		float tx = 0.5f *(q0 * points[p0].x + q1 * points[p1].x + q2 * points[p2].x + q3 * points[p3].x);
		float ty = 0.5f *(q0 * points[p0].y + q1 * points[p1].y + q2 * points[p2].y + q3 * points[p3].y);

		return { tx, ty };
	}
};
class OneLoneCoder_Splines : public olcConsoleGameEngine
{
public:
	OneLoneCoder_Splines()
	{
		m_sAppName = L"Splines";
	}
private:
	sSpline path;
	int nSelectedPoint = 0;
	float fMaker = 0.0f;
protected:
	virtual bool OnUserCreate()
	{
		//path.points = { {10, 5}, {40, 5}, {70, 5}, {80, 5}, {100, 5}, {100, 40}, {80, 40}, {70, 40}, {40, 40}, {10, 40} };
		path.points = { { 10, 41 },{ 20, 41 },{ 30, 41 },{ 40, 41 },{ 50, 41 },{ 60, 41 },{ 70, 41 },{ 80, 41 },{ 90, 41 },{ 100, 41 } };
		return true;
	} 
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		if (m_keys[L'X'].bReleased)
		{
			nSelectedPoint++;
			if (nSelectedPoint >= path.points.size())
				nSelectedPoint = 0;
		}
		if (m_keys[L'Z'].bReleased)
		{
			nSelectedPoint--;
			if (nSelectedPoint < 0)
				nSelectedPoint = path.points.size() - 1;
		}
		if (m_keys[VK_LEFT].bHeld)
			path.points[nSelectedPoint].x -= 30.f * fElapsedTime;
		if (m_keys[VK_RIGHT].bHeld)
			path.points[nSelectedPoint].x += 30.f * fElapsedTime;
		if (m_keys[VK_UP].bHeld)
			path.points[nSelectedPoint].y -= 30.f * fElapsedTime;
		if (m_keys[VK_DOWN].bHeld)
			path.points[nSelectedPoint].y += 30.f * fElapsedTime;
		if (m_keys[L'A'].bHeld)
		{
			fMaker -= 5.0f * fElapsedTime;
		}
		if (m_keys[L'S'].bHeld)
		{
			fMaker += 5.0f * fElapsedTime;
		}
		if (fMaker >= (float)path.points.size()) {
			fMaker -= (float)path.points.size();
		}
		if (fMaker < 0.0f)
		{
			fMaker += (float)path.points.size();
		}
		for (float t = 0.0f; t < (float) path.points.size(); t += 0.005f)
		{
			sPoint2D pos = path.GetSplinePoint(t, true);
			Draw(pos.x, pos.y);
		}
		


		for (int i = 0; i < path.points.size(); i++)
		{
			Fill(path.points[i].x - 1, path.points[i].y - 1, path.points[i].x + 2, path.points[i].y + 2, PIXEL_SOLID, FG_RED);
			DrawString(path.points[i].x, path.points[i].y, to_wstring(i));
		}
		Fill(path.points[nSelectedPoint].x - 1, path.points[nSelectedPoint].y - 1, path.points[nSelectedPoint].x + 2, path.points[nSelectedPoint].y + 2, PIXEL_SOLID, FG_YELLOW);
		DrawString(path.points[nSelectedPoint].x, path.points[nSelectedPoint].y, to_wstring(nSelectedPoint));
		
		sPoint2D p1 = path.GetSplinePoint(fMaker, true);
		sPoint2D g1 = path.GetSplineGradient(fMaker, true);
		float r = atan2(-g1.y, g1.x);
		//DrawLine(10.0f * cos(r) + p1.x, 10.0f * sin(r) + p1.y, -10.0f * cos(r) + p1.x, -10.0f * sin(r) + p1.y, PIXEL_SOLID, FG_BLUE);
		DrawLine(5.0f * sin(r) + p1.x, 5.0f * cos(r) + p1.y, -5.0f * sin(r) + p1.x, -5.0f * cos(r) + p1.y, PIXEL_SOLID, FG_BLUE);		
		
		return true;
	}
};
int main()
{
	OneLoneCoder_Splines demo;
	demo.ConstructConsole(120, 100, 5, 5);
	demo.Start();
	return 0;
}