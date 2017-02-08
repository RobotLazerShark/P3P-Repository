#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <mge/core/Collider.hpp>
#include <mge/config.hpp>

class AbstractBehaviour;
class AbstractMaterial;
class World;
class Mesh;

/**
 * A GameObject wraps all data required to display an (interactive / dynamic) object, but knows nothing about OpenGL or rendering.
 * You will need to alter this class to add colliders etc.
 */
class GameObject
{
	public:
		GameObject (std::string pName, glm::vec3 pPosition);
		GameObject (std::string pName, glm::vec3 pPosition, glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox = true, bool pStatic = true);
		GameObject (std::string pName, glm::vec3 pPosition, float pRadius, glm::vec3 pCenterOffset, bool pStatic = true);
		GameObject (std::string pFilename);
		GameObject (std::string pFilename, glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox = true, bool pStatic = true);
		GameObject (std::string pFilename, float pRadius, glm::vec3 pCenterOffset, bool pStatic = true);
		GameObject ();
		GameObject (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox = true, bool pStatic = true);
		GameObject (float pRadius, glm::vec3 pCenterOffset, bool pStatic = true);
		virtual ~GameObject();
		void addCollider (glm::vec3 pHalfExtents, glm::vec3 pCenterOffset, bool pBigBox = true, bool pStatic = true);
		void addCollider (float pRadius, glm::vec3 pCenterOffset, bool pStatic = true);
		Collider* getCollider ();
		void updateCollider ();
		void removeCollider ();

		void setName (std::string pName);
		std::string getName () const;

		void setTransform (const glm::mat4& pTransform);
		const glm::mat4& getTransform () const;
		void setLocalPosition (glm::vec3 pPosition);
		void setWorldPosition (glm::vec3 pPosition);
		glm::vec3 getLocalPosition () const;
		glm::vec3 getWorldPosition () const;
		glm::mat4 getWorldTransform () const;
		void translate (glm::vec3 pTranslation);
		void rotate (float pAngle, glm::vec3 pAxis);
		void scale (float pScale);//Only allow uniform scaling (for normal calculations)
		void badScale (glm::vec3 pScale);//Allow for non-uniform scaling. The naming should make clear it shouldn't be used carelessly.
		glm::vec3 getScale ();//Tracking the actual scale value more easily
		glm::vec3 getWorldScale ();
		float getUniformScale ();
		float getWorldUniformScale ();
		void setMesh (Mesh* pMesh);
		void setMesh (std::string pFilename);
		Mesh* getMesh () const;
		void setMaterial (AbstractMaterial* pMaterial);
		AbstractMaterial* getMaterial () const;
		void setBehaviour (AbstractBehaviour* pBehaviour);
		AbstractBehaviour* getBehaviour () const;
		virtual void update (float pStep, bool pUpdateWorldTransform = false);
		void add (GameObject* pChild);
		void remove (GameObject* pChild);
		virtual void setParent (GameObject* pGameObject);
		GameObject* getParent ();
		int getChildCount ();
		GameObject* getChildAt (int pIndex);
	protected:
		Collider* _collider;
		std::string _name = "";
		glm::mat4 _transform;
		GameObject* _parent;
		std::vector<GameObject*> _children;
		Mesh* _mesh;
		AbstractBehaviour* _behaviour;
		AbstractMaterial* _material;
		bool _partOfWorld = false;
		void _innerAdd (GameObject* pChild);
		void _innerRemove (GameObject* pChild);
	    private:
		mutable bool _dirtyWorldTransform;
		bool _dirtyCollider = false;
		mutable glm::mat4 _worldTransform;
		bool _updateChildTransforms = false;
		glm::vec3 _scale = glm::vec3 (1, 1, 1);
		GameObject (const GameObject&);
		GameObject& operator= (const GameObject&);
};

#endif // GAMEOBJECT_H
