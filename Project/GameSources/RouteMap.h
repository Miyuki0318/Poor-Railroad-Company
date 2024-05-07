/*!
@file RouteMap.h
@brief ˜Hü}(“ïˆÕ“x‘I‘ð)‚ÌÝ’è
@author VÈˆ¨
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class RouteMap : public TemplateObject
	{
		enum eMapLevel
		{
			easyMap,
			normalMap,
			hardMap
		};

		wstring m_mapLevel[3] = {
			L"EASYMAP_TX",
			L"NORMALMAP_TX",
			L"HARDMAP_TX"
		};

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(30.0f, 1.0f, -15.0f);

		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		Mat4x4 m_spanMat;

		Vec3 m_playerPosition;
		Vec3 m_cameraPosition;
		Vec3 m_cameraAt;

		shared_ptr<Sprite> m_mapSprite;
	public:
		RouteMap(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}