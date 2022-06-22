#include "Entity/Scene.h"
#include "Entity/Lobject.h"

namespace Lily {
Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::Init() {
	Renderer::SetClearColor(glm::vec4(1.0f));
    m_root = new Lobject(m_registry.create(), this);
    m_root->add_component<Transform>();
    m_root->add_component<Family>();

    m_camera = new Lobject(m_registry.create(), this);
    m_camera->add_component<Transform>();
    m_camera->add_component<Family>();
    m_camera->add_component<Camera>();
    m_camera->set_name("Scene Camera");
    m_camera->get<Camera>().Initialize(1920, 1080);
}

void Scene::clear() {
    for (auto& [ent, obj] : m_objects) {
        clear_Lobject(obj);
    }
    m_objects.clear();
}

void Scene::update(long long dt) {
	Renderer::Clear();
	Renderer::Begin(m_camera->get<Camera>());
    Renderer::update_viewPos(m_camera->get<Camera>().position);

	auto& children = m_registry.get_pool<Transform>();
	for (auto& i : children) {
		auto tr = glm::mat4(1.0);
        tr = glm::translate(tr, i.get_pos());

        tr = glm::rotate(tr, i.get_rot().x, glm::vec3(1, 0, 0));
        tr = glm::rotate(tr, i.get_rot().y, glm::vec3(0, 1, 0));
        tr = glm::rotate(tr, i.get_rot().z, glm::vec3(0, 0, 1));

        tr = glm::scale(tr, i.get_sca());
        i.set_localspace(tr);

		Entity j = m_registry.get<Family>(i.get_ent()).parent;
        auto& par = m_registry.get<Transform>(j).get_worldspace();
        tr = par * tr;

		i.set_worldspace(tr);
	}

	auto& meshes = m_registry.get_pool<Mesh>();

	for (auto& mesh : meshes) {
        if (mesh.imported)
		    Renderer::DrawMesh(mesh, m_registry.get<Transform>(mesh.get_ent()).get_worldspace());
	}

    auto& lights = m_registry.get_pool<Light>();
    for (auto light : lights) Renderer::update_light(m_registry.get<Transform>(light.get_ent()).get_pos());

}

Lobject* Scene::create_Lobject() {
	auto* obj = new Lobject(m_registry.create(), this);
	obj->add_component<Transform>();
	obj->add_component<Family>();
	m_objects.emplace(obj->m_entity, obj);
    m_root->get<Family>().add_child(obj->get<Family>());
	return obj;
}

void Scene::delete_Lobject(Lobject* obj) {
    if (!obj) return;
	auto& fam = obj->get<Family>();
    auto par = m_registry.try_get<Family>(fam.parent);
    if (par) fam.remove_parent(*par);
	for (auto i : obj->get_children()) {
		delete_Lobject(i);
	}
	m_registry.delete_entity(obj->m_entity);
	m_objects.erase(obj->m_entity);
	delete obj;
    obj = nullptr;
}

void Scene::clear_Lobject(Lobject* obj) {
    auto& fam = obj->get<Family>();
    auto par = m_registry.try_get<Family>(fam.parent);
    if (par && fam.parent != 0) fam.remove_parent(*par);
    m_registry.delete_entity(obj->m_entity);
    delete obj;
    obj = nullptr;
}

Lobject* Scene::get(Entity ent) {
    if (ent == m_root->m_entity) return get_root();
	return m_objects.at(ent);
}

Lobject* Scene::get_root() {
    return m_root;
}

}