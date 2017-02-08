#include <cassert>
#include <iostream>
using namespace std;

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <mge/core/AbstractGame.hpp>


//////////////////////////////|	INSTANCE MANAGEMENT
//Constructor with position
GameObject::GameObject(std::string pName, glm::vec3 pPosition )
:	_name( pName ), _transform( glm::translate( pPosition ) ),
    _parent(NULL), _children(), _mesh( NULL ),_behaviour( NULL ), _material(NULL), _collider (nullptr)
{
}
//Constructor with position and axis-aligned box collider
GameObject::GameObject (string pName, glm::vec3 pPosition, glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic)
{
	_name = pName;
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_mesh = nullptr;
	_transform = glm::translate (pPosition);
	_collider = new Collider (pHalfExtents, pCenterOffset, pBigBox, pStatic);
}
//Constructor with position and sphere collider
GameObject::GameObject (string pName, glm::vec3 pPosition, float pRadius, glm::vec3 pCenterOffset, bool pStatic)
{
	_name = pName;
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_mesh = nullptr;
	_transform = glm::translate(pPosition);
	_collider = new Collider (pRadius, pCenterOffset, pStatic);
}
//Constructor with mesh
GameObject::GameObject (string pFilename)
{
	_name = pFilename + ":" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_collider = nullptr;
	_mesh = Mesh::load (config::MGE_MODEL_PATH + pFilename);
}
//Constructor with mesh and axis-aligned box collider
GameObject::GameObject (string pFilename, glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic)
{
	_name = pFilename + ":" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_collider = new Collider (pHalfExtents, pCenterOffset, pBigBox, pStatic);
	_mesh = Mesh::load (config::MGE_MODEL_PATH + pFilename);
}
GameObject::GameObject (string pFilename, float pRadius, glm::vec3 pCenterOffset, bool pStatic)
{
	_name = pFilename + ":" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_collider = new Collider (pRadius, pCenterOffset, pStatic);
	_mesh = Mesh::load (config::MGE_MODEL_PATH + pFilename);
}
//Empty constructor
GameObject::GameObject ()
{
	_name = "[X]:" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_mesh = nullptr;
	_collider = nullptr;
}
//Constructor with axis-aligned box collider
GameObject::GameObject (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic)
{
	_name = "[X]:" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_mesh = nullptr;
	_collider = new Collider (pHalfExtents, pCenterOffset, pBigBox, pStatic);
}
//Constructor with sphere collider
GameObject::GameObject (float pRadius, glm::vec3 pCenterOffset, bool pStatic)
{
	_name = "[X]:" + to_string (AbstractGame::Time ());
	_parent = nullptr;
	_behaviour = nullptr;
	_material = nullptr;
	_mesh = nullptr;
	_collider = new Collider (pRadius, pCenterOffset, pStatic);
}
//Destructor
GameObject::~GameObject()
{
	if (_behaviour != nullptr)
	{
		delete _behaviour;
	}
	if (_collider != nullptr)
	{
		delete _collider;
	}

	while (_children.size () > 0)
	{
		GameObject* child = _children [0];
		remove (child);
		delete child;
	}
}

//////////////////////////////|	COLLISIONS
//Add an axis-aligned box collider to this object
void GameObject::addCollider (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox, bool pStatic)
{
	if (_collider != nullptr)
	{
		delete _collider;
	}
	_collider = new Collider (pHalfExtents, pCenterOffset, pBigBox, pStatic);
	if (_dirtyWorldTransform)
	{
		getWorldTransform ();
	}
	_collider->CalculateWorldExtents (_worldTransform);
	_dirtyCollider = false;
}
//Add a sphere collider to this object
void GameObject::addCollider (float pRadius, glm::vec3 pCenterOffset, bool pStatic)
{
	if (_collider != nullptr)
	{
		delete _collider;
	}
	_collider = new Collider (pRadius, pCenterOffset, pStatic);
	if (_dirtyWorldTransform)
	{
		getWorldTransform ();
	}
	_collider->CalculateWorldExtents (_worldTransform);
	_dirtyCollider = false;
}
//Get this object's collider
Collider* GameObject::getCollider ()
{
	return _collider;
}
//Update this object's collider
void GameObject::updateCollider ()
{
	if (_dirtyWorldTransform)
	{
		getWorldTransform ();
	}
	_collider->CalculateWorldExtents (_worldTransform);
	_dirtyCollider = false;
}
//Remove this object's collider
void GameObject::removeCollider ()
{
	if (_collider != nullptr)
	{
		delete _collider;
		_dirtyCollider = true;
	}
}

void GameObject::setName (std::string pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
	_transform = pTransform;
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
	_transform [3] = glm::vec4 (pPosition, 1);
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
}

void GameObject::setWorldPosition (glm::vec3 pPosition)
{
	if (_parent == nullptr)
	{
		_transform [3] = glm::vec4 (pPosition, 1);
	}
	else
	{
		glm::vec3 parPos = _parent->getWorldPosition ();
		_transform [3] = glm::vec4 ((pPosition - parPos), 1);
	}
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}
void GameObject::setMesh (std::string pFilename)
{
	_mesh = Mesh::load (config::MGE_MODEL_PATH + pFilename);
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

void GameObject::setBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviour = pBehaviour;
	_behaviour->setOwner(this);
}

AbstractBehaviour * GameObject::getBehaviour() const
{
    return _behaviour;
}

void GameObject::setParent (GameObject* pParent)
{
	if (pParent == _parent)
	{
		bool oldPartOfWorld = _partOfWorld;
		_partOfWorld = pParent->_partOfWorld;
		if (_partOfWorld != oldPartOfWorld)
		{
			for (int i = _children.size()-1; i >= 0; i-- )
			{
				_children [i]->setParent (this);
			}
		}
		return;
	}

	bool oldPartOfWorld = _partOfWorld;

	//remove from previous parent
	if (_parent != nullptr)
	{
		_parent->_innerRemove (this);
		_parent = nullptr;
	}

	//set new parent
	if (pParent != nullptr)
	{
		_parent = pParent;
		_parent->_innerAdd (this);
		_partOfWorld = _parent->_partOfWorld;
	}
	else
	{
		_partOfWorld = false;
	}

	if (_partOfWorld != oldPartOfWorld)
	{
		for (int i = _children.size()-1; i >= 0; i-- )
		{
			_children [i]->setParent (this);
		}
	}
}

GameObject* GameObject::getParent() {
    return _parent;
}

void GameObject::_innerAdd (GameObject* pChild)
{
	_children.push_back (pChild);
}

void GameObject::_innerRemove (GameObject* pChild)
{
	for (std::vector<GameObject*>::iterator i = _children.begin (); i != _children.end (); ++ i)
	{
		if (*i == pChild)
		{
			_children.erase (i);
			return;
		}
	}
}

void GameObject::add (GameObject* pChild)
{
	pChild->setParent (this);
}

void GameObject::remove (GameObject* pChild)
{
	if (pChild->getParent () != this)
	{
		return;
	}
	pChild->setParent (nullptr);
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition () const
{
	if (_parent == nullptr)
	{
		_worldTransform = _transform;
	}
	else
	{
		if (_dirtyWorldTransform)
		{
			_worldTransform = _parent->getWorldTransform () * _transform;
			_dirtyWorldTransform = false;
		}
	}
	return glm::vec3 (_worldTransform [3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform () const
{
	if (!_dirtyWorldTransform)
	{
		return glm::mat4 (_worldTransform);
	}

	if (_parent == nullptr)
	{
		_worldTransform = _transform;
	}
	else
	{
		_worldTransform = _parent->getWorldTransform () * _transform;
	}
	_dirtyWorldTransform = false;
	return glm::mat4 (_worldTransform);
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	_transform = glm::translate (_transform, pTranslation);
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
	_dirtyCollider = true;
}

void GameObject::scale (float pScale)
{
	_transform = glm::scale (_transform, glm::vec3 (pScale, pScale, pScale));
	_scale *= pScale;
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
	_dirtyCollider = true;
}
void GameObject::badScale (glm::vec3 pScale)
{
	_transform = glm::scale (_transform, pScale);
	_scale *= pScale;
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
	_dirtyCollider = true;
}

glm::vec3 GameObject::getScale ()
{
	return _scale;
}
glm::vec3 GameObject::getWorldScale ()
{
	if (_parent != nullptr)
	{
		return _scale * _parent->getWorldScale ();
	}
	else
	{
		return _scale;
	}
}
float GameObject::getUniformScale ()//Return the approximate total scale
{
	return (_scale.x + _scale.y + _scale.z) / 3;
}
float GameObject::getWorldUniformScale ()
{
	if (_parent != nullptr)
	{
		return (_scale.x + _scale.y + _scale.z) / 3 * _parent->getWorldUniformScale ();
	}
	else
	{
		return (_scale.x + _scale.y + _scale.z) / 3;
	}
}

void GameObject::rotate (float pAngle, glm::vec3 pAxis)
{
	_transform = glm::rotate (_transform, pAngle, pAxis);
	_dirtyWorldTransform = true;
	_updateChildTransforms = true;
	_dirtyCollider = true;
}

//all game objects are updated in a backward loop, first the behaviour is updated, then all children are updated
void GameObject::update (float pStep, bool pUpdateWorldTransform)
{
	if (pUpdateWorldTransform)
	{
		_dirtyWorldTransform = true;
		_updateChildTransforms = true;
		_dirtyCollider = true;
	}

	if (_behaviour) _behaviour->update (pStep);
	if (_dirtyCollider && _collider != nullptr)
	{
		if (_dirtyWorldTransform)
		{
			getWorldTransform ();
		}
		_collider->CalculateWorldExtents (_worldTransform);
		_dirtyCollider = false;
	}

	for (int i = _children.size () - 1; i >= 0; --i )
	{
		_children [i]->update (pStep, _updateChildTransforms);
	}
	_updateChildTransforms = false;
}

int GameObject::getChildCount() {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) {
    return _children[pIndex];
}

