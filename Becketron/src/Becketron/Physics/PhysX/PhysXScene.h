#pragma once

#include "Becketron.h"

//#ifdef USE_NVIDIA

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxFoundation.h>
#include <PhysX/PxScene.h>

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
		static physx::PxDefaultCpuDispatcher* m_Dispatcher;
		static physx::PxCooking* m_Cooking;
		static physx::PxPhysics* m_Physics;
		static physx::PxScene* m_PhysXScene;
	protected:
		Scene* m_Scene;

	private:
	};
}

//#else
//#error You need to use nvidia for physx!
//#endif 