#include "precompiled.h"

#include "Entity.h"

std::map<EntityType, EntityCreator::CreateFunc>* EntityCreator::s_creation_map = nullptr;


void Entity::SerialiseTo(std::ostringstream& out) const {
	out <<= _type;
	out <<= _pos;
	out <<= _dc;
	out <<= _serialise_order;
}

Entity* EntityCreator::VirtualSerialiseFrom(std::istringstream& in, World* w) {
	EnsureMap();

	EntityType t;

	in >>= t;

	// put the type back for the ctor to read
	for (int i = 0; i < sizeof(EntityType); i++) {
		in.unget();
	}

	auto it = s_creation_map->find(t);

	assert(it != s_creation_map->end());

	return it->second(in, w);
}

void EntityCreator::RegisterCreator(const EntityCreator * ac) {
	EnsureMap();

	assert(s_creation_map->find(ac->_type) == s_creation_map->end());

	(*s_creation_map)[ac->_type] = ac->_func;
}

void EntityCreator::EnsureMap() {
	if (!s_creation_map) {
		s_creation_map = new std::map<EntityType, CreateFunc>();
	}
}

