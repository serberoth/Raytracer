#ifndef __NEW_SCENE_H_
#define __NEW_SCENE_H_

#pragma once
/*---------------------------------------------------------------------------*/

typedef std::vector<Entity *, gc_allocator<Entity *> > EntityList;
typedef std::vector<Light *, gc_allocator<Light *> > LightList;

class SceneNode : public gc_cleanup {

}; /* class SceneNode */

class Scene : public gc_cleanup {
private:
	EntityList entities;
	LightList lights;

protected:
	inline EntityList Entities () const {
		return entities;
	}

	inline LightList Lights () const {
		return lights;
	}

public:
	Scene () : entities (), lights () { }
	Scene (EntityList _entities) : entities (_entities), lights () {
		for (EntityList::const_iterator iter = entities.begin (), end = entities.end (); iter != end; ++iter) {
			Entity *entity = (*iter);
			if (Light *light = dynamic_cast<Light *> (entity)) {
				lights.insert (lights.end (), light);
			}
		}
	}
	virtual ~Scene () { }

	inline void Add (Entity *entity) {
		entities.insert (entities.end (), entity);
		if (Light *light = dynamic_cast<Light *> (entity)) {
			lights.insert (lights.end (), light);
		}
	}

	inline size_t Size () const {
		return entities.size ();
	}

	inline size_t LightCount () const {
		return lights.size ();
	}

	inline EntityList::const_iterator First () const {
		return entities.begin ();
	}

	inline EntityList::const_iterator Last () const {
		return entities.end ();
	}

	inline LightList::const_iterator FirstLight () const {
		return lights.begin ();
	}

	inline LightList::const_iterator LastLight () const {
		return lights.end ();
	}

}; /* class Scene */

/*---------------------------------------------------------------------------*/
#endif /* __NEW_SCENE_H_ */
