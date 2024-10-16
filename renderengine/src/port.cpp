#include "renderengine/port.hpp"

#include <numbers>

namespace port {

HPS::SegmentKey getLinePort(HPS::Point p1, HPS::Point p2) {
    auto line = getLine(p1, p2);
    line.GetMaterialMappingControl().SetLineColor(HPS::RGBColor(0, 0, 1));

    auto cone = getCone(p1, p2);
    cone.GetMaterialMappingControl().SetFaceColor(HPS::RGBColor(1, 0, 0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(line);
    root.IncludeSegment(cone);

    return root;
}

HPS::SegmentKey getLinePortPreview(HPS::Point p1, HPS::Point p2) {
    auto line = getLine(p1, p2);
    line.GetMaterialMappingControl().SetLineColor(
        HPS::RGBColor(255 / 255.0, 128 / 255.0, 64 / 255.0));

    auto cone = getCone(p1, p2);
    cone.GetMaterialMappingControl().SetFaceColor(
        HPS::RGBColor(167 / 255.0, 84 / 255.0, 42 / 255.0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(line);
    root.IncludeSegment(cone);

    return root;
}

HPS::SegmentKey getQuadrilateralFacePort(HPS::Point p1, HPS::Point p2,
                                         HPS::Point o1, HPS::Point o2) {
    auto transparentFace = getTransparentFace(HPS::PointArray{p1, p2, o2, o1});

    // auto linePort = getLinePort((p1 + p2) / 2, (o1 + o2) / 2);
    // auto line = getLine((p1 + o1) / 2, (p2 + o2) / 2);
    auto linePort = getLinePort(
        HPS::Point{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2},
        HPS::Point{(o1.x + o2.x) / 2, (o1.y + o2.y) / 2, (o1.z + o2.z) / 2});
    auto line = getLine(
        HPS::Point{(p1.x + o1.x) / 2, (p1.y + o1.y) / 2, (p1.z + o1.z) / 2},
        HPS::Point{(p2.x + o2.x) / 2, (p2.y + o2.y) / 2, (p2.z + o2.z) / 2});
    line.GetMaterialMappingControl().SetLineColor(HPS::RGBColor(1, 0, 0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(transparentFace);
    root.IncludeSegment(linePort);
    root.IncludeSegment(line);

    return root;
}

HPS::SegmentKey getQuadrilateralFacePortPreview(HPS::Point p1, HPS::Point p2,
                                                HPS::Point o1, HPS::Point o2) {
    auto transparentFace = getTransparentFace(HPS::PointArray{p1, p2, o2, o1});

    // auto linePort = getLinePort((p1 + p2) / 2, (o1 + o2) / 2);
    // auto line = getLine((p1 + o1) / 2, (p2 + o2) / 2);
    auto linePortPreview = getLinePortPreview(
        HPS::Point{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2},
        HPS::Point{(o1.x + o2.x) / 2, (o1.y + o2.y) / 2, (o1.z + o2.z) / 2});
    auto line = getLine(
        HPS::Point{(p1.x + o1.x) / 2, (p1.y + o1.y) / 2, (p1.z + o1.z) / 2},
        HPS::Point{(p2.x + o2.x) / 2, (p2.y + o2.y) / 2, (p2.z + o2.z) / 2});
    line.GetMaterialMappingControl().SetLineColor(
        HPS::RGBColor(255 / 255.0, 128 / 255.0, 64 / 255.0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(transparentFace);
    root.IncludeSegment(linePortPreview);
    root.IncludeSegment(line);

    return root;
}

HPS::SegmentKey getHoopFacePort(types::Ellipse ellipse1,
                                types::Ellipse ellipse2) {
    // assert contains otherwise swap

    auto transparentFace = getTransparentFace(ellipse1, ellipse2);

    auto [point1, point2] = getClosestPointPair(ellipse1, ellipse2);
    auto linePort = getLinePort(point1, point2);

    auto lines = getAverageLines(ellipse1, ellipse2);
    lines.GetMaterialMappingControl().SetLineColor(HPS::RGBColor(1, 0, 0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(transparentFace);
    root.IncludeSegment(linePort);
    root.IncludeSegment(lines);

    return root;
}

HPS::SegmentKey getHoopFacePortPreview(types::Ellipse ellipse1,
                                       types::Ellipse ellipse2) {
    // assert contains otherwise swap

    auto transparentFace = getTransparentFace(ellipse1, ellipse2);

    auto [point1, point2] = getClosestPointPair(ellipse1, ellipse2);
    auto linePortPreview = getLinePortPreview(point1, point2);

    auto lines = getAverageLines(ellipse1, ellipse2);
    lines.GetMaterialMappingControl().SetLineColor(
        HPS::RGBColor(255 / 255.0, 128 / 255.0, 64 / 255.0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(transparentFace);
    root.IncludeSegment(linePortPreview);
    root.IncludeSegment(lines);

    return root;
}

HPS::SegmentKey getLine(HPS::Point p1, HPS::Point p2) {
    // clang-format off
    HPS::PointArray points{
        p1, p2
    };
    // clang-format on

    HPS::LineKit line;
    line.SetPoints(points);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertLine(line);
    root.GetVisibilityControl().SetLines(true);
    root.GetLineAttributeControl().SetWeight(5);

    return root;
}

HPS::SegmentKey getCone(HPS::Point p1, HPS::Point p2) {
    HPS::Point centerPoint{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2,
                           (p1.z + p2.z) / 2};

    // clang-format off
    HPS::PointArray points{
        centerPoint + HPS::Vector{p1 - p2}.Normalize() * 0.125,
        centerPoint + HPS::Vector{p2 - p1}.Normalize() * 0.125
    };
    HPS::FloatArray radii{
        0, 0.25
    };
    // clang-format on

    HPS::CylinderKit cone;
    cone.SetPoints(points);
    cone.SetRadii(radii);
    cone.SetCaps(HPS::Cylinder::Capping::Both);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertCylinder(cone);

    return root;
}

HPS::SegmentKey getAverageLines(types::Ellipse ellipse1,
                                types::Ellipse ellipse2) {
    if (std::numbers::pi * ellipse1.major * ellipse1.minor <
        std::numbers::pi * ellipse2.major * ellipse2.minor) {
        std::swap(ellipse1, ellipse2);
    }

    auto polygon1 = getEllipsePolygonPoints(ellipse1);
    auto polygon2 = getEllipsePolygonPoints(ellipse2);
    assert(polygon1.size() == polygon2.size());

    HPS::PointArray polygon(polygon1.size());
    for (int n = polygon1.size(), i = 0; i < n; ++i) {
        auto point = (polygon1[i] + polygon2[i]) / 2;
        polygon[i] = HPS::Point{point.x(), point.y(), point.z()};
    }
    polygon.emplace_back(polygon.front());

    HPS::LineKit lineKit;
    lineKit.SetPoints(polygon);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertLine(lineKit);
    root.GetVisibilityControl().SetLines(true);
    root.GetLineAttributeControl().SetWeight(5);

    return root;
}

HPS::SegmentKey getTransparentFace(HPS::PointArray points) {
    HPS::PolygonKit polygon;
    polygon.SetPoints(points);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertPolygon(polygon);
    root.GetMaterialMappingControl().SetFaceAlpha(0.5);

    return root;
}

HPS::SegmentKey getTransparentFace(types::Ellipse ellipse1,
                                   types::Ellipse ellipse2) {
    HPS::PointArray points{};
    for (auto ellipse : {ellipse1, ellipse2}) {
        auto polygon = getEllipsePolygonPoints(ellipse);

        for (auto point : polygon) {
            points.emplace_back(point.x(), point.y(), point.z());
        }
    }

    HPS::IntArray faces{};
    faces.emplace_back(100);
    for (int i = 0; i < 100; ++i) {
        faces.emplace_back(i);
    }
    faces.emplace_back(-100);
    for (int i = 100; i < 200; ++i) {
        faces.emplace_back(i);
    }

    HPS::ShellKit shell;
    shell.SetPoints(points);
    shell.SetFacelist(faces);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertShell(shell);
    root.GetMaterialMappingControl().SetFaceAlpha(0.5);

    return root;
}

std::vector<QVector3D> getEllipsePolygonPoints(types::Ellipse ellipse) {
    auto [centerPoint, normal, major, minor, cnt] = ellipse;

    normal.normalize();

    QVector3D w(0, 0, 1);
    if (std::abs(normal.x()) < 1e-6 && std::abs(normal.y()) < 1e-6) {
        w = QVector3D(1, 0, 0);
    }

    QVector3D u = QVector3D::crossProduct(w, normal).normalized();
    QVector3D v = QVector3D::crossProduct(normal, u).normalized();

    std::vector<QVector3D> points;
    points.reserve(cnt);

    for (int i = 0; i < cnt; ++i) {
        double theta = 2.0 * std::acos(-1) * i / cnt;
        QVector3D point = centerPoint + u * (major * std::cos(theta)) +
                          v * (minor * std::sin(theta));
        points.push_back(point);
    }

    return points;
}

std::pair<HPS::Point, HPS::Point> getClosestPointPair(types::Ellipse ellipse1,
                                                      types::Ellipse ellipse2) {
    auto polygon1 = getEllipsePolygonPoints(ellipse1);
    auto polygon2 = getEllipsePolygonPoints(ellipse2);

    double distance = std::numeric_limits<double>::infinity();
    std::pair<QVector3D, QVector3D> ans;
    for (auto point1 : polygon1) {
        for (auto point2 : polygon2) {
            double currentDistance = point1.distanceToPoint(point2);
            if (currentDistance < distance) {
                distance = currentDistance;
                ans = {point1, point2};
            }
        }
    }

    return std::pair<HPS::Point, HPS::Point>{
        HPS::Point{ ans.first.x(),  ans.first.y(),  ans.first.z()},
        HPS::Point{ans.second.x(), ans.second.y(), ans.second.z()}
    };
}

}  // namespace port
