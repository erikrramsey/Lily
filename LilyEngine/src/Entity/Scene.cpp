#include "Entity/Scene.h"


namespace Lily {
Scene::Scene() {
	m_camera = create_Lobject();
	m_camera->add_component<Camera>();
	m_camera->set_name("Scene Camera");
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
		for (Entity j = m_registry.get<Family>(i.get_ent()).parent; j != 0; j = m_registry.get<Family>(j).parent) {
			tr = tr * m_registry.get<Transform>(j).get_worldspace();
		}

		tr = glm::translate(tr, i.get_pos());

		tr = glm::rotate(tr, i.get_rot().x, glm::vec3(1, 0, 0));
		tr = glm::rotate(tr, i.get_rot().y, glm::vec3(0, 1, 0));
		tr = glm::rotate(tr, i.get_rot().z, glm::vec3(0, 0, 1));

		tr = glm::scale(tr, i.get_sca());

		i.set_worldspace(tr);
	}

	auto& meshes = m_registry.get_pool<Mesh>();

	for (int i = 0; i < meshes.size(); i++) {
		Renderer::DrawMesh(meshes[i], m_registry.get<Transform>(meshes[i].get_ent()).get_worldspace());
	}
}

void Scene::import_component(Lobject* obj, std::string& path) {
	m_importer->load_model(obj, path);
}

Lobject* Scene::create_Lobject() {
	Lobject* obj = new Lobject(m_registry.create(), this);
	obj->add_component<Transform>();
	obj->add_component<Family>();
	m_objects.emplace(obj->m_entity, obj);
	return obj;
}

void Scene::delete_Lobject(Lobject* obj) {
	auto& fam = obj->get<Family>();
	if (fam.parent) fam.remove_parent(m_registry.get<Family>(fam.parent));
	for (auto i : obj->get_children()) {
		delete_Lobject(i);
	}
	m_registry.delete_entity(obj->m_entity);
	m_objects.erase(obj->m_entity);
	delete obj;
}

Lobject* Scene::get(Entity ent) {
	return m_objects.at(ent);
}

}