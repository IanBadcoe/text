#include "precompiled.h"

#include "Entity.h"

#include "World.h"

std::map<EntityType, EntityCreator::CreateFunc> EntityCreator::s_creation_map;

Terrain::Terrain(std::istringstream& in) : Entity(in)
{
	in >>= _is_walkable;
	in >>= _is_transparent;
}

void Terrain::SerialiseTo(std::ostringstream& out) const
{
	Entity::SerialiseTo(out);

	out <<= _is_walkable;
	out <<= _is_transparent;
}

void Entity::SerialiseTo(std::ostringstream& out) const
{
	out <<= _type;
	out <<= _pos;
}

Entity* EntityCreator::VirtualSerialiseFrom(std::istringstream& in)
{
	EntityType t;

	in >>= t;

	// put the type back for the ctor to read
	for (int i = 0; i < sizeof(EntityType); i++)
	{
		in.unget();
	}

	auto it = s_creation_map.find(t);

	assert(it != s_creation_map.end());

	return it->second(in);
}

void EntityCreator::RegisterCreator(const EntityCreator * ac)
{
    assert(s_creation_map.find(ac->_type) == s_creation_map.end());

    s_creation_map[ac->_type] = ac->_func;
}
