/**
 * @file include/graham_scan.hpp
 * @author Eldarushkaa
 *
 * Реализация алгоритма построения выпуклой
 * оболочки методом обхода Грэхема.
 **/

#ifndef INCLUDE_GRAHAM_SCAN_HPP_
#define INCLUDE_GRAHAM_SCAN_HPP_

#include <cstddef>
#include <algorithm>
#include <common.hpp>
#include <point.hpp>
#include <polygon.hpp>
#include <iostream>

namespace geometry {

template<typename T>
class Point;
  
template<typename T, typename Container>
class Polygon;

/**
 * Функция сравнения вершин по их углу
 **/
template<typename T>
int polarCmp(Point<T> p, Point<T> q){
  T precision = T(0.001);
  T pPolar = p.PolarAngle(precision);
  T qPolar = q.PolarAngle(precision);
  long double vpl = std::sqrt(p.X() * p.X() + p.Y() * p.Y());
  long double vql = std::sqrt(q.X() * q.X() + q.Y() * q.Y());
  // std::cout<<p.X()<<" "<<p.Y()<<"    "<<q.X()<<" "<<q.Y()<<std::endl<<vpl<<" "<<vql<<" "<<pPolar<<" "<<qPolar<<std::endl;
  if (pPolar < qPolar) return 1;
  if (pPolar > qPolar) return 0;
  if (vpl < vql) return 0;
  if (vpl > vql) return 1;
  // return p.X() > q.X();
  // return pPolar > qPolar;
  return 0;
}
/*   
Polygon *GrahamScan(Point pts[], int n){
    int m = 0;
    for (int i = 1; i < n; i++)
        if ((pts[i].y < pts[m].y) ||
            (pts[i].y == pts[m].y) && (pts[i].x < pts[m].x)) 
            m = i;
    swap(pts[0], pts[m]);
    originPt = pts[0];
            
    Point **p = new (Point*)[n];
    for (i = 0; i < n; i++)
        p[i] = &pts[i];
    sort(&p[1], &p[1] + n - 1, polarCmp);
    
    for (i = 1; p[i + 1]->classify(*p[0], *p[i]) == Beyond; i++);
    stack<Point*> s;
    s.push(p[0]);
    s.push(p[i]);
    for (i = i + 1; i < n; i++){
        while (p[i]->classify(*s.nextToTop(), *s.top()) != Left){
            s.pop();
        }
        s.push(p[i]);
    }
    Polygon *q = new Polygon;
    while (!s.empty()){
        q->insert(*s.top());
        s.pop();
    }
    delete p;
    return q;
}
*/

template<typename T>
Polygon<T> *GrahamScan(std::list<Point<T>> pts, size_t n){

  size_t m = 0;
  if (m == n) m = n;
  
  /**
   * Ищем самую нижнюю левую вершину, чтобы взять её за центр координат
   **/
  auto imax = pts.begin();
  for (auto itr = pts.begin(); itr != pts.end(); itr++){
    if (itr->Y() < imax->Y() || (std::fabs(itr->Y() - imax->Y()) < 0.01 && itr->X() > imax->X())) imax = itr;
  }
  
  /**
   * Сдвигаем все вершины к новому центру
   **/
  Point<T> min = *imax;
  for (auto itr = pts.begin(); itr != pts.end(); itr++){
    *itr = *itr - min;
  }
  
  /**
   * Сортируем порядок вершин относительно их угла, начало координат добавляем и в начало и в конец
   **/
  pts.erase(imax);
  pts.sort(polarCmp<T>);
  pts.push_front(*(new Point<T>(0, 0)));
  pts.push_back(*(new Point<T>(0, 0)));
  
//   while (pts.begin() != imax){
//     pts.push_back(*(pts.begin()));
//     pts.pop_front();
//   }
  
  /**
   * Удаляем те вершины угол которых при обходе уменьшается, а не увеличивается
   **/
  auto itr = pts.begin();
  auto ipr = pts.begin();
  itr++;
  itr++;
  for (; itr != pts.end(); itr++){
    imax = itr;
    imax--;
    ipr = imax;
    ipr--;
    while(imax != pts.begin() && !polarCmp<T>(*imax - *ipr, *itr - *imax)){
      pts.erase(imax);
      imax = ipr;
      ipr--;
    }
  }

  /**
   * Возвращаем в старую систему координат
   **/
  pts.pop_back();
  for (itr = pts.begin(); itr != pts.end(); itr++){
    *itr = *itr + min;
  }
  Polygon<T> *P = new (Polygon<T>)(pts);
  return P;
}

}  // namespace geometry

#endif  // INCLUDE_GRAHAM_SCAN_HPP_