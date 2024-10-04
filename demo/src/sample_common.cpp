#include "sample_common.h"

void describeComponent(Component component, UTF8 *description) {
    switch (component.GetComponentType()) {
        case Component::ComponentType::ExchangeModelFile:
            *description = *description + "Component Type: Model File";
            break;
        case Component::ComponentType::ExchangeProductOccurrence:
            *description = *description + "Component Type: Product Occurrence";
            break;
        case Component::ComponentType::ExchangePartDefinition:
            *description = *description + "Component Type: Part Definition";
            break;

        case Component::ComponentType::ExchangeView:
            *description = *description + "Component Type: View";
            break;
        case Component::ComponentType::ExchangeFilter:
            *description = *description + "Component Type: Filter";
            break;

        case Component::ComponentType::ExchangeRIBRepModel:
            *description = *description + "Component Type: RI BRep Model";
            break;
        case Component::ComponentType::ExchangeRICurve:
            *description = *description + "Component Type: RI Curve";
            break;
        case Component::ComponentType::ExchangeRIDirection:
            *description = *description + "Component Type: RI Direction";
            break;
        case Component::ComponentType::ExchangeRIPlane:
            *description = *description + "Component Type: RI Plane";
            break;
        case Component::ComponentType::ExchangeRIPointSet:
            *description = *description + "Component Type: RI Point Set";
            break;
        case Component::ComponentType::ExchangeRIPolyBRepModel:
            *description = *description + "Component Type: RI Poly BRep Model";
            break;
        case Component::ComponentType::ExchangeRIPolyWire:
            *description = *description + "Component Type: RI Poly Wire";
            break;
        case Component::ComponentType::ExchangeRISet:
            *description = *description + "Component Type: RI Set";
            break;
        case Component::ComponentType::ExchangeRICoordinateSystem:
            *description =
                *description + "Component Type: RI Coordinate System";
            break;

        case Component::ComponentType::ExchangeTopoBody:
            *description = *description + "Component Type: Topo Body";
            break;
        case Component::ComponentType::ExchangeTopoConnex:
            *description = *description + "Component Type: Topo Connex";
            break;
        case Component::ComponentType::ExchangeTopoShell:
            *description = *description + "Component Type: Topo Shell";
            break;
        case Component::ComponentType::ExchangeTopoFace:
            *description = *description + "Component Type: Topo Face";
            break;
        case Component::ComponentType::ExchangeTopoLoop:
            *description = *description + "Component Type: Topo Loop";
            break;
        case Component::ComponentType::ExchangeTopoCoEdge:
            *description = *description + "Component Type: Topo Co Edge";
            break;
        case Component::ComponentType::ExchangeTopoEdge:
            *description = *description + "Component Type: Topo Edge";
            break;
        case Component::ComponentType::ExchangeTopoVertex:
            *description = *description + "Component Type: Topo Vertex";
            break;
        case Component::ComponentType::ExchangeTopoSingleWireBody:
            *description =
                *description + "Component Type: Topo Single Wire Body";
            break;
        case Component::ComponentType::ExchangeTopoWireEdge:
            *description = *description + "Component Type: Topo Wire Edge";
            break;

        case Component::ComponentType::ExchangePMI:
            *description = *description + "Component Type: PMI";
            break;
        case Component::ComponentType::ExchangePMIText:
            *description = *description + "Component Type: PMI Text";
            break;
        case Component::ComponentType::ExchangePMIRichText:
            *description = *description + "Component Type: PMI Rich Text";
            break;
        case Component::ComponentType::ExchangePMIRoughness:
            *description = *description + "Component Type: PMI Roughness";
            break;
        case Component::ComponentType::ExchangePMIGDT:
            *description = *description + "Component Type: PMI GDT";
            break;
        case Component::ComponentType::ExchangePMIDatum:
            *description = *description + "Component Type: PMI Datum";
            break;
        case Component::ComponentType::ExchangePMILineWelding:
            *description = *description + "Component Type: PMI Line Welding";
            break;
        case Component::ComponentType::ExchangePMISpotWelding:
            *description = *description + "Component Type: PMI Spot Welding";
            break;
        case Component::ComponentType::ExchangePMIDimension:
            *description = *description + "Component Type: PMI Dimension";
            break;

        default:
            *description = *description + "Component Type: UNKNOWN";
    }
}

ShellKey CreateBox(SegmentKey insertHere, Point min, Point max) {
    Point points[8];
    Point point;

    point.x = min.x;
    point.y = min.y;
    point.z = min.z;
    points[0] = point;

    point.x = max.x;
    points[1] = point;

    point.y = max.y;
    points[2] = point;

    point.x = min.x;
    points[3] = point;

    point.y = min.y;
    point.z = max.z;
    points[4] = point;

    point.x = max.x;
    points[5] = point;

    point.y = max.y;
    points[6] = point;

    point.x = min.x;
    points[7] = point;

    int faces[] = {4, 0, 1, 2, 3, 4, 7, 6, 5, 4, 4, 1, 5, 6, 2,
                   4, 4, 0, 3, 7, 4, 3, 2, 6, 7, 4, 4, 5, 1, 0};

    return insertHere.InsertShell(8, points, 30, faces);
}

float DegreesToRadians(float degrees) {
    return (float)(degrees * PI / 180.0f);
}

MeshKey CreateToroid(SegmentKey insertHere) {
    float golden;
    float cx, cy, cz;
    float cosa, sina;
    Vector basis1, basis2;

    PointArray points;
    VectorArray normals;

    golden = (float)(sqrt(5) + 1.0f) * 0.5f;
    cz = 0.0f;

    for (int i = 0; i <= 360; i += 10) {
        cx = golden * (float)cos(DegreesToRadians((float)i));
        cy = (float)sin(DegreesToRadians((float)i));

        basis1.x = 0.0f;
        basis1.y = 0.0f;
        basis1.z = 1.0f;

        basis2.y = golden * (float)sin(DegreesToRadians((float)i));
        basis2.x = (float)cos(DegreesToRadians((float)i));
        basis2.z = 0.0f;
        basis2.Normalize();

        for (int j = 0; j <= 360; j += 15) {
            Point point;
            Vector normal;
            cosa = (float)cos(DegreesToRadians((float)j));
            sina = (float)sin(DegreesToRadians((float)j));

            point.x = cx + 0.2f * (cosa * basis1.x + sina * basis2.x);
            point.y = cy + 0.2f * (cosa * basis1.y + sina * basis2.y);
            point.z = cz + 0.2f * (cosa * basis1.z + sina * basis2.z);
            points.push_back(point);

            normal.x = cosa * basis1.x + sina * basis2.x;
            normal.y = cosa * basis1.y + sina * basis2.y;
            normal.z = cosa * basis1.z + sina * basis2.z;
            normals.push_back(normal);
        }  // end of inner for loop
    }  // end of outer for loop

    MeshKey ret = insertHere.InsertMesh(37, 25, points);

    for (int i = 0; i <= 360; i += 10) {
        for (int j = 0; j <= 360; j += 15) {
            ret.SetVertexNormalsByRange(0, normals);
        }
    }

    return ret;
}

SegmentKey InsertCross(SegmentKey insertHere) {
    SegmentKey crossSegment = insertHere.Subsegment();
    crossSegment.InsertLine(Point(0, 0, 0), Point(1, 0, 0));
    crossSegment.InsertLine(Point(0.5f, 0.5f, 0), Point(0.5f, -0.5f, 0));
    crossSegment.GetVisibilityControl().SetLines(true);
    crossSegment.GetLineAttributeControl().SetWeight(3);

    return crossSegment;
}

EventHandler::HandleResult CustomEventHandler::Handle(Event const *in_event) {
    SegmentKey textKey =
        (reinterpret_cast<CustomEvent const *>(in_event))->textSegmentKey;
    SegmentKey labelKey = textKey.Subsegment();
    labelKey.GetDrawingAttributeControl().SetDepthRange(0.0f, 0.1f);
    labelKey.GetVisibilityControl().SetEverything(false).SetText(true);
    labelKey.GetCameraControl()
        .SetPosition(Point(0, 0, -5))
        .SetTarget(Point(0, 0, 0))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(2, 2)
        .SetProjection(Camera::Projection::Stretched);
    labelKey.GetTextAttributeControl()
        .SetSize(0.02f, Text::SizeUnits::SubscreenRelative)
        .SetAlignment(Text::Alignment::Center,
                      Text::ReferenceFrame::WorldAligned,
                      Text::Justification::Center);
    labelKey.InsertText(Point(0, 0, 0),
                        "Your custom event\n has been handled successfully");

    return HandleResult::Handled;
}

Event *CustomEvent::Clone() const {
    CustomEvent *ce = new CustomEvent(GetChannel());
    ce->textSegmentKey = this->textSegmentKey;
    return ce;
}