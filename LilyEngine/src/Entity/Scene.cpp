#include "Entity/Scene.h"
#include "Entity/UObject.h"


namespace Lily {
Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::Init() {
	Renderer::SetClearColor(glm::vec4(1.0f));
}

void Scene::Update(long long dt) {
	Renderer::SetClearColor(glm::vec4(1.0, 0.5, 0.5, 1.0));
	Renderer::Clear();
	Renderer::Begin(m_camera);

	// fix this, grabing every transform, not just those with a mesh
	auto& meshes = m_registry.get_pool<Mesh_component>();

	for (int i = 0; i < meshes.size(); i++) {
		Renderer::DrawMesh(meshes[i]);
	}
}

UObject* Scene::create_UObject() {
	UObject* obj = new UObject(m_registry.create(), this);
	obj->add_component<Transform_component>();
	m_objects.push_back(obj);
	return obj;
}

void Scene::delete_UObject(UObject* obj) {


}
}