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
#include <limits>
#include "dictionary.h"
#include "rectangle.h"

namespace geometry {

enum Side { LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE, TOP_SIDE };

template <typename T> class AxisParallelEdge {
 public:
  static const T T_MAX;
  static const T T_MIN;
  Rectangle<T> *r;
  int count;
  T m;
  Side type;
  AxisParallelEdge(Rectangle<T> *_r, Side _type)
      : r(_r), count(0), m(AxisParallelEdge<T>::T_MIN), type(_type) {}
  T Pos();
  T Min();
  T Max();
  void SetMin(T);
  void HandleLeftEdge(Dictionary<AxisParallelEdge<T> *> *,
                      std::list<Edge<T> *> *);
  void HandleRightEdge(Dictionary<AxisParallelEdge<T> *> *,
                       std::list<Edge<T> *> *);
};
template <typename T>
const T AxisParallelEdge<T>::T_MAX = std::numeric_limits<T>::max();
template <typename T>
const T AxisParallelEdge<T>::T_MIN = std::numeric_limits<T>::lowest();


template <typename T> T AxisParallelEdge<T>::Pos() {
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

template <typename T> T AxisParallelEdge<T>::Min() {
  if (m > AxisParallelEdge<T>::T_MIN)
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

template <typename T> T AxisParallelEdge<T>::Max() {
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

template <typename T> void AxisParallelEdge<T>::SetMin(T f) { m = f; }

template <typename T>
void AxisParallelEdge<T>::HandleLeftEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  sweepline->Insert(new AxisParallelEdge<T>(r, Side::TOP_SIDE));
  AxisParallelEdge<T> *u = sweepline->Val();
  sweepline->Insert(new AxisParallelEdge<T>(r, Side::BOTTOM_SIDE));
  AxisParallelEdge<T> *l = sweepline->Val();
  AxisParallelEdge<T> *p = sweepline->Prev();
  T curx = Pos();
  l->count = p->count + 1;
  p = sweepline->Next();
  l = sweepline->Next();
  for (; l != u; p = l, l = sweepline->Next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (l->count++ == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
      segs->push_back(
          new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));
    } else {if ((l->type == Side::TOP_SIDE) && (l->count++ == 0))
    {segs->push_back(
          new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));}
           }
  }
  if ((l->count = p->count - 1) == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
}

template <typename T>
void AxisParallelEdge<T>::HandleRightEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  AxisParallelEdge<T> uedge(r, Side::TOP_SIDE);
  AxisParallelEdge<T> ledge(r, Side::BOTTOM_SIDE);
  AxisParallelEdge<T> *u = sweepline->Find(&uedge);
  AxisParallelEdge<T> *l = sweepline->Find(&ledge);
  T curx = Pos();
  if (l->count == 1)
    segs->push_back(
        new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));
  if (u->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(u->Min(), u->Pos()), Point<T>(curx, u->Pos())));
  AxisParallelEdge<T> *initl = l;
  AxisParallelEdge<T> *p = l;
  l = sweepline->Next();
  for (; l != u; p = l, l = sweepline->Next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (--l->count == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
      l->SetMin(curx);
    } else {if ((l->type == Side::TOP_SIDE) && (--l->count == 0))
      l->SetMin(curx);}
  }
  if (l->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
  sweepline->Remove(u);
  sweepline->Remove(initl);
}

}  // namespace geometry

#endif  // INCLUDE_AXIS_PARALLEL_EDGE_H_

