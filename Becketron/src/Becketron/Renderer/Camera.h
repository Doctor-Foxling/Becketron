#pragma once

#include <glm/glm.hpp>

namespace Becketron {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		// Once per frame when we calculate where our camera is
		// we want to recalculate this view matrix
		glm::mat4 m_ViewMatrix;
		// Might need a cached view projection matrix so u dont do a multiplication everytime 
		glm::mat4 m_ViewProjectionMatrix;
		// You don't really rotate an orthographic camera, you can rotate it along the Z-axis
		// So we will just store that as a float rather than a vec3
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};
}