#include "btpch.h"
#include "Scene.h"

#include "Components.h"
#include "Becketron/Renderer/Renderer2D.h"
#include "Becketron/Renderer/Renderer3D.h"
#include "Becketron/Renderer/Cubemap.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Entity.h"
#include "Becketron/Physics/PhysX/PhysXRigidbody.h"

#include "Becketron/Physics/PhysX/PxHelper.h"

//#define BT_PHYSICS

#define TEMP_TEST

#define USE_PHYSX

//#define USE_OLD_PHYSX

namespace Becketron {

	//Temp makeshift
	static int sceneCount = 0;

#ifdef USE_PHYSX
	//static physx::PxDefaultErrorCallback s_PXErrorCallback;
	//static physx::PxDefaultAllocator s_PXAllocator;
	//static physx::PxFoundation* s_PXFoundation;

	//static physx::PxPhysics* s_PXPhysicsFactory;

	static physx::PxScene* m_PxScene = NULL;

	static physx::PxMaterial* gMaterial = NULL;

	struct SceneComponent
	{
		int SceneID = sceneCount;
	};

	struct PhysXSceneComponent
	{
		physx::PxScene* World;
	};
#endif

	Scene::Scene()
	{
		//m_SceneEntity = { m_Registry.create(), this };
		//m_Registry.emplace<SceneComponent>(m_SceneEntity, sceneCount);

#ifdef USE_PHYSX
		gMaterial = PhysXManager::s_PXPhysicsFactory->createMaterial(0.5F, 0.5F, 0.6F);
		physx::PxSceneDesc sceneDesc = PhysXManager::CreateSceneDesc();
		sceneDesc.gravity = physx::PxVec3(0.0F, -9.8F, 0.0F);
		PhysXManager::s_PXDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = PhysXManager::s_PXDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_PxScene = PhysXManager::CreateScene(sceneDesc);
		BT_CORE_ASSERT(m_PxScene);
		m_Ground = PxCreatePlane(*PhysXManager::s_PXPhysicsFactory, physx::PxPlane(0, 1, 0, 3), *gMaterial);
		m_PxScene->addActor(*m_Ground);
		sceneCount++;
#endif

	}

	Scene::~Scene()
	{
		m_Registry.clear();
	}

	void Scene::OnShutdown()
	{
#ifdef USE_PHYSX
		//auto physxView = m_Registry.view<PhysXSceneComponent>();
		m_PxScene->release();
#endif
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

#ifdef USE_PHYSX
	// temp physx

	/*
	static physx::PxRigidDynamic* createDynamic(const physx::PxTransform& t,
		const physx::PxGeometry& geometry, const physx::PxVec3& velocity = physx::PxVec3(0))
	{
		physx::PxRigidDynamic* dynamic = physx::PxCreateDynamic(*PhysXManager::s_PXPhysicsFactory, t, geometry, *gMaterial, 10.0f);
		dynamic->setAngularDamping(0.5f);
		dynamic->setLinearVelocity(velocity);
		m_PxScene->addActor(*dynamic);
		return dynamic;
	}
	*/


#endif

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };

					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});
		}

#ifdef USE_OLD_PHYSX
		if (m_SceneRestartLast != m_SceneRestart)
		{
			m_SceneRestartLast = m_SceneRestart;

			auto view = m_Registry.view<PhysXRigidbodyComponent, TransformComponent>();

			for (const auto& entity : view)
			{
				auto [tc, rb] = view.get<TransformComponent, PhysXRigidbodyComponent>(entity);

				//tc.Position = pc.Position;

				BT_CORE_INFO("x {0}", rb.m_body->GetPos().x);
				BT_CORE_INFO("y {0}", rb.m_body->GetPos().y);
				BT_CORE_INFO("z {0}", rb.m_body->GetPos().z);

				tc.Translation = rb.m_body->GetPos();
			}
		}
#endif

#ifdef BT_PHYSICS
		//{
		//	m_Registry.view<PhysicsComponent>().each([=](auto entity, auto& phy)
		//	{
		//		if (!phy.Instance)
		//		{
		//			phy.Instance = phy.InstantiatePhysics();
		//			phy.Instance->m_Entity = Entity{ entity, this };

		//			phy.Instance->OnCreate();
		//		}
		//		phy.Instance->OnUpdate(ts);
		//	});
		//}

		if (m_SceneRestartLast != m_SceneRestart)
		{
			m_SceneRestartLast = m_SceneRestart;

			auto view = m_Registry.view<PhysicsComponent>();
			if (!m_SceneRestart)
			{
				//m_ScenePlay = true;
				for (auto entity : view)
				{
					auto& physComp = view.get<PhysicsComponent>(entity);

					auto& phyObj = physComp.physicsObject;
					//phyObj->m_play = m_ScenePlay;
					phyObj->m_restart = m_SceneRestart;
				}
			}
			else
			{
				//m_ScenePlay = false;
				m_ScenePlay = true;
				for (auto entity : view)
				{
					auto& physComp = view.get<PhysicsComponent>(entity);

					auto& phyObj = physComp.physicsObject;
					//phyObj->m_play = m_ScenePlay;
					phyObj->m_restart = m_SceneRestart;
				}
			}
		}

		if (m_ScenePlayLast != m_ScenePlay)
		{
			m_ScenePlayLast = m_ScenePlay;
			if (m_SceneRestart)
				m_SceneRestart = false;
			auto view = m_Registry.view<PhysicsComponent>();
			for (auto entity : view)
			{
				auto& physComp = view.get<PhysicsComponent>(entity);

				auto& phyObj = physComp.physicsObject;
				phyObj->m_play = m_ScenePlay;
			}
		}

		if (m_ScenePlay)
		{
			auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
			for (auto entity : view)
			{
				auto [transform, physComp] = view.get<TransformComponent, PhysicsComponent>(entity);

				auto& phyObj = physComp.physicsObject;
				//phyObj.Integrate(ts);
				if (!physComp.controllable)
				{
					transform.Translation = phyObj->GetPosition();
					transform.Scale = phyObj->GetScale();
				}
				else
				{
					phyObj->SetPosition(transform.Translation);
					phyObj->SetScale(transform.Scale);
				}

				// ---- Bounding Spheres
				//float cube_side = phyObj->GetRadius() * glm::root_two<float>();
				//transform.Scale = glm::vec3(cube_side);
			}
		}

		// Update PhysicsEngine
		//if (m_ScenePlay && m_ScenePlay == m_ScenePlayLast)
		{
			m_PhysEng.Simulate(ts);
			m_PhysEng.HandleCollision(PhysicsEngine::CollisionType::AABB);
		}
		
#endif

#ifdef USE_PHYSX
		{
			//auto physxView = m_Registry.view<PhysXSceneComponent>();
			//physx::PxScene* physxScene = m_Registry.get<PhysXSceneComponent>(physxView.front()).World;
			if (m_PxScene)
			{
				if (m_ScenePlay)
				{
					m_PxScene->simulate(ts);
					m_PxScene->fetchResults(true);
				}

				auto view = m_Registry.view<TransformComponent, PhysXRigidDynamicComponent>();
				for (auto entity : view)
				{
					auto [transform, physX_body] = view.get<TransformComponent, 
														PhysXRigidDynamicComponent>(entity);
					
					glm::quat q = physX_body.rigidbody->GetRot();
					glm::vec3 euler = glm::eulerAngles(q);

					if (m_SceneRestart)
						physX_body.rigidbody->BodyReset();
					
					transform.Translation = physX_body.rigidbody->GetPos();
					transform.Rotation = euler;

					// Update rigidbody prperties 
					auto& rb = physX_body.rigidbody;

					if (physX_body.scale != transform.Scale)
					{
						rb->SetScale(transform.Scale);
						physX_body.scale = transform.Scale;
					}

					if (physX_body.old_staticFriction != physX_body.staticFriction ||
						physX_body.old_dynamicFriction != physX_body.dynamicFriction ||
						physX_body.old_restitution != physX_body.restitution)
					{
						physx::PxMaterial* mat = PhysXManager::s_PXPhysicsFactory->createMaterial(physX_body.staticFriction,
							physX_body.dynamicFriction, physX_body.restitution);

						rb->SetMaterial(mat);

						physX_body.old_staticFriction = physX_body.staticFriction;
						physX_body.old_dynamicFriction = physX_body.dynamicFriction;
						physX_body.old_restitution = physX_body.restitution;
					}

					if (physX_body.old_mass != physX_body.mass)
					{
						rb->SetMass(physX_body.mass);
						physX_body.old_mass = physX_body.mass;
					}

					if (physX_body.old_density != physX_body.density)
					{

						rb->SetDensity(physX_body.density);
						physX_body.old_density = physX_body.density;
					}

					/*if (physX_body.old_linearVelocity != physX_body.linearVelocity)
					{
						physx::PxVec3 vel = PxHelper::Vec3_glmToPx(physX_body.linearVelocity, ts);

						rb->SetLinearVelocity(vel);
						physX_body.old_linearVelocity = physX_body.linearVelocity;
					}*/


					// Update linear velocity
					if (physX_body.linearVelocity != glm::vec3(0.0f))
					{
						rb->SetLinearVelocity(PxHelper::Vec3_glmToPx(physX_body.linearVelocity));
					}
					
					// Update angular velocity
					if (physX_body.angularVelocity != glm::vec3(0.0f))
					{
						rb->SetAngularVelocity(PxHelper::Vec3_glmToPx(physX_body.angularVelocity));
					}

					if (physX_body.gravity_effect)
						rb->EnableGravity(false);
					else
						rb->EnableGravity(true);

				}

				// If reset button was pressed, set back to inactive
				if (m_SceneRestart)
					m_SceneRestart = false;
			}
		}
#endif
		// --- Renderering ---

		// Camera
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		// Cubemap
		if (mainCamera)
		{
			auto view = m_Registry.view<CubemapComponent>();

			for (auto entity : view)
			{
				Cubemap::RenderSkybox(*mainCamera, cameraTransform);
			}
		}

		// Quad
		if (mainCamera)
		{
		 	Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}

		// Normal Cube
		if (mainCamera)
		{
			Renderer3D::BeginScene(*mainCamera, cameraTransform);

			auto view = m_Registry.view<TransformComponent, CubeRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, cube] = view.get<TransformComponent, CubeRendererComponent>(entity);

				Renderer3D::DrawCube(transform.GetTransform(), cube.Color);
			}

			Renderer3D::EndScene();

		}

		// Textured Cube
		if (mainCamera)
		{
			Renderer3D::BeginScene(*mainCamera, cameraTransform);

			auto view = m_Registry.view<TransformComponent, TexturedCubeComponent>();
			for (auto entity : view)
			{
				auto [transform, texCube] = view.get<TransformComponent, TexturedCubeComponent>(entity);

				Renderer3D::DrawCube(transform.GetTransform(), texCube.texture, texCube.tiling_factor, texCube.Color);
			}

			Renderer3D::EndScene();
		}

		// Textured Sprite
				// Textured Cube
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto view = m_Registry.view<TransformComponent, TexturedSpriteComponent>();
			for (auto entity : view)
			{
				auto [transform, texSprite] = view.get<TransformComponent, TexturedSpriteComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), texSprite.Texture, texSprite.TilingFactor, texSprite.Color);
			}

			Renderer2D::EndScene();
		}

		// Light Cube
		if (mainCamera)
		{
			auto view = m_Registry.view<TransformComponent, LightCubeComponent>();
			for (auto entity : view)
			{
				auto [transform, light] = view.get<TransformComponent, LightCubeComponent>(entity);

				Renderer3D::ShowLightCube(transform.GetTransform(), light.Color, mainCamera->GetProjection(), cameraTransform);
			}

		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		//component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CubeRendererComponent>(Entity entity, CubeRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TexturedCubeComponent>(Entity entity, TexturedCubeComponent& component)
	{
		if (component.texture == nullptr)
		{
			component.texture = Texture2D::Create("assets/textures/Chess_board.jpg");
		}
	}

	template<>
	void Scene::OnComponentAdded<TexturedSpriteComponent>(Entity entity, TexturedSpriteComponent& component)
	{
		if (component.Texture == nullptr)
		{
			component.Texture = Texture2D::Create("assets/textures/Rock1.jpg");
		}
	}

	template<>
	void Scene::OnComponentAdded<LightCubeComponent>(Entity entity, LightCubeComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CubemapComponent>(Entity entity, CubemapComponent& component)
	{
		if (component.Changed)
			Cubemap::ChangeSkybox(component.Faces);
	}


#ifdef USE_OLD_PHYSX
	template<>
	void Scene::OnComponentAdded<PhysXRigidbodyComponent>(Entity entity, PhysXRigidbodyComponent& component)
	{
	}
#endif

#ifdef USE_PHYSX
	template<>
	void Scene::OnComponentAdded<PhysXSceneComponent>(Entity entity, PhysXSceneComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<PhysXRigidDynamicComponent>(Entity entity, PhysXRigidDynamicComponent& component)
	{
		const glm::mat4& transform = m_Registry.get<TransformComponent>(entity).GetTransform();
		auto [pos, rot, scale] = PxHelper::GetTransformDecomposition(transform);

		if (component.material)
			component.rigidbody = CreateRef<PhysXRigidbody>(pos, rot, scale, component.material);
		else
		{
			component.material = PhysXManager::s_PXPhysicsFactory->createMaterial(component.staticFriction,
				component.dynamicFriction, component.restitution);
			
			component.rigidbody = CreateRef<PhysXRigidbody>(pos, rot, scale, component.material);
		}


		m_PxScene->addActor(*component.rigidbody->GetRigidbody());

		//component.rigidbody->SetKinematic(component.isKinematic);
	}

#endif

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

#ifdef BT_PHYSICS

	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Entity entity, PhysicsComponent& component)
	{
		m_PhysEng.AddObject(component.physicsObject);
	}

#endif

}

