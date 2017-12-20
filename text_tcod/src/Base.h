#pragma once

#include "Terrain.h"

#include <sstream>

class Player;

class Base : public Terrain {
public:
	Base(std::istringstream& in, const CreatorArg& ca);
	Base(int id) : Terrain(EntityType::Base, false, true), _player_in(false), _id(id) {}

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;
	virtual bool DrawCompatWith(const Terrain* other) const override;

	void SetPlayer(const Player* p) {
		_player = p;
		FixDisplay();
	}

	void FixDisplay();

	int GetId() const;
private:
    static EntityCreator s_creator;

	const Player* _player;
	bool _player_in;

	int _id;
};

class BaseEdge : public Terrain {
public:
	BaseEdge(std::istringstream& in, const CreatorArg& ca);
	BaseEdge(const Base* base, Coord::Dir d);

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;
	virtual bool DrawCompatWith(const Terrain* other) const override;

private:
	static EntityCreator s_creator;

	const Base* _base;
	void SetBase(const Base* base);
};
