/**
 * @file include/axis_parallel_edge.h
 * @author Boris Shevchenko
 *
 * Определение и частичная реализация методов класса ребер параллельных осям.
 */

#ifndef INCLUDE_AXIS_PARALLEL_EDGE_H_
#define INCLUDE_AXIS_PARALLEL_EDGE_H_

#include <iostream>
#include <list>
#include "dictionary.h"
#include "rectangle.h"

#define T_MAX 1.e+5  //  Максимально допустимые координаты

namespace geometry {

enum Side { LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE, TOP_SIDE };

template <typename T> class AxisParallelEdge {
 public:
  Rectangle<T> *r;
  int count;
  T m;
  Side type;
  AxisParallelEdge(Rectangle<T> *_r, Side _type)
      : r(_r), count(0), m((T)-T_MAX), type(_type) {}
  T pos();
  T min();
  T max();
  void setMin(T);
  void HandleLeftEdge(Dictionary<AxisParallelEdge<T> *> *,
                      std::list<Edge<T> *> *);
  void HandleRightEdge(Dictionary<AxisParallelEdge<T> *> *,
                       std::list<Edge<T> *> *);
};

template <typename T> T AxisParallelEdge<T>::pos() {
  switch (type) {
  case Side::LEFT_SIDE:
    return r->sw.X();
    break;
  case Side::RIGHT_SIDE:
    return r->ne.X();
    break;
  case Side::TOP_SIDE:
    return r->ne.Y();
    break;
  case Side::BOTTOM_SIDE:
  default:
    return r->sw.Y();
    break;
  }
}

template <typename T> T AxisParallelEdge<T>::min() {
  if (m > -T_MAX)
    return m;
  switch (type) {
  case Side::LEFT_SIDE:
  case Side::RIGHT_SIDE:
    return r->sw.Y();
    break;
  case Side::TOP_SIDE:
  case Side::BOTTOM_SIDE:
  default:
    return r->sw.X();
    break;
  }
}

template <typename T> T AxisParallelEdge<T>::max() {
  switch (type) {
  case Side::LEFT_SIDE:
  case Side::RIGHT_SIDE:
    return r->ne.Y();
    break;
  case Side::TOP_SIDE:
  case Side::BOTTOM_SIDE:
  default:
    return r->ne.X();
    break;
  }
}

template <typename T> void AxisParallelEdge<T>::setMin(T f) { m = f; }

template <typename T>
void AxisParallelEdge<T>::HandleLeftEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  sweepline->insert(new AxisParallelEdge<T>(r, Side::TOP_SIDE));
  AxisParallelEdge<T> *u = sweepline->val();
  sweepline->insert(new AxisParallelEdge<T>(r, Side::BOTTOM_SIDE));
  AxisParallelEdge<T> *l = sweepline->val();
  AxisParallelEdge<T> *p = sweepline->prev();
  T curx = pos();
  l->count = p->count + 1;
  p = sweepline->next();
  l = sweepline->next();
  for (; l != u; p = l, l = sweepline->next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (l->count++ == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->pos()), Point<T>(curx, l->pos())));
      segs->push_back(
          new Edge<T>(Point<T>(l->min(), l->pos()), Point<T>(curx, l->pos())));
    } else {if ((l->type == Side::TOP_SIDE) && (l->count++ == 0))
    {segs->push_back(
          new Edge<T>(Point<T>(l->min(), l->pos()), Point<T>(curx, l->pos())));}
           }
  }
  if ((l->count = p->count - 1) == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->pos()), Point<T>(curx, l->pos())));
}

template <typename T>
void AxisParallelEdge<T>::HandleRightEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  AxisParallelEdge<T> uedge(r, Side::TOP_SIDE);
  AxisParallelEdge<T> ledge(r, Side::BOTTOM_SIDE);
  AxisParallelEdge<T> *u = sweepline->find(&uedge);
  AxisParallelEdge<T> *l = sweepline->find(&ledge);
  T curx = pos();
  if (l->count == 1)
    segs->push_back(
        new Edge<T>(Point<T>(l->min(), l->pos()), Point<T>(curx, l->pos())));
  if (u->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(u->min(), u->pos()), Point<T>(curx, u->pos())));
  AxisParallelEdge<T> *initl = l;
  AxisParallelEdge<T> *p = l;
  l = sweepline->next();
  for (; l != u; p = l, l = sweepline->next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (--l->count == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->pos()), Point<T>(curx, l->pos())));
      l->setMin(curx);
    } else {if ((l->type == Side::TOP_SIDE) && (--l->count == 0))
      l->setMin(curx);}
  }
  if (l->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->pos()), Point<T>(curx, l->pos())));
  sweepline->remove(u);
  sweepline->remove(initl);
}

}  // namespace geometry

#endif  // INCLUDE_AXIS_PARALLEL_EDGE_H_
