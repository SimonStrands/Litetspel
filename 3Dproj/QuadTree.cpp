#include "QuadTree.h"
#include "rotation.h"
//size is from mid to outer
static int id = 0;
QuadTree::QuadTree(std::vector<GameObject*>& objectList, vec2 position, int depth, float size, bool fi)
{
	tisid = id++;
	//check so depth is its not the leaf node
	this->position = vec3(position.x, 0, position.y);
	this->depth = depth;
	this->size = size;
	first = fi;
	if (fi) {
		this->ObjectList = objectList;
		qtCD = new QTCamData;
	}
	if (depth != 0) {//else we gonna create quadtree
		//see where the quadtree node is going to be
		vec2 offset;
		for (int i = 0; i < 4; i++) {
			switch (i)
			{
			case 0:
				offset = vec2(position.x + size / 2, position.y + size / 2);//upper höger
				break;
			case 1:
				offset = vec2(position.x + size / 2, position.y - size / 2);//nedre höger
				break;
			case 2:
				offset = vec2(position.x - size / 2, position.y - size / 2);//nedre vänster
				break;
			case 3:
				offset = vec2(position.x - size / 2, position.y + size / 2);//upper vänster
				break;
			}
			nodes[i] = new QuadTree(objectList, offset, depth - 1, size / 2, false);
		}
	}
	else {
		//check if a object is contained in here
		for (int i = 0; i < objectList.size(); i++) {
			DirectX::XMFLOAT4 a[2];
			DirectX::XMVECTOR p[2];
			objectList[i]->getBoundingBox(p);
			DirectX::XMStoreFloat4(&a[0], p[0]);
			DirectX::XMStoreFloat4(&a[1], p[1]);
			if (collision2d(a, position, size)) {//it is inside this quad tree
				//add it to this object list
				std::cout << "added obj at id: " << tisid << std::endl;
				this->ObjectList.push_back(objectList[i]);
				//if its inside we don't remove it cuz it can me in multiple trees at the same time
			}
		}
	}
	
}

void QuadTree::setUpCamProp(float distanceFarPlane)
{
	this->farPlane = distanceFarPlane;
	if (depth != 0) {
		for (int i = 0; i < 4; i++) {
			nodes[i]->sendQTCamData(this->qtCD);
			nodes[i]->setUpCamProp(distanceFarPlane);
		}
	}
}

QuadTree::~QuadTree()
{
	if (first) {
		delete qtCD;
	}
	for (int i = 0; i < 4; i++) {
		delete nodes[i];
	}
}

void QuadTree::draw(Graphics*& gfx, Camera* cam, bool shadowMap)
{
	//get all camera propeties
	vec3 fVector = cam->getForwardVec().Normalize();
	vec3 uVector = cam->getUpVector().Normalize();
	vec3 RVector = cam->getRightVector().Normalize();
	vec3 CamOrgin(cam->getPos());

	//this is wrong (rotate globaly not localy)
	vec3 frustoms[4];//left, right, up, down
	cam->getViewFrustoms(frustoms);

	vec3 leftNorm = frustoms[0].X(uVector).Normalize();//right normal
	vec3 rightNorm = frustoms[1].X(uVector.mirror()).Normalize();

	vec3 UpNorm = frustoms[2].X(RVector.mirror()).Normalize();
	vec3 DownNorm = frustoms[3].X(RVector).Normalize();

	qtCD->CamPos = CamOrgin;
	qtCD->forwardVector = fVector;
	qtCD->LeftNorm = leftNorm;
	qtCD->RightNorm = rightNorm;
	qtCD->UpNorm = UpNorm;
	qtCD->DownNorm = DownNorm;

	Sdraw(gfx, cam, shadowMap);
}

void QuadTree::Sdraw(Graphics*& gfx, Camera* cam, bool shadowMap)
{
	//if we are on the lowest level we are going to draw
	if (depth == 0) {
		for (int i = 0; i < ObjectList.size(); i++) {
			if (!ObjectList[i]->isDrawed()) {
				ObjectList[i]->updateVertexShader(gfx);
				ObjectList[i]->draw(gfx, shadowMap);
			}
		}
	}
	else {//else we check if the other is in view frustum and if they are draw them
		

		//for each node
		for (int i = 0; i < 4; i++) {
			//check if it is to far away with far plane
			nodes[i]->position.y = this->qtCD->CamPos.y;
			if ((this->qtCD->CamPos - nodes[i]->position).length() < farPlane + size) {
				//check if we are inside the node or if the nodes mid is inside
				if (isInsideQuad(nodes[i], this->qtCD->CamPos)) {
					nodes[i]->Sdraw(gfx, cam, shadowMap);
				}
				else {
					//check so point is not behind us
					nodes[i]->position.y = 0;
					nodes[i]->position.y = this->qtCD->CamPos.y + (this->qtCD->forwardVector.y * (this->qtCD->CamPos - nodes[i]->position).length());
					bool done = false;
					for (int qp = 0; qp < 4 && !done; qp++) {
						//watch all four corners
						vec3 offset;
						switch (qp) {
						case 0:
							offset = vec3(nodes[i]->size, 0, nodes[i]->size);
							break;
						case 1:
							offset = vec3(nodes[i]->size, 0, -nodes[i]->size);
							break;
						case 2:
							offset = vec3(-nodes[i]->size, 0, -nodes[i]->size);
							break;
						case 3:
							offset = vec3(-nodes[i]->size, 0, nodes[i]->size);
							break;
						}
						if (pointInFront(nodes[i]->position + offset - this->qtCD->CamPos, this->qtCD->forwardVector)) {
							float ld = (nodes[i]->position + offset - this->qtCD->CamPos) * (this->qtCD->LeftNorm);
							float rd = (nodes[i]->position + offset - this->qtCD->CamPos) * (this->qtCD->RightNorm);
							float ud = -((nodes[i]->position + offset - this->qtCD->CamPos) * (this->qtCD->UpNorm));
							float dd = -((nodes[i]->position + offset - this->qtCD->CamPos) * (this->qtCD->DownNorm));
							//see if that point is inside frustom
							//float Lsize = sqrt(size * size * 2);//make so we don't miss anything
							if (ld < 0 && rd < 0 && ud < 0 && dd < 0) {
								done = true;
								nodes[i]->Sdraw(gfx, cam, shadowMap);
							}
						}
						//else no
					}
				}
			}
		}
	}

}

vec2 QuadTree::getPosition()
{
	return vec2(this->position.x, this->position.y);
}

void QuadTree::clearAlrDraw()
{
	for (int i = 0; i < ObjectList.size(); i++) {
		ObjectList[i]->clearDrawed();
	}
}

bool QuadTree::isInsideQuad(QuadTree *node, vec3 camPos)
{
	if (node->position.x + node->size > camPos.x &&
		node->position.x - node->size < camPos.x &&
		node->position.z + node->size > camPos.z &&
		node->position.z - node->size < camPos.z) {
		return true;
	}
	return false;
}

bool QuadTree::pointInFront(vec3 point, vec3 cam)
{
	if (point * cam > 0) {
		return true;
	}
	return false;
}

void QuadTree::sendQTCamData(QTCamData* qtCD)
{
	this->qtCD = qtCD;
}
