#pragma once

#include <string>
#include <vector>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API Node
	{
	public:
		Node(const std::string& name);
		Node(std::string&& name);
		virtual ~Node();

		bool isDirectChild(Node* node);
		bool isDirectParent(Node* node) { return node->isDirectChild(this); }
		bool isIndirectChild(Node* node);
		bool isIndirectParent(Node* node);

		bool addChild(Node* node);
		void removeChild(Node* node);

		auto& getName() const { return m_Name; }
		auto& getChildren() const { return m_Children; }
		auto& getParents() const { return m_Parents; }

	protected:
		void detachChildren();

	private:
		virtual void onDetached(Node* parent) {}
		virtual void onAttached(Node* parent) {}
		virtual void onChildDetached(Node* child) {}
		virtual void onChildAttached(Node* child) {}

	private:
		std::string        m_Name;
		std::vector<Node*> m_Children;
		std::vector<Node*> m_Parents;
	};
} // namespace MonsterEngine::Renderer::RHI