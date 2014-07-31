/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef RAY_CAST_H
#define RAY_CAST_H

// This test demonstrates how to use the world ray-cast feature.
// NOTE: we are intentionally filtering one of the polygons, therefore
// the ray will always miss one type of polygon.

// This callback finds the closest hit. Polygon 0 is filtered.
class RayCastClosestCallback : public b2RayCastCallback
{
public:
	RayCastClosestCallback()
	{
		m_hit = false;
	}

	float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{
		b2Body* body = fixture->GetBody();
		void* userData = body->GetUserData();
		if (userData)
		{
			int32 index = *(int32*)userData;
			if (index == 0)
			{
				// filter
				return -1.0f;
			}
		}

		m_hit = true;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;
		m_fixture = fixture;
		return fraction;
	}
	
	bool m_hit;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float32 m_fraction;
	b2Fixture *m_fixture;
};

// This callback finds any hit. Polygon 0 is filtered.
class RayCastAnyCallback : public b2RayCastCallback
{
public:
	RayCastAnyCallback()
	{
		m_hit = false;
	}

	float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{
		b2Body* body = fixture->GetBody();
		void* userData = body->GetUserData();
		if (userData)
		{
			int32 index = *(int32*)userData;
			if (index == 0)
			{
				// filter
				return -1.0f;
			}
		}

		m_hit = true;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;
		m_fixture = fixture;
		return 0.0f;
	}

	bool m_hit;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float32 m_fraction;
	b2Fixture *m_fixture;
};

// This ray cast collects multiple hits along the ray. Polygon 0 is filtered.
class RayCastMultipleCallback : public b2RayCastCallback
{
public:
	enum
	{
		e_maxCount = 3
	};

	RayCastMultipleCallback()
	{
		m_count = 0;
	}

	float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{
		b2Body* body = fixture->GetBody();
		int32 index = 0;
		void* userData = body->GetUserData();
		if (userData)
		{
			int32 index = *(int32*)userData;
			if (index == 0)
			{
				// filter
				return -1.0f;
			}
		}

		b2Assert(m_count < e_maxCount);

		m_points[m_count] = point;
		m_normals[m_count] = normal;
		m_fractions[m_count] = fraction;
		m_fixtures[m_count] = fixture;
		++m_count;

		if (m_count == e_maxCount)
		{
			return 0.0f;
		}

		return 1.0f;
	}

	b2Vec2 m_points[e_maxCount];
	b2Vec2 m_normals[e_maxCount];
	float32 m_fractions[e_maxCount];
	b2Fixture *m_fixtures[e_maxCount];
	int32 m_count;
};

#endif