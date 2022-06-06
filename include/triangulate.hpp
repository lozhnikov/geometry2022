#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <point.hpp>
#include <edge.hpp>

namespace geometry {
    template<class T> bool operator == (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return false;
        if (a.Origin() > b.Origin()) return false;
        if (a.Destination() < b.Destination()) return false;
        if (a.Destination() > b.Destination()) return false;
        return true;
    }
    template<class T> bool operator != (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return true;
        if (a.Origin() > b.Origin()) return true;
        if (a.Destination() < b.Destination()) return true;
        if (a.Destination() > b.Destination()) return true;
        return false;
    }
    template<class T> bool operator < (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return false;
        if (a.Origin() > b.Origin()) return false;
        if (a.Destination() < b.Destination()) return true;
        if (a.Destination() > b.Destination()) return false;
        return false;
    }
    template<class T> bool operator > (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return true;
        if (a.Origin() > b.Origin()) return true;
        if (a.Destination() < b.Destination()) return false;
        if (a.Destination() > b.Destination()) return true;
        return false;
    }
    template<class T> bool operator <= (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return false;
        if (a.Origin() > b.Origin()) return false;
        if (a.Destination() < b.Destination()) return true;
        if (a.Destination() > b.Destination()) return false;
        return true;
    }
    template<class T> bool operator >= (const Edge<T> & a, const Edge<T> & b) {
        if (a.Origin() < b.Origin()) return true;
        if (a.Origin() > b.Origin()) return true;
        if (a.Destination() < b.Destination()) return false;
        if (a.Destination() > b.Destination()) return true;
        return true;
    }
    template<class T> bool Mate(const Edge<T> & e,
    const std::vector<Point<T>> & v,
    Point<T> * p) {
        const Point<T> * best_p = 0;
        T t = 0;
        T best_t = 0;
        Edge<T> f = e;
        f.Rotate();
        for (auto & s : v) {
            if (s.Classify(e, T(1.0E-8)) == Position::Right) {
                Edge<T> g(e.Destination(), s);
                g.Rotate();
                f.Intersect(g, &t, T(1.0E-8));
                if (t < best_t || !best_p) {
                    best_p = &s;
                    best_t = t;
                }
            }
        }
        if (best_p) {
            *p = *best_p;
            return true;
        } else {
            return false;
        }
    }
    template<class T> Edge<T> HullEdge(std::vector<Point<T>> * v) {
        size_t m = 0;
        for (size_t i = 1; i < v->size(); i++)
            if ((*v)[i] < (*v)[m])
                m = i;
        std::swap((*v)[0], (*v)[m]);
        m = 1;
        for (size_t i = 2; i < v->size(); i++) {
            // Ищем самый левый вектор среди исходящих из v[0]
            auto cls = (*v)[i].Classify(Edge<T>((*v)[0], (*v)[m]), T(1.0E-8));
            if (cls == Position::Left || cls == Position::Between) m = i;
        }
        return Edge<T>((*v)[0], (*v)[m]);
    }
    template<class T> int SetFind(const std::vector<T> * frontier,
    const T & t) {
        for (size_t i = 0; i < frontier->size(); i++)
            if ((*frontier)[i] == t)
                return static_cast<int>(i);
        return -1;
    }
    template<class T> void SetAdd(std::vector<T> * frontier, const T & t) {
        auto iter = frontier->begin();
        for (size_t i = 0; i < frontier->size(); i++)
            if ((*frontier)[i] >= t) {
                frontier->insert(iter, t);
                return;
                iter++;
        }
        frontier->push_back(t);
    }
    template<class T> void UpdateFrontier(std::vector<Edge<T>> * frontier,
    const Point<T> & a, const Point<T> & b) {
        Edge<T> e(a, b);
        auto i = SetFind(frontier, e);
        if (i >= 0) {
            frontier->erase(frontier->begin() += i);
        } else {
            e.Flip();
            SetAdd(frontier, e);
        }
    }
    template<class T> int PointIndex(const std::vector<Point<T>> & v,
    const Point<T> & p) {
        for (size_t i = 0; i < v.size(); i++)
            if (Point<T>::IsEqual(v[i], p, T(1.0E-8)))
                return static_cast<int>(i);
        throw p;
    }
    template<class T> std::vector<int>
    Triangulate(const std::vector<Point<T>> & v) {
        std::vector<Point<T>> vc = v;
        std::vector<int> indices;
        std::vector<Edge<T>> frontier;
        auto e = HullEdge(&vc);
        frontier.push_back(e);

        while (!frontier.empty()) {
            auto ei = frontier.begin();
            e = *ei;
            frontier.erase(ei);
            Point<T> p;

            if (Mate(e, vc, &p)) {
                UpdateFrontier(&frontier, p, e.Origin());
                UpdateFrontier(&frontier, e.Destination(), p);
                int i1 = PointIndex(v, e.Origin());
                int i2 = PointIndex(v, e.Destination());
                int i3 = PointIndex(v, p);
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }
        return indices;
    }
}  // namespace geometry
