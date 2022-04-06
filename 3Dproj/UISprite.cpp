#include "UISprite.h"

UISprite::UISprite(ID3D11ShaderResourceView* _sprite, vec2 pos, vec2 size)
	: UIElements(pos, size), sprite(_sprite)
{

}

void UISprite::draw(Graphics*& gfx)
{
	UINT offset = 0;
	static UINT strid = sizeof(UIVertex);
	//gfx->get_IMctx()->VSSetConstantBuffers();
	gfx->get_IMctx()->PSSetShaderResources(0,1,&sprite);
	
	gfx->get_IMctx()->Draw(4, 0);
}
