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
			float speed = 5.0f;
			float rotationSpeed = 0.01f;

			if (Input::IsKeyPressed(BT_KEY_A))
				translation.x -= speed * ts;
			if (Input::IsKeyPressed(BT_KEY_D))
				translation.x += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_W))
				translation.y += speed * ts;
			if (Input::IsKeyPressed(BT_KEY_S))
				translation.y -= speed * ts;

			if (Input::IsMouseButtonPressed(BT_MOUSE_BUTTON_2))
			{
				rotation.x = Input::GetMouseY() * ts * rotationSpeed;
				//rotation.x += Input::GetMouseY() * ts * rotationSpeed;
				rotation.y = Input::GetMouseX() * ts * rotationSpeed;
				//rotation.y -= Input::GetMouseX() * ts * rotationSpeed;

			}


		}
	};
}