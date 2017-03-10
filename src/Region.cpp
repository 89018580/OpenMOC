#include "Region.h"



/**
 * @brief FIXME: Is this needed for an abstract class???
 */
Region::Region() { }


/**
 * @brief FIXME: Is this needed for an abstract class???
 */
Region::~Region() { }


/**
 * @brief
 * @param
 * @returns
 */
Intersection* Region::getIntersection(Region* other) {
  // FIXME: This is memory leak hell
  Intersection* new_intersection = new Intersection();

  std::vector<Region*> other_nodes = other->getNodes();
  std::vector<Region*>::iterator iter;
  for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
    new_intersection->addNode(*iter);

  return new_intersection;
}


/**
 * @brief
 * @param
 * @returns
 */
Union* Region::getUnion(Region* other) {
  // FIXME: This is memory leak hell
  Union* new_union = new Union();

  std::vector<Region*> other_nodes = other->getNodes();
  std::vector<Region*>::iterator iter;
  for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
    new_union->addNode(*iter);

  return new_union;
}


/**
 * @brief
 * @returns
 */
Complement* Region::getInversion() {
  // FIXME: This is memory leak hell
  Complement* new_complement = new Complement();
  new_complement->setNode(this);
  return new_complement;
}





/**
 * @brief FIXME: Is this necessary???
 */
Intersection::Intersection() { }


/**
 * @brief FIXME: Should this delete the nodes???
 */
Intersection::~Intersection() { }


/**
 * @brief
 * @param
 */
void Intersection::addNode(Region* node) {
  _nodes.push_back(node);
}


/**
 * @brief
 * @returns
 */
std::vector<Region*> Intersection::getNodes() {
  return _nodes;
}


/**
 * @brief
 * @param
 * @returns
 */
Intersection* Intersection::getIntersection(Region* other) {

  std::vector<Region*> other_nodes = other->getNodes();
  std::vector<Region*>::iterator iter;
  Intersection* new_intersection = new Intersection();

  for (iter = _nodes.begin(); iter != _nodes.end(); iter++)
    new_intersection->addNode(*iter);

  for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
    new_intersection->addNode(*iter);

  return new_intersection;
}


/**
 * @brief FIXME: Rename this for the ray tracing code convention
 * @param
 * @returns
 */
bool Intersection::contains(Point* point) {

  bool inside = true;
  std::vector<Region*>::iterator iter;

  for (iter = _nodes.begin(); iter != _nodes.end(); iter++) {
    if (!(*iter)->contains(point))
      inside = false;
  }

  return inside;
}




/**
 * @brief FIXME: Is this necessary???
 */
Union::Union() { }


/**
 * @brief FIXME: Should this delete the nodes???
 */
Union::~Union() { }


/**
 * @brief
 * @param
 * @returns
 */
void Union::addNode(Region* node) {
  _nodes.push_back(node);
}


/**
 * @brie
 * @returns
 */
std::vector<Region*> Union::getNodes() {
  return _nodes;
}


/**
 * @brief
 * @param
 * @returns
 */
Union* Union::getUnion(Region* other) {

  std::vector<Region*> other_nodes = other->getNodes();
  std::vector<Region*>::iterator iter;
  Union* new_union = new Union();  

  for (iter = _nodes.begin(); iter != _nodes.end(); iter++)
    new_union->addNode(*iter);

  for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
    new_union->addNode(*iter);

  return new_union;
}


/**
 * @brief FIXME: Rename this for the ray tracing code convention
 * @param
 * @returns
 */
bool Union::contains(Point* point) {

  bool inside = false;
  std::vector<Region*>::iterator iter;

  for (iter = _nodes.begin(); iter != _nodes.end(); iter++) {
    if ((*iter)->contains(point))
      inside = true;
  }

  return inside;
}





/**
 * @brief FIXME: Is this necessary???
 */
Complement::Complement() { }


/**
 * @brief FIXME: Should this delete the nodes???
 */
Complement::~Complement() { }


/**
 * @brief
 * @param
 * @returns
 */
void Complement::setNode(Region* node) {
  _node = node;
}


/**
 * @brief
 * @returns
 */
std::vector<Region*> Complement::getNodes() {
  std::vector<Region*> nodes;
  nodes.push_back(_node);
  return nodes;
}


/**
 * @brief FIXME: Rename this for the ray tracing code convention
 * @param @returns
 */
bool Complement::contains(Point* point) {
  return !_node->contains(point);
}




/**
 * @brief
 * @param
 * @param
 */
Halfspace::Halfspace(Surface* surface, int halfspace) {
  _surface = surface;
  _halfspace = halfspace;
}


/**
 * @brief FIXME: Should this delete the nodes???
 */
Halfspace::~Halfspace() { }


/**
 * @brief This may be bullshit
 * @returns
 */
std::vector<Region*> Halfspace::getNodes() {
  std::vector<Region*> nodes;
  nodes.push_back(this);
  return nodes;
}


/**
 * @brief
 * @param
 * @returns
 */
Intersection* Halfspace::getIntersection(Region* other) {
  Intersection* new_intersection = new Intersection();
  new_intersection->addNode(this);

  if (dynamic_cast<Intersection*>(other)) {
    std::vector<Region*> other_nodes = other->getNodes();
    std::vector<Region*>::iterator iter;
    for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
      new_intersection->addNode(*iter);
  }
  else
    new_intersection->addNode(other);    

  return new_intersection;
}


/**
 * @brief
 * @param
 * @returns
 */
Union* Halfspace::getUnion(Region* other) {
  Union* new_union = new Union();
  new_union->addNode(this);

  if (dynamic_cast<Union*>(other)) {
    std::vector<Region*> other_nodes = other->getNodes();
    std::vector<Region*>::iterator iter;
    for (iter = other_nodes.begin(); iter != other_nodes.end(); iter++)
      new_union->addNode(*iter);
  }
  else
    new_union->addNode(other);

  return new_union;
}


/**
 * @brief
 * @param
 * @returns
 */
Halfspace* Halfspace::getInversion() {
  // FIXME: This is memory leak hell
  Halfspace* new_halfspace = new Halfspace(_surface, -1 * _halfspace);
  return new_halfspace;
}


/**
 * @brief FIXME: Rename this for the ray tracing code convention
 * @param
 * @returns
 */
bool Halfspace::contains(Point* point) {
  // FIXME: This may be an optimization over what we have now
  if (_halfspace == 1)
    return (_surface->evaluate(point) >= 0);
  else
    return (_surface->evaluate(point) < 0);
}
