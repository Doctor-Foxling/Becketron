#pragma once

#include "Becketron.h"
#include "Becketron/Scene/ScriptableEntity.h"

namespace Becketron {
	class RigidbodyController : public ScriptableEntity
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
			auto& r_body = GetComponent<PhysXRigidDynamicComponent>().rigidbody;
			float speed = 150.0f;

			if (Input::IsKeyPressed(BT_KEY_LEFT) && Input::IsKeyPressed(BT_KEY_RIGHT_ALT))
				r_body->SetAngularVelocity({ 0.0f, 0.0f, speed * ts});
			else if (Input::IsKeyPressed(BT_KEY_RIGHT) && Input::IsKeyPressed(BT_KEY_RIGHT_ALT))
				r_body->SetAngularVelocity({ 0.0f, 0.0f, -speed * ts });
			else if (Input::IsKeyPressed(BT_KEY_LEFT))
				r_body->SetLinearVelocity({-speed * ts, 0.0f, 0.0f });
			else if (Input::IsKeyPressed(BT_KEY_RIGHT))
				r_body->SetLinearVelocity({ speed * ts, 0.0f, 0.0f });
			
			if (Input::IsKeyPressed(BT_KEY_DOWN) && Input::IsKeyPressed(BT_KEY_RIGHT_ALT))
				r_body->SetAngularVelocity({ speed * ts, 0.0f, 0.0f });
			else if (Input::IsKeyPressed(BT_KEY_UP) && Input::IsKeyPressed(BT_KEY_RIGHT_ALT))
				r_body->SetAngularVelocity({ -speed * ts, 0.0f, 0.0f });
			else if (Input::IsKeyPressed(BT_KEY_UP))
				r_body->SetLinearVelocity({ 0.0f, speed * ts, 0.0f });
			else if (Input::IsKeyPressed(BT_KEY_DOWN))
				r_body->SetLinearVelocity({ 0.0f, -speed * ts, 0.0f });
			
			if ((Input::IsKeyPressed(BT_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BT_KEY_RIGHT_CONTROL)) && Input::IsKeyPressed(BT_KEY_Z)) //Input::IsKeyPressed(BT_KEY_UP))
				r_body->SetLinearVelocity({ 0.0f, 0.0f, -speed * ts });
			else if ((Input::IsKeyPressed(BT_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BT_KEY_RIGHT_CONTROL)) && Input::IsKeyPressed(BT_KEY_X))
				r_body->SetLinearVelocity({ 0.0f, 0.0f, speed * ts });

			if (Input::IsKeyPressed(BT_KEY_SPACE))
			{
				physx::PxVec3 force = physx::PxVec3(0.0f, speed * ts * 80.0f, 0.0f);
				r_body->AddForce(force);
			}

		}
	};
}