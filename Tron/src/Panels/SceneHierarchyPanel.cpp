#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <typeinfo>

#include "Becketron/Scene/Components.h"

#include "ImFileBrowser/imfilebrowser.h"
#include "Becketron/Renderer/Texture.h"

namespace Becketron {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		// Unselect an entity if mouse os clicked somewhere else in the window
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				// TODO: Need to find a better solution than just checking against every component

				if (!m_SelectionContext.HasComponent<CubeRendererComponent>() &&
					!m_SelectionContext.HasComponent<SpriteRendererComponent>() 
					&& ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_SelectionContext.HasComponent<CubeRendererComponent>() &&
					!m_SelectionContext.HasComponent<SpriteRendererComponent>()
					&& ImGui::MenuItem("3D Cube"))
				{
					m_SelectionContext.AddComponent<CubeRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				//if (ImGui::MenuItem("3DTextured Cube"))
				//{
				//	m_SelectionContext.AddComponent<TexturedCubeComponent>();
				//	ImGui::CloseCurrentPopup();
				//}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Scene Options");
		/*if (ImGui::BeginTabBar("Scene options"))
		{*/
			SceneOptions();
			//ImGui::EndTabBar();
		//}
		ImGui::End();
	}

	void SceneHierarchyPanel::SceneOptions()
	{
			ImGui::PushItemWidth(-ImGui::GetFontSize() * 15);
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			static float sz = 36.0f;
			static float thickness = 6.0f;
			static int ngon_sides = 3;
			
			const ImVec2 p = ImGui::GetCursorScreenPos();
			const ImU32 col = ImColor({0.1f, 1.0f, 1.0f, 1.0f});
			const float spacing = 10.0f;
		//	const ImDrawCornerFlags corners_none = 0;
		//	const ImDrawCornerFlags corners_all = ImDrawCornerFlags_All;
		//	const ImDrawCornerFlags corners_tl_br = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotRight;
		//	const int circle_segments = 0;
		//	const int curve_segments = 0;
			float x = p.x + 4.0f;
			float y = p.y + 0.0f;
			
			// center, radius, col, num segments
			draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.4f), sz * 0.4f, col, ngon_sides);               x += sz + spacing * 2.5f;  // N-gon
			draw_list->AddRectFilled(ImVec2(x, y + 5.0f), ImVec2(x + thickness, y + sz * 0.75f), col);                             x += spacing * 1.5f;
			draw_list->AddRectFilled(ImVec2(x, y + 5.0f), ImVec2(x + thickness, y + sz * 0.75f), col);                             x += spacing * 2.0f;
			//ImGui::SameLine();

			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f, 0.4f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f, 0.7f));
			if (ImGui::Button("", { 45.0f, 30.0f }))
			{
				if (m_Context->m_ScenePlay)
					m_Context->m_ScenePlay = false;
				else
					m_Context->m_ScenePlay = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();

			// restart button
			ImGui::SameLine();
			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f, 0.4f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f, 0.7f));
			
			if (ImGui::Button("", { 45.0f, 30.0f }))
			{
				m_Context->m_SceneFirstStart = true;
				if (m_Context->m_SceneRestart) // && !m_Context->m_ScenePlay)
				{
					m_Context->m_SceneRestart = false;
				}
				else
					m_Context->m_SceneRestart = true;
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
			//ImGui::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
			ImGui::PopItemWidth();
			
			//ImGui::AlignTextToFramePadding();
			if (m_Context->m_ScenePlay)
				ImGui::Text(" Pause");
			else
				ImGui::Text(" Play");
			ImGui::SameLine();
			if (!m_Context->m_SceneRestart && m_Context->m_ScenePlay)
				ImGui::Text("Restart");

			if (!m_Context->m_SceneFirstStart && !m_Context->m_SceneRestart)
			{
				ImGui::Text("  Start");
			}
			//else if (m_Context->m_ScenePlay)
			//	ImGui::Text("  -----");

			//ImGui::SameLine();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template <typename T>
	static void AddComponentProps(Entity entity, ImGuiTreeNodeFlags treeNodeFlags, const char* propName) 
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, propName);
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

		if (ImGui::Button("+", ImVec2{ 20, 20 }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}
		ImGui::PopStyleVar();

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			auto& src = entity.GetComponent<T>();
			ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
			ImGui::TreePop();
		}

		if (entity.HasComponent<TexturedCubeComponent>())
		{
			static ImGui::FileBrowser fileDialog;

			fileDialog.SetTitle("Select Texture");
			fileDialog.SetTypeFilters({ ".jpg", ".PNG" });

			if (ImGui::Button("Select Texture"))
				fileDialog.Open();

			fileDialog.Display();

			if (fileDialog.HasSelected())
			{
				Ref<Texture2D> newTex = Texture2D::Create(fileDialog.GetSelected().string());
				auto& src = entity.GetComponent<TexturedCubeComponent>();
				src.texture = newTex;
				BT_TRACE("Image Relative Path: {0}", (fileDialog.GetSelected().relative_path().string()));
				BT_TRACE("Image string: {0}", fileDialog.GetSelected().string());
				BT_TRACE("Image parent path: {0}", fileDialog.GetSelected().parent_path().string());
				BT_TRACE("Image Root Path: {0}", fileDialog.GetSelected().root_path().string());
				BT_TRACE("Image Root name: {0}", fileDialog.GetSelected().root_name().string());
				fileDialog.ClearSelected();
			}
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			// Getting the transforms hash code as a unique id for the tree node
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							// Type casting i to ProjectionType
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float perspNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspNear))
						camera.SetPerspectiveNearClip(perspNear);

					float perspFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspFar))
						camera.SetPerspectiveFarClip(perspFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetPerspectiveNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			AddComponentProps<SpriteRendererComponent>(entity, treeNodeFlags, "Sprite Renderer");
		}

		if (entity.HasComponent<CubeRendererComponent>())
		{
			AddComponentProps<CubeRendererComponent>(entity, treeNodeFlags, "Cube Renderer");
		}

		if (entity.HasComponent<TexturedCubeComponent>())
		{
			AddComponentProps<TexturedCubeComponent>(entity, treeNodeFlags, "Textured Cube");
		}

		if (entity.HasComponent<LightCubeComponent>())
		{
			AddComponentProps<LightCubeComponent>(entity, treeNodeFlags, "Light cube");
		}

	}

}