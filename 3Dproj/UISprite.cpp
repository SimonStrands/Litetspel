#include "UISprite.h"

UISprite::UISprite(ID3D11ShaderResourceView* _sprite, vec2 pos, vec2 size)
	: UIElements(pos, size), sprite(_sprite)
{

}
