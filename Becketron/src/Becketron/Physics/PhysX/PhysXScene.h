#pragma once

#include "Becketron.h"


//#ifdef USE_NVIDIA

#include <physx/PxPhysicsAPI.h>
//#include <physx/PxScene.h>

#include "ErrorCallback.h"

namespace Becketron {

	class Scene;

	class PhysXScene {
	public:
		PhysXScene(Scene* scene);
		~PhysXScene();

		void Update(Timestep ts);

		PhysXErrorCallback m_DefaultErrorCallback;
		physx::PxDefaultAllocator m_DefaultAllocatorCallback;
		static physx::PxFoundation* m_Foundation;
		static physx::PxCooking* m_Cooking;
		static physx::PxPhysics* m_Physics;

	protected:
		physx::PxScene* m_PhysXScene;
		Scene* m_Scene;

	private:
	};
}

//#else
//#error You need to use nvidia for physx!
//#endif 