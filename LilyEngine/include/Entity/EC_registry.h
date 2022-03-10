#pragma once

#include <vector>
#include <iostream>
#include <unordered_set>

namespace Lily {

typedef unsigned long Entity;

class IPool {
public:
	virtual const std::type_info& type_id() = 0;
};

template <class T>
class Pool : public IPool {
public:
	const int MAX_VALUE = 10'000;

	Pool();
	~Pool();

	virtual const std::type_info& type_id() {
		return typeid(components[0]);
	}

	T& emplace(T comp, Entity ent);
	T& get(Entity ent);
	T* try_get(Entity ent);
	void remove(Entity ent);

	std::vector<T> components;

private:
	long* ids;
	unsigned int m_size;
};

template<class T>
Pool<T>::Pool() {
	ids = new long[MAX_VALUE + 1];
	for (int i = 0; i < MAX_VALUE; i++) {
		ids[i] = -1;
	}

	m_size = 0;
}

template<class T>
Pool<T>::~Pool() {
	delete ids;
}

template<class T>
T& Pool<T>::emplace(T comp, Entity ent) {
	components.push_back(comp);
	ids[ent] = m_size;
	m_size++;
	return components[m_size - 1];
}

template<class T>
T& Pool<T>::get(Entity ent) {
	return components[ids[ent]];
}

template<class T>
T* Pool<T>::try_get(Entity ent) {
	if (ids[ent] == -1)
		return nullptr;
	return &components[ids[ent]];
}


template<class T>
void Pool<T>::remove(Entity ent) {
	components[ids[ent]] = components[ids[m_size - 1]];
	components.pop_back();
	ids[m_size - 1] = -1;
	m_size--;
}

class EC_registry {
public:
	EC_registry();
	~EC_registry();

	// return a unique id belonging to an entity
	Entity create();
	void Delete_Entity(Entity ent);

	template<class T>
	void remove_component(Entity ent);

	// emplace a new component of type t into the vector? of them,
	// if one does not already exist create the vecotr using component_register()
	template<class T>
	T& emplace(Entity ent);
	template<class T>
	T& emplace(Entity ent, T comp);

	// get ref to entity if it exists
	template<class T>
	T& get(Entity ent);

	template<class T>
	T* try_get(Entity ent);

	template<class T>
	std::vector<T>& get_pool();

	template<class T>
	Pool<T>* pool_exists();
private:
	unsigned long m_next_id = 0;
	std::unordered_set<unsigned long> m_ents;

	std::vector<IPool*> m_pools;
	// create a vector of type t to store all components of a type
	template<class T>
	T& component_register(T& type, Entity ent);
};

// T must have a default constructor currently
template<class T>
T& EC_registry::emplace(Entity ent) {
	T comp;
	if (!m_pools.empty()) {
		Pool<T>* pool = pool_exists<T>();
		if (pool != nullptr) {
			return pool->emplace(comp, ent);
		}
	}
	return component_register(comp, ent);
}

template<class T>
T& EC_registry::emplace(Entity ent, T comp) {
	if (!m_pools.empty()) {
		Pool<T>* pool = pool_exists<T>();
		if (pool != nullptr) {
			return pool->emplace(comp, ent);
		}
	}
	return component_register(comp, ent);
}

template<class T>
T& EC_registry::get(Entity ent) {
	return pool_exists<T>()->get(ent);
}

template<class T>
T* EC_registry::try_get(Entity ent) {
	auto pool = pool_exists<T>();
	if (m_pools.empty() || !pool) return nullptr;
	return pool->try_get(ent);
}

template<class T>
void EC_registry::remove_component(Entity ent) {
	Pool<T>* pool = pool_exists<T>();
	pool->remove(ent);
}

template<class T>
std::vector<T>& EC_registry::get_pool() {
	return pool_exists<T>()->components;
}

template<class T>
T& EC_registry::component_register(T& comp, Entity ent) {
	Pool<T>* new_pool = new Pool<T>;
	m_pools.push_back(new_pool);
	return new_pool->emplace(comp, ent);
}

template<class T>
Pool<T>* EC_registry::pool_exists() {
	for (auto pool : m_pools) {
		Pool<T>* test_pool = dynamic_cast<Pool<T>*>(pool);
		if (test_pool != nullptr /*&& test_pool->type_id() == typeid(comp)*/) {
			return test_pool;
		}
	}
	return nullptr;
}
}