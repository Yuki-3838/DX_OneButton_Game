#pragma once
#include "GameObject.h"

//ÚG‚µ‚½•ûŒü‚Ì’è‹`
enum class ColRes : unsigned
{
	NONE = 0,//ÚG–³‚µ
	TOP = 1u << 0,//ã‚©‚ç
	BOTTOM = 1u << 1,//‰º‚©‚ç
	LEFT = 1u << 2,//¶‚©‚ç
	RIGHT = 1u << 3,//‰E‚©‚ç
};

namespace Col
{
	inline bool Any(ColRes res) { return res != ColRes::NONE; }
	inline bool IsTop(ColRes res) { return (static_cast<unsigned>(res) & static_cast<unsigned>(ColRes::TOP)) != 0; }
}

//’ZŒ`“¯m‚Ì“–‚½‚è”»’è
ColRes CollisionRect(const GameObject& obj1, const GameObject& obj2);