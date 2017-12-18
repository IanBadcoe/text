#include "precompiled.h"

#include "Terrain.h"

std::map<uint8_t, uint8_t> Terrain::s_wall_translate;
std::map<uint8_t, uint8_t> Terrain::s_block_translate;

Terrain::Terrain(std::istringstream& in) : Entity(in) {
	in >>= _is_walkable;
	in >>= _is_transparent;
}

void Terrain::SerialiseTo(std::ostringstream& out) const {
	Entity::SerialiseTo(out);

	out <<= _is_walkable;
	out <<= _is_transparent;
}

uint8_t Terrain::GetWallCharacter(uint8_t code) {
	if (s_wall_translate.size() == 0)
		InitWallData();

	return s_wall_translate[code];
}

uint8_t Terrain::GetBlockCharacter(uint8_t code) {
	if (s_block_translate.size() == 0)
		InitBlockData();

	return s_block_translate[code];
}

void Terrain::InitWallData() {
	const uint8_t N = 0x1;
	const uint8_t NE = 0x2;
	const uint8_t E = 0x4;
	const uint8_t SE = 0x8;
	const uint8_t S = 0x10;
	const uint8_t SW = 0x20;
	const uint8_t W = 0x40;
	const uint8_t NW = 0x80;

	// ff is not one of the chars we use here
	for (int i = 0; i < 0x100; i++) {
		uint8_t c = 0xff;

		int num_orths = 0;

		if (i & S) num_orths++;
		if (i & E) num_orths++;
		if (i & N) num_orths++;
		if (i & W) num_orths++;

		if (!num_orths) {
			c = 0xb2;
		} else if (num_orths == 1) {
			if (i & S) {
				c = 0xf1;
			} else if (i & E) {
				c = 0xa0;
			} else if (i & N) {
				c = 0xf2;
			} else if (i & W) {
				c = 0x90;
			} else {
				assert(false);
			}
		} else if (num_orths == 2) {
			if (i & N && i & S) {
				c = 0xf0;
			} else if (i & E && i & W) {
				c = 0xe0;
			} else if (i & S && i & E) {
				c = i & SE ? 0xf4 : 0x91;
			} else if (i & S && i & W) {
				c = i & SW ? 0xf6 : 0x92;
			} else if (i & N && i & E) {
				c = i & NE ? 0xe4 : 0xa1;
			} else if (i & N && i & W) {
				c = i & NW ? 0xe6 : 0xa2;
			} else {
				assert(false);
			}
		} else if (num_orths == 3) {
			if (i & S && i & E && i & N) {
				if (i & SE) {
					c = i & NE ? 0xf3 : 0x9c;
				} else {
					c = i & NE ? 0xac : 0xa5;
				}
			} else if (i & S && i & W && i & N) {
				if (i & SW) {
					c = i & NW ? 0xe3 : 0x9d;
				} else {
					c = i & NW ? 0xad : 0x95;
				}
			} else if (i & W && i & E && i & N) {
				if (i & NW) {
					c = i & NE ? 0xe5 : 0xab;
				} else {
					c = i & NE ? 0xaa : 0xa4;
				}
			} else if (i & S && i & E && i & W) {
				if (i & SE) {
					c = i & SW ? 0xf5 : 0x9a;
				} else {
					c = i & SW ? 0x9b : 0xa3;
				}
			} else {
				assert(false);
			}
		} else {
			int num_diags = 0;

			if (i & SW) num_diags++;
			if (i & SE) num_diags++;
			if (i & NW) num_diags++;
			if (i & NE) num_diags++;

			if (num_diags == 0) {
				c = 0xe1;
			} else if (num_diags == 1) {
				if (i & SE) {
					c = 0xdd;
				} else if (i & NE) {
					c = 0xdc;
				} else if (i & NW) {
					c = 0xdf;
				} else if (i & SW) {
					c = 0xde;
				} else {
					assert(false);
				}
			} else if (num_diags == 2) {
				if (i & SE && i & NW) {
					c = 0xb0;
				} else if (i & NE && i & SW) {
					c = 0xb1;
				} else if (i & SE && i & NE) {
					c = 0x94;
				} else if (i & NE && i & NW) {
					c = 0x93;
				} else if (i & NW && i & SW) {
					c = 0xa6;
				} else if (i & SW && i & SE) {
					c = 0x96;
				} else {
					assert(false);
				}
			} else if (num_diags == 3) {
				if (!(i & SE)) {
					c = 0x99;
				} else if (!(i & NE)) {
					c = 0xa9;
				} else if (!(i & NW)) {
					c = 0xa8;
				} else if (!(i & SW)) {
					c = 0x98;
				} else {
					assert(false);
				}
			} else {
				c = 0xfe;
			}
		}

		assert(c != 0xff);

		s_wall_translate[i] = c;
	}

	assert(s_wall_translate.size() == 0x100);
}

void Terrain::InitBlockData() {
	const uint8_t N = 0x1;
	const uint8_t E = 0x2;
	const uint8_t S = 0x4;
	const uint8_t W = 0x8;

	// ff is not one of the chars we use here
	for (int i = 0; i < 0x10; i++) {
		uint8_t c = 0xff;

		int num = 0;

		if (i & N)
			num++;

		if (i & E)
			num++;

		if (i & S)
			num++;

		if (i & W)
			num++;

		if (num >= 3 || num == 0) {
			c = 0xfe;
		} else if (num == 2) {
			if (i & N && i & S) {
				c = 0xfe;
			} else if (i & E && i & W) {
				c = 0xfe;
			} else if (i & N && i & E) {
				c = 0xae;
			} else if (i & N && i & W) {
				c = 0xaf;
			} else if (i & S && i & E) {
				c = 0x9e;
			} else if (i & S && i & W) {
				c = 0x9f;
			} else {
				assert(false);
			}
		} else if (num == 1) {
			if (i & N) {
				c = 0x1f;
			} else if (i & E) {
				c = 0x11;
			} else if(i & S) {
				c = 0x1e;
			} else if (i & W) {
				c = 0x10;
			} else {
				assert(false);
			}
		}

		assert(c != 0xff);

		s_block_translate[i] = c;
	}

	assert(s_block_translate.size() == 0x10);
}

