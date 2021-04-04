#include "btpch.h"
#include "Scene.h"

#include "Components.h"
#include "Becketron/Renderer/Renderer2D.h"
#include "Becketron/Renderer/Renderer3D.h"

#include <glm/glm.hpp>

#include "Entity.h"


namespace Becketron {

	
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
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

		// Quad
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);


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
			Renderer3D::BeginScene(mainCamera->GetProjection(), cameraTransform);

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
			Renderer3D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto view = m_Registry.view<TransformComponent, TexturedCubeComponent>();
			for (auto entity : view)
			{
				auto [transform, texCube] = view.get<TransformComponent, TexturedCubeComponent>(entity);

				Renderer3D::DrawCube(transform.GetTransform(), texCube.texture, texCube.tiling_factor, texCube.Color);
			}

			Renderer3D::EndScene();
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

	/*Renderer3D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			for (auto entity : group)
			{

				auto [transform, cube] = group.get<TransformComponent, CubeRendererComponent>(entity);

				Renderer3D::DrawCube(transform.GetTransform(), cube.Color);
			}

			Renderer3D::EndScene();*/

			//			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

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
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Entity entity, PhysicsComponent& component)
	{
		m_PhysEng.AddObject(component.physicsObject);
	}
}

