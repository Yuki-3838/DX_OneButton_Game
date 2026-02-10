#include "Collision.h"
#include <algorithm>

ColRes CollisionRect(const GameObject& obj1, const GameObject& obj2)
{
    DirectX::XMFLOAT2 aPos = obj1.GetPosition();
    DirectX::XMFLOAT2 bPos = obj2.GetPosition();
    
    DirectX::XMFLOAT2 aSize = obj1.GetSize();
    DirectX::XMFLOAT2 bSize = obj2.GetSize();

    //AABBîªíË
    if(aPos.x + aSize.x < bPos.x ||
	   aPos.x > bPos.x + bSize.x ||
	   aPos.y + aSize.y < bPos.y ||
	   aPos.y > bPos.y + bSize.y)
	{
		return ColRes::NONE;
	}

    //è’ìÀÇµÇƒÇ¢ÇÈèÍçáÅAè’ìÀï˚å¸ÇîªíË
    float overlapLeft   = (aPos.x + aSize.x) - bPos.x;
    float overlapRight  = (bPos.x + bSize.x) - aPos.x;
    float overlapTop    = (aPos.y + aSize.y) - bPos.y;
    float overlapBottom = (bPos.y + bSize.y) - aPos.y;

    float minOverlap = (std::min)({overlapLeft, overlapRight, overlapTop, overlapBottom});

    if(minOverlap == overlapTop)return ColRes::TOP;
	if(minOverlap == overlapBottom)return ColRes::BOTTOM;
	if(minOverlap == overlapLeft)return ColRes::LEFT;

	return ColRes::RIGHT;
}
