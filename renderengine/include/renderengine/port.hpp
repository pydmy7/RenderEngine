#pragma once

#include "renderengine/types.hpp"

#include <hps.h>

namespace port {

HPS::SegmentKey getLinePort(HPS::Point p1, HPS::Point p2);
HPS::SegmentKey getLinePortPreview(HPS::Point p1, HPS::Point p2);
HPS::SegmentKey getQuadrilateralFacePort(HPS::Point p1, HPS::Point p2,
                                         HPS::Point o1, HPS::Point o2);
HPS::SegmentKey getQuadrilateralFacePortPreview(HPS::Point p1, HPS::Point p2,
                                                HPS::Point o1, HPS::Point o2);
HPS::SegmentKey getHoopFacePort(types::Ellipse ellipse1,
                                types::Ellipse ellipse2);
HPS::SegmentKey getHoopFacePortPreview(types::Ellipse ellipse1,
                                       types::Ellipse ellipse2);

HPS::SegmentKey getLine(HPS::Point p1, HPS::Point p2);
HPS::SegmentKey getCone(HPS::Point p1, HPS::Point p2);
HPS::SegmentKey getAverageLines(types::Ellipse ellipse1,
                                types::Ellipse ellipse2);
HPS::SegmentKey getTransparentFace(HPS::PointArray points);
HPS::SegmentKey getTransparentFace(types::Ellipse ellipse1,
                                   types::Ellipse ellipse2);

std::vector<QVector3D> getEllipsePolygonPoints(types::Ellipse ellipse);
std::pair<HPS::Point, HPS::Point> getClosestPointPair(types::Ellipse ellipse1,
                                                      types::Ellipse ellipse2);

}  // namespace port
