#pragma once

#include "Becketron/Core/Core.h"
#include <PhysX/PxPhysicsAPI.h>

namespace Becketron {

	class PhysXManager
	{
	public:
		static void Init();
		static void Shutdown();

		static physx::PxSceneDesc CreateSceneDesc();
		static physx::PxScene* CreateScene(const physx::PxSceneDesc& sceneDesc);
		
	public:
		static physx::PxDefaultErrorCallback s_PXErrorCallback;
		static physx::PxDefaultAllocator s_PXAllocator;
		static physx::PxFoundation* s_PXFoundation;
		static physx::PxPhysics* s_PXPhysicsFactory;
		static physx::PxDefaultCpuDispatcher* s_PXDispatcher;
	private:
	};
}