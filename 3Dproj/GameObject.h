#pragma once
#include <vector>
#include <string>
#include "vertex.h"
#include "object.h"

#include "Model.h"

//#thisNeedFix
//git
class GameObject : public object{
public:
	GameObject(ModelObj*file, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), std::string otherTex = "");
	virtual ~GameObject();
	
	void draw(Graphics*& gfx, bool sm = false);

	//update vertexConstBuffer on object
	void Updateshaders(Graphics*& gfx, bool vertex = true);

	/*if Disp_map exist and this funciton is not called displaysment tesselation will occur 
	else if tess == true phong tesselation will occur
	else if tess == false it will not tesselate*/
	void setTesselation(bool tess, Graphics*& gfx);

	//gives 2 points 0 = the lowest in x,y and z while 1 is the highest in x,y and zdimensions
	void getBoundingBox(DirectX::XMVECTOR theReturn[]);

	//see if this object has beened drawned before(used for quad tree)
	const bool isDrawed();

	//clear that the object has not been drawned before
	void clearDrawed();
protected:
	ModelObj* model;
private:
	bool tess;
	bool drawed;
};