#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "Becketron/Physics/PhysicsObject.h"
#include "Becketron/Renderer/Texture.h"

#include "ScriptableEntity.h"
#include "PhysicsEntity.h"

namespace Becketron {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		// copy constructor
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CubeRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		CubeRendererComponent() = default;
		CubeRendererComponent(const CubeRendererComponent&) = default;
		CubeRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct TexturedCubeComponent
	{
		Ref<Texture2D> texture;
		float tiling_factor;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		TexturedCubeComponent() = default;
		TexturedCubeComponent(const TexturedCubeComponent&) = default;
		TexturedCubeComponent(Ref<Texture2D>& tex, float tilingFactor, const glm::vec4& color)
			: texture(tex), tiling_factor(tilingFactor), Color(color) {}
	};

	struct LightCubeComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		LightCubeComponent() = default;
		LightCubeComponent(const LightCubeComponent&) = default;
		LightCubeComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;  // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct PhysicsComponent
	{
		PhysicsObject* physicsObject;

		PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent&) = default;
		PhysicsComponent(PhysicsObject* physObj)
			: physicsObject(physObj) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		// Using function pointers instead of std::function
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			// Since we are not capturing 'this' we simulate that by passeing a pointer to iteself as a paramenter
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	//struct PhysicsComponent
	//{
	//	PhysicsEntity* Instance = nullptr;
	//	using Instantiate_fn = std::function<PhysicsEntity*()>;
	//	Instantiate_fn InstantiatePhysics;
	//	//PhysicsEntity* (*InstantiatePhysics)();
	//	void (*DestroyPhysics)(PhysicsComponent*);

	//	template<typename T>
	//	void Bind(PhysicsObject& physObj)
	//	{
	//		InstantiatePhysics = [physObj]() { return static_cast<PhysicsEntity*>(new T(physObj)); };
	//		// simulating 'this' being captured
	//		DestroyPhysics = [](PhysicsComponent* phy) {delete phy->Instance; phy->Instance = nullptr; };
	//	}
	//};
}