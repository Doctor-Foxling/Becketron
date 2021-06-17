#pragma once

#include "Becketron.h"
#include "Becketron/Scene/ScriptableEntity.h"

namespace Becketron
{
	class CameraController : public ScriptableEntity
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
			auto& rotation = GetComponent<TransformComponent>().Rotation;
			float speed = 6.0f;
			float rotationSpeed = 1.0f;

			if (Input::IsKeyPressed(BT_KEY_A) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.x -= speed * ts;
			if (Input::IsKeyPressed(BT_KEY_D) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.x += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_W) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.y += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_S) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.y -= speed * ts;
			if (Input::IsKeyPressed(BT_KEY_Q) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.z += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_E) && !Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				translation.z -= speed * ts;

			// ---- rotation

			if (Input::IsKeyPressed(BT_KEY_A) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				rotation.y += rotationSpeed * ts;
			if (Input::IsKeyPressed(BT_KEY_D) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				rotation.y -= rotationSpeed * ts;
			if (Input::IsKeyPressed(BT_KEY_W) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				rotation.x += rotationSpeed * ts;
			if (Input::IsKeyPressed(BT_KEY_S) && Input::IsKeyPressed(BT_KEY_LEFT_CONTROL))
				rotation.x -= rotationSpeed * ts;
			

		}
	};
}