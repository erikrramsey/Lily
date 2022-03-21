#include "Entity/Scene.h"
#include "Entity/Lobject.h"


namespace Lily {
Scene::Scene() {
	m_camera = create_Lobject();
	m_camera->add_component<Camera>();
	m_importer = new Importer(this);
}

Scene::~Scene() {
	delete m_importer;
}

void Scene::Init() {
	Renderer::SetClearColor(glm::vec4(1.0f));
}

void Scene::update(long long dt) {
	Renderer::Clear();
	Renderer::Begin(m_camera->get<Camera>());

	auto& children = m_registry.get_pool<Transform>();
	for (auto& i : children) {
		auto tr = glm::mat4(1.0);
		tr = glm::translate(tr, i.get_pos());
		for (Entity j = i.get_parent(); j != 0; j = m_registry.get<Transform>(j).get_parent()) {
			tr *= m_registry.get<Transform>(j).get_worldspace();
		}

		i.set_worldspace(tr);
	}

	auto& meshes = m_registry.get_pool<Mesh>();

	for (int i = 0; i < meshes.size(); i++) {
		Renderer::DrawMesh(meshes[i], m_registry.get<Transform>(meshes[i].get_ent()).get_worldspace());
	}
}

void Scene::load(std::string& path) {
	m_importer->load_model(path);
}

Lobject* Scene::create_Lobject() {
	Lobject* obj = new Lobject(m_registry.create(), this);
	obj->add_component<Transform>();
	m_objects.push_back(obj);
	return obj;
}

void Scene::delete_Lobject(Lobject* obj) {
	m_registry.delete_entity(obj->m_entity);
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj));
	delete obj;
}
}