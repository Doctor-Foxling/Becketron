#include "btpch.h"
#include "PhysXScene.h"

#include "Becketron/Scene/Scene.h"

namespace Becketron {

	PhysXScene::PhysXScene(Scene* scene) : m_Scene(scene)
	{
		//m_PhysXScene = new physx::PxScene();
		if (m_Foundation == nullptr)
		{
			m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);

			if (m_Foundation == nullptr)
			{
				BT_CORE_ERROR("Tried to created!");
				BT_CORE_ASSERT("'m_Foundation' was null!");
			}

		}
		if (m_Cooking)
		{
			physx::PxTolerancesScale ToleranceScale;
			m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(ToleranceScale));
		}
		physx::PxCookingParams cookingParameters = m_Cooking->getParams();
		cookingParameters.meshPreprocessParams.set(physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
		m_Cooking->setParams(cookingParameters);

		if (m_Physics)
		{
			physx::PxTolerancesScale ToleranceScale;
			m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, ToleranceScale);
		}

	}

	void PhysXScene::Update(Timestep ts)
	{
	}

	PhysXScene::~PhysXScene()
	{

	}

}