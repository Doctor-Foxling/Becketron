#pragma once

#include "Becketron.h"
#include "Becketron/Scene/ScriptableEntity.h"

namespace Becketron {
	class CubeController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{
			//auto& translation = GetComponent<TransformComponent>().Translation;
			//translation.x = rand() % 10 - 5.0f;
		}

		virtual void OnDestroy() override
		{
		}

		virtual void OnUpdate(Timestep ts) override
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 5.0f;

			if (Input::IsKeyPressed(BT_KEY_LEFT))
				translation.x -= speed * ts;
			else if (Input::IsKeyPressed(BT_KEY_RIGHT))
				translation.x += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_UP))
				translation.y += speed * ts;
			else if (Input::IsKeyPressed(BT_KEY_DOWN))
				translation.y -= speed * ts;
			if ((Input::IsKeyPressed(BT_KEY_LEFT_CONTROL)|| Input::IsKeyPressed(BT_KEY_RIGHT_CONTROL)) && Input::IsKeyPressed(BT_KEY_UP))
				translation.z -= speed * ts;
			if ((Input::IsKeyPressed(BT_KEY_LEFT_CONTROL)|| Input::IsKeyPressed(BT_KEY_RIGHT_CONTROL)) && Input::IsKeyPressed(BT_KEY_DOWN))
				translation.z += speed * ts;


		}
	};
}