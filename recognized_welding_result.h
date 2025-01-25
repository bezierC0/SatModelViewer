/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef _WORK_FEATURERECOGNITION_AVRO_FR_RECOGNIZED_WELDING_RESULT_H_3218641067__H_
#define _WORK_FEATURERECOGNITION_AVRO_FR_RECOGNIZED_WELDING_RESULT_H_3218641067__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace fr {
namespace recognize_welding {
struct Point3D {
    double x;
    double y;
    double z;
    Point3D() :
        x(),
        y(),
        z()
    { }
    Point3D(double v_x, double v_y, double v_z) :
        x(v_x),
        y(v_y),
        z(v_z)
    { }
};

struct Box {
    Point3D low;
    Point3D high;
    Box() :
        low(),
        high()
    { }
    Box(const Point3D& v_low, const Point3D& v_high) :
        low(v_low),
        high(v_high)
    { }
};

struct UnitVector3D {
    double x;
    double y;
    double z;
    UnitVector3D() :
        x(),
        y(),
        z()
    { }
    UnitVector3D(double v_x, double v_y, double v_z) :
        x(v_x),
        y(v_y),
        z(v_z)
    { }
};

struct Axis {
    Point3D pos;
    UnitVector3D dir;
    UnitVector3D orthogonalDir;
    Axis() :
        pos(),
        dir(),
        orthogonalDir()
    { }
    Axis(const Point3D& v_pos, const UnitVector3D& v_dir, const UnitVector3D& v_orthogonalDir) :
        pos(v_pos),
        dir(v_dir),
        orthogonalDir(v_orthogonalDir)
    { }
};

struct SheetAssembly {
    std::string id;
    Box bound;
    std::vector<std::string > products;
    std::vector<std::string > weldings;
    Axis layoutAxis;
    int32_t likelihood;
    std::vector<int32_t > errors;
    SheetAssembly() :
        id(),
        bound(),
        products(),
        weldings(),
        layoutAxis(),
        likelihood(),
        errors()
    { }
    SheetAssembly(const std::string& v_id, const Box& v_bound, const std::vector<std::string >& v_products, const std::vector<std::string >& v_weldings, const Axis& v_layoutAxis, int32_t v_likelihood, const std::vector<int32_t >& v_errors) :
        id(v_id),
        bound(v_bound),
        products(v_products),
        weldings(v_weldings),
        layoutAxis(v_layoutAxis),
        likelihood(v_likelihood),
        errors(v_errors)
    { }
};

struct SideWelding {
    std::string id;
    bool isBothSideOnly;
    std::string sideWeldingType;
    std::string primaryFeature;
    std::string secondaryFeature;
    std::string topPart;
    std::string bottomPart;
    SideWelding() :
        id(),
        isBothSideOnly(),
        sideWeldingType(),
        primaryFeature(),
        secondaryFeature(),
        topPart(),
        bottomPart()
    { }
    SideWelding(const std::string& v_id, bool v_isBothSideOnly, const std::string& v_sideWeldingType, const std::string& v_primaryFeature, const std::string& v_secondaryFeature, const std::string& v_topPart, const std::string& v_bottomPart) :
        id(v_id),
        isBothSideOnly(v_isBothSideOnly),
        sideWeldingType(v_sideWeldingType),
        primaryFeature(v_primaryFeature),
        secondaryFeature(v_secondaryFeature),
        topPart(v_topPart),
        bottomPart(v_bottomPart)
    { }
};

struct SurfaceWelding {
    std::string id;
    std::string primaryFeature;
    std::string secondaryFeature;
    std::vector<std::string > parts;
    SurfaceWelding() :
        id(),
        primaryFeature(),
        secondaryFeature(),
        parts()
    { }
    SurfaceWelding(const std::string& v_id, const std::string& v_primaryFeature, const std::string& v_secondaryFeature, const std::vector<std::string >& v_parts) :
        id(v_id),
        primaryFeature(v_primaryFeature),
        secondaryFeature(v_secondaryFeature),
        parts(v_parts)
    { }
};

struct LineWeldingPart {
    std::string id;
    std::vector<std::string > primaryFaces;
    std::vector<std::string > secondaryFaces;
    std::string primaryEdge;
    std::string secondaryEdge;
    std::vector<std::string > facesRelatedToEdge;
    std::vector<std::string > facesProximityToPrimaryEdge;
    std::vector<std::string > facesProximityToSecondaryEdge;
    std::string holeFeatureId;
    double length;
    bool isEnabledGrooveWelding;
    std::vector<std::string > ignorableReasons;
    LineWeldingPart() :
        id(),
        primaryFaces(),
        secondaryFaces(),
        primaryEdge(),
        secondaryEdge(),
        facesRelatedToEdge(),
        facesProximityToPrimaryEdge(),
        facesProximityToSecondaryEdge(),
        holeFeatureId(),
        length(),
        isEnabledGrooveWelding(),
        ignorableReasons()
    { }
    LineWeldingPart(const std::string& v_id, const std::vector<std::string >& v_primaryFaces, const std::vector<std::string >& v_secondaryFaces, const std::string& v_primaryEdge, const std::string& v_secondaryEdge, const std::vector<std::string >& v_facesRelatedToEdge, const std::vector<std::string >& v_facesProximityToPrimaryEdge, const std::vector<std::string >& v_facesProximityToSecondaryEdge, const std::string& v_holeFeatureId, double v_length, bool v_isEnabledGrooveWelding, const std::vector<std::string >& v_ignorableReasons) :
        id(v_id),
        primaryFaces(v_primaryFaces),
        secondaryFaces(v_secondaryFaces),
        primaryEdge(v_primaryEdge),
        secondaryEdge(v_secondaryEdge),
        facesRelatedToEdge(v_facesRelatedToEdge),
        facesProximityToPrimaryEdge(v_facesProximityToPrimaryEdge),
        facesProximityToSecondaryEdge(v_facesProximityToSecondaryEdge),
        holeFeatureId(v_holeFeatureId),
        length(v_length),
        isEnabledGrooveWelding(v_isEnabledGrooveWelding),
        ignorableReasons(v_ignorableReasons)
    { }
};

struct NonWeldingPart {
    std::string id;
    NonWeldingPart() :
        id()
    { }
    NonWeldingPart(const std::string& v_id) :
        id(v_id)
    { }
};

struct SurfaceWeldingPart {
    std::string id;
    std::string primaryFace;
    std::string secondaryFace;
    SurfaceWeldingPart() :
        id(),
        primaryFace(),
        secondaryFace()
    { }
    SurfaceWeldingPart(const std::string& v_id, const std::string& v_primaryFace, const std::string& v_secondaryFace) :
        id(v_id),
        primaryFace(v_primaryFace),
        secondaryFace(v_secondaryFace)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    SheetAssembly get_SheetAssembly() const;
    void get(SheetAssembly& v) const;
    void set_SheetAssembly(const SheetAssembly& v);
    void set(const SheetAssembly& v);
    SideWelding get_SideWelding() const;
    void get(SideWelding& v) const;
    void set_SideWelding(const SideWelding& v);
    void set(const SideWelding& v);
    SurfaceWelding get_SurfaceWelding() const;
    void get(SurfaceWelding& v) const;
    void set_SurfaceWelding(const SurfaceWelding& v);
    void set(const SurfaceWelding& v);
    LineWeldingPart get_LineWeldingPart() const;
    void get(LineWeldingPart& v) const;
    void set_LineWeldingPart(const LineWeldingPart& v);
    void set(const LineWeldingPart& v);
    NonWeldingPart get_NonWeldingPart() const;
    void get(NonWeldingPart& v) const;
    void set_NonWeldingPart(const NonWeldingPart& v);
    void set(const NonWeldingPart& v);
    SurfaceWeldingPart get_SurfaceWeldingPart() const;
    void get(SurfaceWeldingPart& v) const;
    void set_SurfaceWeldingPart(const SurfaceWeldingPart& v);
    void set(const SurfaceWeldingPart& v);
    _RecognizedWeldingResultRaw_json_Union__0__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = false;
    using unique_valuetype = void;
};

struct Vector3D {
    double x;
    double y;
    double z;
    Vector3D() :
        x(),
        y(),
        z()
    { }
    Vector3D(double v_x, double v_y, double v_z) :
        x(v_x),
        y(v_y),
        z(v_z)
    { }
};

struct Parameter {
    double value;
    Parameter() :
        value()
    { }
    Parameter(double v_value) :
        value(v_value)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__1__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    double get_double() const;
    void get(double& v) const;
    void set_double(const double& v);
    void set(double v);
    _RecognizedWeldingResultRaw_json_Union__1__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = true;
    using unique_valuetype = double;
};

struct _RecognizedWeldingResultRaw_json_Union__2__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    double get_double() const;
    void get(double& v) const;
    void set_double(const double& v);
    void set(double v);
    _RecognizedWeldingResultRaw_json_Union__2__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = true;
    using unique_valuetype = double;
};

struct Interval {
    using start_t = _RecognizedWeldingResultRaw_json_Union__1__;
    using end_t = _RecognizedWeldingResultRaw_json_Union__2__;
    start_t start;
    end_t end;
    Interval() :
        start(),
        end()
    { }
    Interval(const start_t& v_start, const end_t& v_end) :
        start(v_start),
        end(v_end)
    { }
};

struct Circle {
    Point3D center;
    UnitVector3D normal;
    Vector3D majorAxis;
    double radiusRatio;
    Parameter paramOff;
    bool periodic;
    Interval paramRange;
    Circle() :
        center(),
        normal(),
        majorAxis(),
        radiusRatio(),
        paramOff(),
        periodic(),
        paramRange()
    { }
    Circle(const Point3D& v_center, const UnitVector3D& v_normal, const Vector3D& v_majorAxis, double v_radiusRatio, const Parameter& v_paramOff, bool v_periodic, const Interval& v_paramRange) :
        center(v_center),
        normal(v_normal),
        majorAxis(v_majorAxis),
        radiusRatio(v_radiusRatio),
        paramOff(v_paramOff),
        periodic(v_periodic),
        paramRange(v_paramRange)
    { }
};

struct Ellipse {
    Point3D center;
    UnitVector3D normal;
    Vector3D majorAxis;
    double radiusRatio;
    Parameter paramOff;
    bool periodic;
    Interval paramRange;
    Ellipse() :
        center(),
        normal(),
        majorAxis(),
        radiusRatio(),
        paramOff(),
        periodic(),
        paramRange()
    { }
    Ellipse(const Point3D& v_center, const UnitVector3D& v_normal, const Vector3D& v_majorAxis, double v_radiusRatio, const Parameter& v_paramOff, bool v_periodic, const Interval& v_paramRange) :
        center(v_center),
        normal(v_normal),
        majorAxis(v_majorAxis),
        radiusRatio(v_radiusRatio),
        paramOff(v_paramOff),
        periodic(v_periodic),
        paramRange(v_paramRange)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__3__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    Circle get_Circle() const;
    void get(Circle& v) const;
    void set_Circle(const Circle& v);
    void set(const Circle& v);
    Ellipse get_Ellipse() const;
    void get(Ellipse& v) const;
    void set_Ellipse(const Ellipse& v);
    void set(const Ellipse& v);
    _RecognizedWeldingResultRaw_json_Union__3__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = false;
    using unique_valuetype = void;
};

struct Cone {
    using base_t = _RecognizedWeldingResultRaw_json_Union__3__;
    base_t base;
    double sineAngle;
    double cosineAngle;
    bool reverseU;
    double uParamScale;
    bool isCylinder;
    Cone() :
        base(),
        sineAngle(),
        cosineAngle(),
        reverseU(),
        uParamScale(),
        isCylinder()
    { }
    Cone(const base_t& v_base, double v_sineAngle, double v_cosineAngle, bool v_reverseU, double v_uParamScale, bool v_isCylinder) :
        base(v_base),
        sineAngle(v_sineAngle),
        cosineAngle(v_cosineAngle),
        reverseU(v_reverseU),
        uParamScale(v_uParamScale),
        isCylinder(v_isCylinder)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__4__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    Circle get_Circle() const;
    void get(Circle& v) const;
    void set_Circle(const Circle& v);
    void set(const Circle& v);
    Ellipse get_Ellipse() const;
    void get(Ellipse& v) const;
    void set_Ellipse(const Ellipse& v);
    void set(const Ellipse& v);
    _RecognizedWeldingResultRaw_json_Union__4__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = false;
    using unique_valuetype = void;
};

struct Cylinder {
    using base_t = _RecognizedWeldingResultRaw_json_Union__4__;
    base_t base;
    double sineAngle;
    double cosineAngle;
    bool reverseU;
    double uParamScale;
    bool isCylinder;
    double radius;
    Cylinder() :
        base(),
        sineAngle(),
        cosineAngle(),
        reverseU(),
        uParamScale(),
        isCylinder(),
        radius()
    { }
    Cylinder(const base_t& v_base, double v_sineAngle, double v_cosineAngle, bool v_reverseU, double v_uParamScale, bool v_isCylinder, double v_radius) :
        base(v_base),
        sineAngle(v_sineAngle),
        cosineAngle(v_cosineAngle),
        reverseU(v_reverseU),
        uParamScale(v_uParamScale),
        isCylinder(v_isCylinder),
        radius(v_radius)
    { }
};

struct Facet {
    std::vector<Point3D > points;
    std::vector<UnitVector3D > normals;
    Facet() :
        points(),
        normals()
    { }
    Facet(const std::vector<Point3D >& v_points, const std::vector<UnitVector3D >& v_normals) :
        points(v_points),
        normals(v_normals)
    { }
};

struct Sphere {
    Point3D center;
    UnitVector3D poleDir;
    double radius;
    bool reverseV;
    UnitVector3D uvOridir;
    Sphere() :
        center(),
        poleDir(),
        radius(),
        reverseV(),
        uvOridir()
    { }
    Sphere(const Point3D& v_center, const UnitVector3D& v_poleDir, double v_radius, bool v_reverseV, const UnitVector3D& v_uvOridir) :
        center(v_center),
        poleDir(v_poleDir),
        radius(v_radius),
        reverseV(v_reverseV),
        uvOridir(v_uvOridir)
    { }
};

struct SurfacePlane {
    UnitVector3D normal;
    bool reverseV;
    Point3D rootPoint;
    Vector3D uDeriv;
    SurfacePlane() :
        normal(),
        reverseV(),
        rootPoint(),
        uDeriv()
    { }
    SurfacePlane(const UnitVector3D& v_normal, bool v_reverseV, const Point3D& v_rootPoint, const Vector3D& v_uDeriv) :
        normal(v_normal),
        reverseV(v_reverseV),
        rootPoint(v_rootPoint),
        uDeriv(v_uDeriv)
    { }
};

struct Torus {
    Point3D center;
    double majorRadius;
    double minorRadius;
    UnitVector3D normal;
    bool reverseV;
    UnitVector3D uvOridir;
    Torus() :
        center(),
        majorRadius(),
        minorRadius(),
        normal(),
        reverseV(),
        uvOridir()
    { }
    Torus(const Point3D& v_center, double v_majorRadius, double v_minorRadius, const UnitVector3D& v_normal, bool v_reverseV, const UnitVector3D& v_uvOridir) :
        center(v_center),
        majorRadius(v_majorRadius),
        minorRadius(v_minorRadius),
        normal(v_normal),
        reverseV(v_reverseV),
        uvOridir(v_uvOridir)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__5__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    Cone get_Cone() const;
    void get(Cone& v) const;
    void set_Cone(const Cone& v);
    void set(const Cone& v);
    Cylinder get_Cylinder() const;
    void get(Cylinder& v) const;
    void set_Cylinder(const Cylinder& v);
    void set(const Cylinder& v);
    Facet get_Facet() const;
    void get(Facet& v) const;
    void set_Facet(const Facet& v);
    void set(const Facet& v);
    Sphere get_Sphere() const;
    void get(Sphere& v) const;
    void set_Sphere(const Sphere& v);
    void set(const Sphere& v);
    SurfacePlane get_SurfacePlane() const;
    void get(SurfacePlane& v) const;
    void set_SurfacePlane(const SurfacePlane& v);
    void set(const SurfacePlane& v);
    Torus get_Torus() const;
    void get(Torus& v) const;
    void set_Torus(const Torus& v);
    void set(const Torus& v);
    _RecognizedWeldingResultRaw_json_Union__5__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = false;
    using unique_valuetype = void;
};

struct Face {
    using surface_t = _RecognizedWeldingResultRaw_json_Union__5__;
    std::string id;
    std::vector<std::string > loops;
    surface_t surface;
    bool sense;
    double area;
    Face() :
        id(),
        loops(),
        surface(),
        sense(),
        area()
    { }
    Face(const std::string& v_id, const std::vector<std::string >& v_loops, const surface_t& v_surface, bool v_sense, double v_area) :
        id(v_id),
        loops(v_loops),
        surface(v_surface),
        sense(v_sense),
        area(v_area)
    { }
};

struct Polyline {
    std::vector<Point3D > points;
    Polyline() :
        points()
    { }
    Polyline(const std::vector<Point3D >& v_points) :
        points(v_points)
    { }
};

struct Straight {
    Point3D rootPoint;
    UnitVector3D direction;
    double paramScale;
    Straight() :
        rootPoint(),
        direction(),
        paramScale()
    { }
    Straight(const Point3D& v_rootPoint, const UnitVector3D& v_direction, double v_paramScale) :
        rootPoint(v_rootPoint),
        direction(v_direction),
        paramScale(v_paramScale)
    { }
};

struct _RecognizedWeldingResultRaw_json_Union__6__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    Circle get_Circle() const;
    void get(Circle& v) const;
    void set_Circle(const Circle& v);
    void set(const Circle& v);
    Ellipse get_Ellipse() const;
    void get(Ellipse& v) const;
    void set_Ellipse(const Ellipse& v);
    void set(const Ellipse& v);
    Polyline get_Polyline() const;
    void get(Polyline& v) const;
    void set_Polyline(const Polyline& v);
    void set(const Polyline& v);
    Straight get_Straight() const;
    void get(Straight& v) const;
    void set_Straight(const Straight& v);
    void set(const Straight& v);
    _RecognizedWeldingResultRaw_json_Union__6__();
    const boost::any& cref() const {
        return value_;
    }
    template<class DecodeType>
    DecodeType get() const {
        DecodeType v;
        get(v);
        return v;
    }
    static const bool valuetype_is_unique = false;
    using unique_valuetype = void;
};

struct Edge {
    using curve_t = _RecognizedWeldingResultRaw_json_Union__6__;
    std::string id;
    Interval paramRange;
    std::string start;
    std::string end;
    curve_t curve;
    bool sense;
    Edge() :
        id(),
        paramRange(),
        start(),
        end(),
        curve(),
        sense()
    { }
    Edge(const std::string& v_id, const Interval& v_paramRange, const std::string& v_start, const std::string& v_end, const curve_t& v_curve, bool v_sense) :
        id(v_id),
        paramRange(v_paramRange),
        start(v_start),
        end(v_end),
        curve(v_curve),
        sense(v_sense)
    { }
};

struct Vertex {
    std::string id;
    Point3D position;
    Vertex() :
        id(),
        position()
    { }
    Vertex(const std::string& v_id, const Point3D& v_position) :
        id(v_id),
        position(v_position)
    { }
};

struct Loop {
    std::string id;
    std::vector<std::string > coedges;
    std::string face;
    Loop() :
        id(),
        coedges(),
        face()
    { }
    Loop(const std::string& v_id, const std::vector<std::string >& v_coedges, const std::string& v_face) :
        id(v_id),
        coedges(v_coedges),
        face(v_face)
    { }
};

struct Coedge {
    std::string id;
    std::string edge;
    std::string loop;
    bool sense;
    Coedge() :
        id(),
        edge(),
        loop(),
        sense()
    { }
    Coedge(const std::string& v_id, const std::string& v_edge, const std::string& v_loop, bool v_sense) :
        id(v_id),
        edge(v_edge),
        loop(v_loop),
        sense(v_sense)
    { }
};

struct Topologies {
    std::vector<Face > faces;
    std::vector<Edge > edges;
    std::vector<Vertex > vertices;
    std::vector<Loop > loops;
    std::vector<Coedge > coedges;
    Topologies() :
        faces(),
        edges(),
        vertices(),
        loops(),
        coedges()
    { }
    Topologies(const std::vector<Face >& v_faces, const std::vector<Edge >& v_edges, const std::vector<Vertex >& v_vertices, const std::vector<Loop >& v_loops, const std::vector<Coedge >& v_coedges) :
        faces(v_faces),
        edges(v_edges),
        vertices(v_vertices),
        loops(v_loops),
        coedges(v_coedges)
    { }
};

struct RecognizedWeldingResultRaw {
    int32_t version;
    std::vector<_RecognizedWeldingResultRaw_json_Union__0__ > features;
    std::vector<std::string > solidIdList;
    std::vector<std::string > products;
    Topologies virtualTopologies;
    RecognizedWeldingResultRaw() :
        version(),
        features(),
        solidIdList(),
        products(),
        virtualTopologies()
    { }
    RecognizedWeldingResultRaw(int32_t v_version, const std::vector<_RecognizedWeldingResultRaw_json_Union__0__ >& v_features, const std::vector<std::string >& v_solidIdList, const std::vector<std::string >& v_products, const Topologies& v_virtualTopologies) :
        version(v_version),
        features(v_features),
        solidIdList(v_solidIdList),
        products(v_products),
        virtualTopologies(v_virtualTopologies)
    { }
};

inline
SheetAssembly _RecognizedWeldingResultRaw_json_Union__0__::get_SheetAssembly() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<SheetAssembly >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(SheetAssembly& v) const {
    v = get_SheetAssembly();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const SheetAssembly& v) {
    idx_ = 0;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_SheetAssembly(const SheetAssembly& v) {
    set(v);
}

inline
SideWelding _RecognizedWeldingResultRaw_json_Union__0__::get_SideWelding() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<SideWelding >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(SideWelding& v) const {
    v = get_SideWelding();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const SideWelding& v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_SideWelding(const SideWelding& v) {
    set(v);
}

inline
SurfaceWelding _RecognizedWeldingResultRaw_json_Union__0__::get_SurfaceWelding() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<SurfaceWelding >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(SurfaceWelding& v) const {
    v = get_SurfaceWelding();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const SurfaceWelding& v) {
    idx_ = 2;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_SurfaceWelding(const SurfaceWelding& v) {
    set(v);
}

inline
LineWeldingPart _RecognizedWeldingResultRaw_json_Union__0__::get_LineWeldingPart() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<LineWeldingPart >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(LineWeldingPart& v) const {
    v = get_LineWeldingPart();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const LineWeldingPart& v) {
    idx_ = 3;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_LineWeldingPart(const LineWeldingPart& v) {
    set(v);
}

inline
NonWeldingPart _RecognizedWeldingResultRaw_json_Union__0__::get_NonWeldingPart() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<NonWeldingPart >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(NonWeldingPart& v) const {
    v = get_NonWeldingPart();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const NonWeldingPart& v) {
    idx_ = 4;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_NonWeldingPart(const NonWeldingPart& v) {
    set(v);
}

inline
SurfaceWeldingPart _RecognizedWeldingResultRaw_json_Union__0__::get_SurfaceWeldingPart() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<SurfaceWeldingPart >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__0__::get(SurfaceWeldingPart& v) const {
    v = get_SurfaceWeldingPart();
}

inline
void _RecognizedWeldingResultRaw_json_Union__0__::set(const SurfaceWeldingPart& v) {
    idx_ = 5;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__0__::set_SurfaceWeldingPart(const SurfaceWeldingPart& v) {
    set(v);
}

inline
double _RecognizedWeldingResultRaw_json_Union__1__::get_double() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<double >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__1__::get(double& v) const {
    v = get_double();
}

inline
void _RecognizedWeldingResultRaw_json_Union__1__::set(double v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__1__::set_double(const double& v) {
    set(v);
}

inline
double _RecognizedWeldingResultRaw_json_Union__2__::get_double() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<double >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__2__::get(double& v) const {
    v = get_double();
}

inline
void _RecognizedWeldingResultRaw_json_Union__2__::set(double v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__2__::set_double(const double& v) {
    set(v);
}

inline
Circle _RecognizedWeldingResultRaw_json_Union__3__::get_Circle() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Circle >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__3__::get(Circle& v) const {
    v = get_Circle();
}

inline
void _RecognizedWeldingResultRaw_json_Union__3__::set(const Circle& v) {
    idx_ = 0;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__3__::set_Circle(const Circle& v) {
    set(v);
}

inline
Ellipse _RecognizedWeldingResultRaw_json_Union__3__::get_Ellipse() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Ellipse >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__3__::get(Ellipse& v) const {
    v = get_Ellipse();
}

inline
void _RecognizedWeldingResultRaw_json_Union__3__::set(const Ellipse& v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__3__::set_Ellipse(const Ellipse& v) {
    set(v);
}

inline
Circle _RecognizedWeldingResultRaw_json_Union__4__::get_Circle() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Circle >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__4__::get(Circle& v) const {
    v = get_Circle();
}

inline
void _RecognizedWeldingResultRaw_json_Union__4__::set(const Circle& v) {
    idx_ = 0;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__4__::set_Circle(const Circle& v) {
    set(v);
}

inline
Ellipse _RecognizedWeldingResultRaw_json_Union__4__::get_Ellipse() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Ellipse >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__4__::get(Ellipse& v) const {
    v = get_Ellipse();
}

inline
void _RecognizedWeldingResultRaw_json_Union__4__::set(const Ellipse& v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__4__::set_Ellipse(const Ellipse& v) {
    set(v);
}

inline
Cone _RecognizedWeldingResultRaw_json_Union__5__::get_Cone() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Cone >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(Cone& v) const {
    v = get_Cone();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const Cone& v) {
    idx_ = 0;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_Cone(const Cone& v) {
    set(v);
}

inline
Cylinder _RecognizedWeldingResultRaw_json_Union__5__::get_Cylinder() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Cylinder >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(Cylinder& v) const {
    v = get_Cylinder();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const Cylinder& v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_Cylinder(const Cylinder& v) {
    set(v);
}

inline
Facet _RecognizedWeldingResultRaw_json_Union__5__::get_Facet() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Facet >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(Facet& v) const {
    v = get_Facet();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const Facet& v) {
    idx_ = 2;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_Facet(const Facet& v) {
    set(v);
}

inline
Sphere _RecognizedWeldingResultRaw_json_Union__5__::get_Sphere() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Sphere >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(Sphere& v) const {
    v = get_Sphere();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const Sphere& v) {
    idx_ = 3;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_Sphere(const Sphere& v) {
    set(v);
}

inline
SurfacePlane _RecognizedWeldingResultRaw_json_Union__5__::get_SurfacePlane() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<SurfacePlane >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(SurfacePlane& v) const {
    v = get_SurfacePlane();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const SurfacePlane& v) {
    idx_ = 4;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_SurfacePlane(const SurfacePlane& v) {
    set(v);
}

inline
Torus _RecognizedWeldingResultRaw_json_Union__5__::get_Torus() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Torus >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__5__::get(Torus& v) const {
    v = get_Torus();
}

inline
void _RecognizedWeldingResultRaw_json_Union__5__::set(const Torus& v) {
    idx_ = 5;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__5__::set_Torus(const Torus& v) {
    set(v);
}

inline
Circle _RecognizedWeldingResultRaw_json_Union__6__::get_Circle() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Circle >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__6__::get(Circle& v) const {
    v = get_Circle();
}

inline
void _RecognizedWeldingResultRaw_json_Union__6__::set(const Circle& v) {
    idx_ = 0;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__6__::set_Circle(const Circle& v) {
    set(v);
}

inline
Ellipse _RecognizedWeldingResultRaw_json_Union__6__::get_Ellipse() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Ellipse >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__6__::get(Ellipse& v) const {
    v = get_Ellipse();
}

inline
void _RecognizedWeldingResultRaw_json_Union__6__::set(const Ellipse& v) {
    idx_ = 1;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__6__::set_Ellipse(const Ellipse& v) {
    set(v);
}

inline
Polyline _RecognizedWeldingResultRaw_json_Union__6__::get_Polyline() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Polyline >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__6__::get(Polyline& v) const {
    v = get_Polyline();
}

inline
void _RecognizedWeldingResultRaw_json_Union__6__::set(const Polyline& v) {
    idx_ = 2;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__6__::set_Polyline(const Polyline& v) {
    set(v);
}

inline
Straight _RecognizedWeldingResultRaw_json_Union__6__::get_Straight() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<Straight >(value_);
}
inline
void  _RecognizedWeldingResultRaw_json_Union__6__::get(Straight& v) const {
    v = get_Straight();
}

inline
void _RecognizedWeldingResultRaw_json_Union__6__::set(const Straight& v) {
    idx_ = 3;
    value_ = v;
}
inline
void _RecognizedWeldingResultRaw_json_Union__6__::set_Straight(const Straight& v) {
    set(v);
}

inline _RecognizedWeldingResultRaw_json_Union__0__::_RecognizedWeldingResultRaw_json_Union__0__() : idx_(0), value_(SheetAssembly()) { }
inline _RecognizedWeldingResultRaw_json_Union__1__::_RecognizedWeldingResultRaw_json_Union__1__() : idx_(0) { }
inline _RecognizedWeldingResultRaw_json_Union__2__::_RecognizedWeldingResultRaw_json_Union__2__() : idx_(0) { }
inline _RecognizedWeldingResultRaw_json_Union__3__::_RecognizedWeldingResultRaw_json_Union__3__() : idx_(0), value_(Circle()) { }
inline _RecognizedWeldingResultRaw_json_Union__4__::_RecognizedWeldingResultRaw_json_Union__4__() : idx_(0), value_(Circle()) { }
inline _RecognizedWeldingResultRaw_json_Union__5__::_RecognizedWeldingResultRaw_json_Union__5__() : idx_(0), value_(Cone()) { }
inline _RecognizedWeldingResultRaw_json_Union__6__::_RecognizedWeldingResultRaw_json_Union__6__() : idx_(0), value_(Circle()) { }
}
}
namespace avro {
template<> struct codec_traits<fr::recognize_welding::Point3D> {
    static void encode(Encoder& e, const fr::recognize_welding::Point3D& v) {
        avro::encode(e, v.x);
        avro::encode(e, v.y);
        avro::encode(e, v.z);
    }
    static void decode(Decoder& d, fr::recognize_welding::Point3D& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.x);
                    break;
                case 1:
                    avro::decode(d, v.y);
                    break;
                case 2:
                    avro::decode(d, v.z);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.x);
            avro::decode(d, v.y);
            avro::decode(d, v.z);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Box> {
    static void encode(Encoder& e, const fr::recognize_welding::Box& v) {
        avro::encode(e, v.low);
        avro::encode(e, v.high);
    }
    static void decode(Decoder& d, fr::recognize_welding::Box& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.low);
                    break;
                case 1:
                    avro::decode(d, v.high);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.low);
            avro::decode(d, v.high);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::UnitVector3D> {
    static void encode(Encoder& e, const fr::recognize_welding::UnitVector3D& v) {
        avro::encode(e, v.x);
        avro::encode(e, v.y);
        avro::encode(e, v.z);
    }
    static void decode(Decoder& d, fr::recognize_welding::UnitVector3D& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.x);
                    break;
                case 1:
                    avro::decode(d, v.y);
                    break;
                case 2:
                    avro::decode(d, v.z);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.x);
            avro::decode(d, v.y);
            avro::decode(d, v.z);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Axis> {
    static void encode(Encoder& e, const fr::recognize_welding::Axis& v) {
        avro::encode(e, v.pos);
        avro::encode(e, v.dir);
        avro::encode(e, v.orthogonalDir);
    }
    static void decode(Decoder& d, fr::recognize_welding::Axis& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.pos);
                    break;
                case 1:
                    avro::decode(d, v.dir);
                    break;
                case 2:
                    avro::decode(d, v.orthogonalDir);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.pos);
            avro::decode(d, v.dir);
            avro::decode(d, v.orthogonalDir);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::SheetAssembly> {
    static void encode(Encoder& e, const fr::recognize_welding::SheetAssembly& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.bound);
        avro::encode(e, v.products);
        avro::encode(e, v.weldings);
        avro::encode(e, v.layoutAxis);
        avro::encode(e, v.likelihood);
        avro::encode(e, v.errors);
    }
    static void decode(Decoder& d, fr::recognize_welding::SheetAssembly& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.bound);
                    break;
                case 2:
                    avro::decode(d, v.products);
                    break;
                case 3:
                    avro::decode(d, v.weldings);
                    break;
                case 4:
                    avro::decode(d, v.layoutAxis);
                    break;
                case 5:
                    avro::decode(d, v.likelihood);
                    break;
                case 6:
                    avro::decode(d, v.errors);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.bound);
            avro::decode(d, v.products);
            avro::decode(d, v.weldings);
            avro::decode(d, v.layoutAxis);
            avro::decode(d, v.likelihood);
            avro::decode(d, v.errors);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::SideWelding> {
    static void encode(Encoder& e, const fr::recognize_welding::SideWelding& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.isBothSideOnly);
        avro::encode(e, v.sideWeldingType);
        avro::encode(e, v.primaryFeature);
        avro::encode(e, v.secondaryFeature);
        avro::encode(e, v.topPart);
        avro::encode(e, v.bottomPart);
    }
    static void decode(Decoder& d, fr::recognize_welding::SideWelding& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.isBothSideOnly);
                    break;
                case 2:
                    avro::decode(d, v.sideWeldingType);
                    break;
                case 3:
                    avro::decode(d, v.primaryFeature);
                    break;
                case 4:
                    avro::decode(d, v.secondaryFeature);
                    break;
                case 5:
                    avro::decode(d, v.topPart);
                    break;
                case 6:
                    avro::decode(d, v.bottomPart);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.isBothSideOnly);
            avro::decode(d, v.sideWeldingType);
            avro::decode(d, v.primaryFeature);
            avro::decode(d, v.secondaryFeature);
            avro::decode(d, v.topPart);
            avro::decode(d, v.bottomPart);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::SurfaceWelding> {
    static void encode(Encoder& e, const fr::recognize_welding::SurfaceWelding& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.primaryFeature);
        avro::encode(e, v.secondaryFeature);
        avro::encode(e, v.parts);
    }
    static void decode(Decoder& d, fr::recognize_welding::SurfaceWelding& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.primaryFeature);
                    break;
                case 2:
                    avro::decode(d, v.secondaryFeature);
                    break;
                case 3:
                    avro::decode(d, v.parts);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.primaryFeature);
            avro::decode(d, v.secondaryFeature);
            avro::decode(d, v.parts);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::LineWeldingPart> {
    static void encode(Encoder& e, const fr::recognize_welding::LineWeldingPart& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.primaryFaces);
        avro::encode(e, v.secondaryFaces);
        avro::encode(e, v.primaryEdge);
        avro::encode(e, v.secondaryEdge);
        avro::encode(e, v.facesRelatedToEdge);
        avro::encode(e, v.facesProximityToPrimaryEdge);
        avro::encode(e, v.facesProximityToSecondaryEdge);
        avro::encode(e, v.holeFeatureId);
        avro::encode(e, v.length);
        avro::encode(e, v.isEnabledGrooveWelding);
        avro::encode(e, v.ignorableReasons);
    }
    static void decode(Decoder& d, fr::recognize_welding::LineWeldingPart& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.primaryFaces);
                    break;
                case 2:
                    avro::decode(d, v.secondaryFaces);
                    break;
                case 3:
                    avro::decode(d, v.primaryEdge);
                    break;
                case 4:
                    avro::decode(d, v.secondaryEdge);
                    break;
                case 5:
                    avro::decode(d, v.facesRelatedToEdge);
                    break;
                case 6:
                    avro::decode(d, v.facesProximityToPrimaryEdge);
                    break;
                case 7:
                    avro::decode(d, v.facesProximityToSecondaryEdge);
                    break;
                case 8:
                    avro::decode(d, v.holeFeatureId);
                    break;
                case 9:
                    avro::decode(d, v.length);
                    break;
                case 10:
                    avro::decode(d, v.isEnabledGrooveWelding);
                    break;
                case 11:
                    avro::decode(d, v.ignorableReasons);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.primaryFaces);
            avro::decode(d, v.secondaryFaces);
            avro::decode(d, v.primaryEdge);
            avro::decode(d, v.secondaryEdge);
            avro::decode(d, v.facesRelatedToEdge);
            avro::decode(d, v.facesProximityToPrimaryEdge);
            avro::decode(d, v.facesProximityToSecondaryEdge);
            avro::decode(d, v.holeFeatureId);
            avro::decode(d, v.length);
            avro::decode(d, v.isEnabledGrooveWelding);
            avro::decode(d, v.ignorableReasons);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::NonWeldingPart> {
    static void encode(Encoder& e, const fr::recognize_welding::NonWeldingPart& v) {
        avro::encode(e, v.id);
    }
    static void decode(Decoder& d, fr::recognize_welding::NonWeldingPart& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::SurfaceWeldingPart> {
    static void encode(Encoder& e, const fr::recognize_welding::SurfaceWeldingPart& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.primaryFace);
        avro::encode(e, v.secondaryFace);
    }
    static void decode(Decoder& d, fr::recognize_welding::SurfaceWeldingPart& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.primaryFace);
                    break;
                case 2:
                    avro::decode(d, v.secondaryFace);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.primaryFace);
            avro::decode(d, v.secondaryFace);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__0__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_SheetAssembly());
            break;
        case 1:
            avro::encode(e, v.get_SideWelding());
            break;
        case 2:
            avro::encode(e, v.get_SurfaceWelding());
            break;
        case 3:
            avro::encode(e, v.get_LineWeldingPart());
            break;
        case 4:
            avro::encode(e, v.get_NonWeldingPart());
            break;
        case 5:
            avro::encode(e, v.get_SurfaceWeldingPart());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 6) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                fr::recognize_welding::SheetAssembly vv;
                avro::decode(d, vv);
                v.set_SheetAssembly(vv);
            }
            break;
        case 1:
            {
                fr::recognize_welding::SideWelding vv;
                avro::decode(d, vv);
                v.set_SideWelding(vv);
            }
            break;
        case 2:
            {
                fr::recognize_welding::SurfaceWelding vv;
                avro::decode(d, vv);
                v.set_SurfaceWelding(vv);
            }
            break;
        case 3:
            {
                fr::recognize_welding::LineWeldingPart vv;
                avro::decode(d, vv);
                v.set_LineWeldingPart(vv);
            }
            break;
        case 4:
            {
                fr::recognize_welding::NonWeldingPart vv;
                avro::decode(d, vv);
                v.set_NonWeldingPart(vv);
            }
            break;
        case 5:
            {
                fr::recognize_welding::SurfaceWeldingPart vv;
                avro::decode(d, vv);
                v.set_SurfaceWeldingPart(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Vector3D> {
    static void encode(Encoder& e, const fr::recognize_welding::Vector3D& v) {
        avro::encode(e, v.x);
        avro::encode(e, v.y);
        avro::encode(e, v.z);
    }
    static void decode(Decoder& d, fr::recognize_welding::Vector3D& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.x);
                    break;
                case 1:
                    avro::decode(d, v.y);
                    break;
                case 2:
                    avro::decode(d, v.z);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.x);
            avro::decode(d, v.y);
            avro::decode(d, v.z);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Parameter> {
    static void encode(Encoder& e, const fr::recognize_welding::Parameter& v) {
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, fr::recognize_welding::Parameter& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__1__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_double());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                double vv;
                avro::decode(d, vv);
                v.set_double(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__2__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__2__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_double());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__2__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                double vv;
                avro::decode(d, vv);
                v.set_double(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Interval> {
    static void encode(Encoder& e, const fr::recognize_welding::Interval& v) {
        avro::encode(e, v.start);
        avro::encode(e, v.end);
    }
    static void decode(Decoder& d, fr::recognize_welding::Interval& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.start);
                    break;
                case 1:
                    avro::decode(d, v.end);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.start);
            avro::decode(d, v.end);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Circle> {
    static void encode(Encoder& e, const fr::recognize_welding::Circle& v) {
        avro::encode(e, v.center);
        avro::encode(e, v.normal);
        avro::encode(e, v.majorAxis);
        avro::encode(e, v.radiusRatio);
        avro::encode(e, v.paramOff);
        avro::encode(e, v.periodic);
        avro::encode(e, v.paramRange);
    }
    static void decode(Decoder& d, fr::recognize_welding::Circle& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.center);
                    break;
                case 1:
                    avro::decode(d, v.normal);
                    break;
                case 2:
                    avro::decode(d, v.majorAxis);
                    break;
                case 3:
                    avro::decode(d, v.radiusRatio);
                    break;
                case 4:
                    avro::decode(d, v.paramOff);
                    break;
                case 5:
                    avro::decode(d, v.periodic);
                    break;
                case 6:
                    avro::decode(d, v.paramRange);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.center);
            avro::decode(d, v.normal);
            avro::decode(d, v.majorAxis);
            avro::decode(d, v.radiusRatio);
            avro::decode(d, v.paramOff);
            avro::decode(d, v.periodic);
            avro::decode(d, v.paramRange);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Ellipse> {
    static void encode(Encoder& e, const fr::recognize_welding::Ellipse& v) {
        avro::encode(e, v.center);
        avro::encode(e, v.normal);
        avro::encode(e, v.majorAxis);
        avro::encode(e, v.radiusRatio);
        avro::encode(e, v.paramOff);
        avro::encode(e, v.periodic);
        avro::encode(e, v.paramRange);
    }
    static void decode(Decoder& d, fr::recognize_welding::Ellipse& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.center);
                    break;
                case 1:
                    avro::decode(d, v.normal);
                    break;
                case 2:
                    avro::decode(d, v.majorAxis);
                    break;
                case 3:
                    avro::decode(d, v.radiusRatio);
                    break;
                case 4:
                    avro::decode(d, v.paramOff);
                    break;
                case 5:
                    avro::decode(d, v.periodic);
                    break;
                case 6:
                    avro::decode(d, v.paramRange);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.center);
            avro::decode(d, v.normal);
            avro::decode(d, v.majorAxis);
            avro::decode(d, v.radiusRatio);
            avro::decode(d, v.paramOff);
            avro::decode(d, v.periodic);
            avro::decode(d, v.paramRange);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__3__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__3__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_Circle());
            break;
        case 1:
            avro::encode(e, v.get_Ellipse());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__3__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                fr::recognize_welding::Circle vv;
                avro::decode(d, vv);
                v.set_Circle(vv);
            }
            break;
        case 1:
            {
                fr::recognize_welding::Ellipse vv;
                avro::decode(d, vv);
                v.set_Ellipse(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Cone> {
    static void encode(Encoder& e, const fr::recognize_welding::Cone& v) {
        avro::encode(e, v.base);
        avro::encode(e, v.sineAngle);
        avro::encode(e, v.cosineAngle);
        avro::encode(e, v.reverseU);
        avro::encode(e, v.uParamScale);
        avro::encode(e, v.isCylinder);
    }
    static void decode(Decoder& d, fr::recognize_welding::Cone& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.base);
                    break;
                case 1:
                    avro::decode(d, v.sineAngle);
                    break;
                case 2:
                    avro::decode(d, v.cosineAngle);
                    break;
                case 3:
                    avro::decode(d, v.reverseU);
                    break;
                case 4:
                    avro::decode(d, v.uParamScale);
                    break;
                case 5:
                    avro::decode(d, v.isCylinder);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.base);
            avro::decode(d, v.sineAngle);
            avro::decode(d, v.cosineAngle);
            avro::decode(d, v.reverseU);
            avro::decode(d, v.uParamScale);
            avro::decode(d, v.isCylinder);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__4__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__4__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_Circle());
            break;
        case 1:
            avro::encode(e, v.get_Ellipse());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__4__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                fr::recognize_welding::Circle vv;
                avro::decode(d, vv);
                v.set_Circle(vv);
            }
            break;
        case 1:
            {
                fr::recognize_welding::Ellipse vv;
                avro::decode(d, vv);
                v.set_Ellipse(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Cylinder> {
    static void encode(Encoder& e, const fr::recognize_welding::Cylinder& v) {
        avro::encode(e, v.base);
        avro::encode(e, v.sineAngle);
        avro::encode(e, v.cosineAngle);
        avro::encode(e, v.reverseU);
        avro::encode(e, v.uParamScale);
        avro::encode(e, v.isCylinder);
        avro::encode(e, v.radius);
    }
    static void decode(Decoder& d, fr::recognize_welding::Cylinder& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.base);
                    break;
                case 1:
                    avro::decode(d, v.sineAngle);
                    break;
                case 2:
                    avro::decode(d, v.cosineAngle);
                    break;
                case 3:
                    avro::decode(d, v.reverseU);
                    break;
                case 4:
                    avro::decode(d, v.uParamScale);
                    break;
                case 5:
                    avro::decode(d, v.isCylinder);
                    break;
                case 6:
                    avro::decode(d, v.radius);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.base);
            avro::decode(d, v.sineAngle);
            avro::decode(d, v.cosineAngle);
            avro::decode(d, v.reverseU);
            avro::decode(d, v.uParamScale);
            avro::decode(d, v.isCylinder);
            avro::decode(d, v.radius);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Facet> {
    static void encode(Encoder& e, const fr::recognize_welding::Facet& v) {
        avro::encode(e, v.points);
        avro::encode(e, v.normals);
    }
    static void decode(Decoder& d, fr::recognize_welding::Facet& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.points);
                    break;
                case 1:
                    avro::decode(d, v.normals);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.points);
            avro::decode(d, v.normals);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Sphere> {
    static void encode(Encoder& e, const fr::recognize_welding::Sphere& v) {
        avro::encode(e, v.center);
        avro::encode(e, v.poleDir);
        avro::encode(e, v.radius);
        avro::encode(e, v.reverseV);
        avro::encode(e, v.uvOridir);
    }
    static void decode(Decoder& d, fr::recognize_welding::Sphere& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.center);
                    break;
                case 1:
                    avro::decode(d, v.poleDir);
                    break;
                case 2:
                    avro::decode(d, v.radius);
                    break;
                case 3:
                    avro::decode(d, v.reverseV);
                    break;
                case 4:
                    avro::decode(d, v.uvOridir);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.center);
            avro::decode(d, v.poleDir);
            avro::decode(d, v.radius);
            avro::decode(d, v.reverseV);
            avro::decode(d, v.uvOridir);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::SurfacePlane> {
    static void encode(Encoder& e, const fr::recognize_welding::SurfacePlane& v) {
        avro::encode(e, v.normal);
        avro::encode(e, v.reverseV);
        avro::encode(e, v.rootPoint);
        avro::encode(e, v.uDeriv);
    }
    static void decode(Decoder& d, fr::recognize_welding::SurfacePlane& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.normal);
                    break;
                case 1:
                    avro::decode(d, v.reverseV);
                    break;
                case 2:
                    avro::decode(d, v.rootPoint);
                    break;
                case 3:
                    avro::decode(d, v.uDeriv);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.normal);
            avro::decode(d, v.reverseV);
            avro::decode(d, v.rootPoint);
            avro::decode(d, v.uDeriv);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Torus> {
    static void encode(Encoder& e, const fr::recognize_welding::Torus& v) {
        avro::encode(e, v.center);
        avro::encode(e, v.majorRadius);
        avro::encode(e, v.minorRadius);
        avro::encode(e, v.normal);
        avro::encode(e, v.reverseV);
        avro::encode(e, v.uvOridir);
    }
    static void decode(Decoder& d, fr::recognize_welding::Torus& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.center);
                    break;
                case 1:
                    avro::decode(d, v.majorRadius);
                    break;
                case 2:
                    avro::decode(d, v.minorRadius);
                    break;
                case 3:
                    avro::decode(d, v.normal);
                    break;
                case 4:
                    avro::decode(d, v.reverseV);
                    break;
                case 5:
                    avro::decode(d, v.uvOridir);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.center);
            avro::decode(d, v.majorRadius);
            avro::decode(d, v.minorRadius);
            avro::decode(d, v.normal);
            avro::decode(d, v.reverseV);
            avro::decode(d, v.uvOridir);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__5__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__5__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_Cone());
            break;
        case 1:
            avro::encode(e, v.get_Cylinder());
            break;
        case 2:
            avro::encode(e, v.get_Facet());
            break;
        case 3:
            avro::encode(e, v.get_Sphere());
            break;
        case 4:
            avro::encode(e, v.get_SurfacePlane());
            break;
        case 5:
            avro::encode(e, v.get_Torus());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__5__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 6) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                fr::recognize_welding::Cone vv;
                avro::decode(d, vv);
                v.set_Cone(vv);
            }
            break;
        case 1:
            {
                fr::recognize_welding::Cylinder vv;
                avro::decode(d, vv);
                v.set_Cylinder(vv);
            }
            break;
        case 2:
            {
                fr::recognize_welding::Facet vv;
                avro::decode(d, vv);
                v.set_Facet(vv);
            }
            break;
        case 3:
            {
                fr::recognize_welding::Sphere vv;
                avro::decode(d, vv);
                v.set_Sphere(vv);
            }
            break;
        case 4:
            {
                fr::recognize_welding::SurfacePlane vv;
                avro::decode(d, vv);
                v.set_SurfacePlane(vv);
            }
            break;
        case 5:
            {
                fr::recognize_welding::Torus vv;
                avro::decode(d, vv);
                v.set_Torus(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Face> {
    static void encode(Encoder& e, const fr::recognize_welding::Face& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.loops);
        avro::encode(e, v.surface);
        avro::encode(e, v.sense);
        avro::encode(e, v.area);
    }
    static void decode(Decoder& d, fr::recognize_welding::Face& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.loops);
                    break;
                case 2:
                    avro::decode(d, v.surface);
                    break;
                case 3:
                    avro::decode(d, v.sense);
                    break;
                case 4:
                    avro::decode(d, v.area);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.loops);
            avro::decode(d, v.surface);
            avro::decode(d, v.sense);
            avro::decode(d, v.area);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Polyline> {
    static void encode(Encoder& e, const fr::recognize_welding::Polyline& v) {
        avro::encode(e, v.points);
    }
    static void decode(Decoder& d, fr::recognize_welding::Polyline& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.points);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.points);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Straight> {
    static void encode(Encoder& e, const fr::recognize_welding::Straight& v) {
        avro::encode(e, v.rootPoint);
        avro::encode(e, v.direction);
        avro::encode(e, v.paramScale);
    }
    static void decode(Decoder& d, fr::recognize_welding::Straight& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.rootPoint);
                    break;
                case 1:
                    avro::decode(d, v.direction);
                    break;
                case 2:
                    avro::decode(d, v.paramScale);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.rootPoint);
            avro::decode(d, v.direction);
            avro::decode(d, v.paramScale);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__6__> {
    static void encode(Encoder& e, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__6__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_Circle());
            break;
        case 1:
            avro::encode(e, v.get_Ellipse());
            break;
        case 2:
            avro::encode(e, v.get_Polyline());
            break;
        case 3:
            avro::encode(e, v.get_Straight());
            break;
        }
    }
    static void decode(Decoder& d, fr::recognize_welding::_RecognizedWeldingResultRaw_json_Union__6__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 4) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                fr::recognize_welding::Circle vv;
                avro::decode(d, vv);
                v.set_Circle(vv);
            }
            break;
        case 1:
            {
                fr::recognize_welding::Ellipse vv;
                avro::decode(d, vv);
                v.set_Ellipse(vv);
            }
            break;
        case 2:
            {
                fr::recognize_welding::Polyline vv;
                avro::decode(d, vv);
                v.set_Polyline(vv);
            }
            break;
        case 3:
            {
                fr::recognize_welding::Straight vv;
                avro::decode(d, vv);
                v.set_Straight(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Edge> {
    static void encode(Encoder& e, const fr::recognize_welding::Edge& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.paramRange);
        avro::encode(e, v.start);
        avro::encode(e, v.end);
        avro::encode(e, v.curve);
        avro::encode(e, v.sense);
    }
    static void decode(Decoder& d, fr::recognize_welding::Edge& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.paramRange);
                    break;
                case 2:
                    avro::decode(d, v.start);
                    break;
                case 3:
                    avro::decode(d, v.end);
                    break;
                case 4:
                    avro::decode(d, v.curve);
                    break;
                case 5:
                    avro::decode(d, v.sense);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.paramRange);
            avro::decode(d, v.start);
            avro::decode(d, v.end);
            avro::decode(d, v.curve);
            avro::decode(d, v.sense);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Vertex> {
    static void encode(Encoder& e, const fr::recognize_welding::Vertex& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.position);
    }
    static void decode(Decoder& d, fr::recognize_welding::Vertex& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.position);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.position);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Loop> {
    static void encode(Encoder& e, const fr::recognize_welding::Loop& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.coedges);
        avro::encode(e, v.face);
    }
    static void decode(Decoder& d, fr::recognize_welding::Loop& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.coedges);
                    break;
                case 2:
                    avro::decode(d, v.face);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.coedges);
            avro::decode(d, v.face);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Coedge> {
    static void encode(Encoder& e, const fr::recognize_welding::Coedge& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.edge);
        avro::encode(e, v.loop);
        avro::encode(e, v.sense);
    }
    static void decode(Decoder& d, fr::recognize_welding::Coedge& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.edge);
                    break;
                case 2:
                    avro::decode(d, v.loop);
                    break;
                case 3:
                    avro::decode(d, v.sense);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.edge);
            avro::decode(d, v.loop);
            avro::decode(d, v.sense);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::Topologies> {
    static void encode(Encoder& e, const fr::recognize_welding::Topologies& v) {
        avro::encode(e, v.faces);
        avro::encode(e, v.edges);
        avro::encode(e, v.vertices);
        avro::encode(e, v.loops);
        avro::encode(e, v.coedges);
    }
    static void decode(Decoder& d, fr::recognize_welding::Topologies& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.faces);
                    break;
                case 1:
                    avro::decode(d, v.edges);
                    break;
                case 2:
                    avro::decode(d, v.vertices);
                    break;
                case 3:
                    avro::decode(d, v.loops);
                    break;
                case 4:
                    avro::decode(d, v.coedges);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.faces);
            avro::decode(d, v.edges);
            avro::decode(d, v.vertices);
            avro::decode(d, v.loops);
            avro::decode(d, v.coedges);
        }
    }
};

template<> struct codec_traits<fr::recognize_welding::RecognizedWeldingResultRaw> {
    static void encode(Encoder& e, const fr::recognize_welding::RecognizedWeldingResultRaw& v) {
        avro::encode(e, v.version);
        avro::encode(e, v.features);
        avro::encode(e, v.solidIdList);
        avro::encode(e, v.products);
        avro::encode(e, v.virtualTopologies);
    }
    static void decode(Decoder& d, fr::recognize_welding::RecognizedWeldingResultRaw& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.version);
                    break;
                case 1:
                    avro::decode(d, v.features);
                    break;
                case 2:
                    avro::decode(d, v.solidIdList);
                    break;
                case 3:
                    avro::decode(d, v.products);
                    break;
                case 4:
                    avro::decode(d, v.virtualTopologies);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.version);
            avro::decode(d, v.features);
            avro::decode(d, v.solidIdList);
            avro::decode(d, v.products);
            avro::decode(d, v.virtualTopologies);
        }
    }
};

}
#endif
