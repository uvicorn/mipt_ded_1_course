#ifndef triangle_hpp
#define triangle_hpp

#include "hittable.hpp"
#include "structs/vec3_simd.hpp"
#include "utils/types.hpp"
#include "utils/funcs.hpp"
#include <initializer_list>
#include <cassert>


class Triangle : public Hittable {
  public:
    Triangle(const Point3 points[3]){
        start_point = points[0];
        vecs[0] = points[1] - points[0];
        vecs[1] = points[2] - points[0];
    }
    Triangle(const Point3& start_point, const Vec3 vecs[2]) :
        start_point(start_point)
    {
        this->vecs[0] = vecs[0];
        this->vecs[1] = vecs[1];
    }
    Triangle(const std::initializer_list<Point3> points):
        Triangle(points.begin())
    {
        assert(points.size()==3);
    }

    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    bool hit(const Ray& ray,Interval,HitRecord& record) const override{
        Vec3 pvec = ray.direction_vec.cross(vecs[1]);
        Coord det = vecs[0].dot(pvec);

        // ray is parallel to plane
        if (IS_ZERO_ABS(det))
            return false;
        

        Coord inv_det = 1 / det;
        Vec3 tvec = ray.start_point - start_point;
        Coord u = tvec.dot(pvec) * inv_det;
        if (u < 0 || u > 1)
            return false;
        

        Vec3 qvec = tvec.cross(vecs[0]);
        Coord v = ray.direction_vec.dot(qvec) * inv_det;
        if (v < 0 || u + v > 1)
            return false;
        
        record.set_face_normal(ray, tvec);
        record.t = vecs[1].dot(qvec) * inv_det;
        // record.set_face_normal(ray, );

        return true;
    }

  protected:
    Point3 start_point;
    Vec3 vecs[2];
    // Point3 points[3];
};

#endif
