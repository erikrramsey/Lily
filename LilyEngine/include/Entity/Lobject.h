#pragma once

#include <vector>
#include <string>

#include "Component.h"
#include "Scene.h"

namespace Lily {

typedef unsigned long Entity;

// API to access an entity and it's components
class Scene;
class Lobject {
public: 
	Lobject(Entity id, Scene* scene);
	~Lobject();

	const char* get_name() { return m_name.c_str(); }
	void set_name(const char* name) { m_name = name; }

	std::vector<Lobject*> get_children();

	template<class T>
	T& add_component(T& comp);

	template<class T>
	T& add_component();

	template<class T>
	T& get();

	template<class T>
	T* try_get_component();

	template<class T>
	void remove_component();

	std::string m_name;
	Entity m_entity;
	Scene* m_scene;
};


template<class T>
T& Lobject::add_component(T& comp) {
	if (try_get_component<T>() != nullptr) {
		std::cout << "Object already has component" << std::endl;
		return get<T>();
	}
	m_scene->m_registry.emplace(m_entity, comp);
	return get<T>();
}

template<class T>
T& Lobject::add_component() {
	if (try_get_component<T>() != nullptr) {
		std::cout << "Object already has component" << std::endl;
		return get<T>();
	}
	T comp(m_entity);
	m_scene->m_registry.emplace(m_entity, comp);
	return get<T>();
}

template<class T>
T& Lobject::get() {
	return m_scene->m_registry.get<T>(m_entity);
}

template<class T>
T* Lobject::try_get_component() {
	return m_scene->m_registry.try_get<T>(m_entity);
}

template<class T>
void Lobject::remove_component() {
	m_scene->m_registry.remove_component<T>(m_entity);
}

}

