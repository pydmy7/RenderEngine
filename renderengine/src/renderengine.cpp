#include "renderengine/renderengine.hpp"

#include <QApplication>
#include <QWheelEvent>

#include <hoops_license.h>
#include <hps.h>
#include <sprk_ops.h>

#include <spdlog/spdlog.h>

namespace RenderEngine {

RenderEngine::RenderEngine(QWidget* parent)
    : QWidget{parent},
      license_{HOOPS_LICENSE},
      validPixelToWindowMatrix_{false},
      pixelToWindowMatrix_{} {
    init();
}

RenderEngine::~RenderEngine() {
    model_.Delete();
    view_.Delete();
    canvas_.GetAttachedLayout().Delete();
    canvas_.Delete();
}

void RenderEngine::resizeEvent(QResizeEvent*) {
    validPixelToWindowMatrix_ = false;
    canvas_.GetWindowKey()
        .UpdateWithNotifier(HPS::Window::UpdateType::Refresh)
        .Wait();
}

void RenderEngine::paintEvent(QPaintEvent*) {
    canvas_.GetWindowKey().Update(HPS::Window::UpdateType::Refresh);
}

void RenderEngine::mousePressEvent(QMouseEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildMouseEvent(event, HPS::MouseEvent::Action::ButtonDown, 1));
}

void RenderEngine::mouseDoubleClickEvent(QMouseEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildMouseEvent(event, HPS::MouseEvent::Action::ButtonDown, 2));
}

void RenderEngine::mouseReleaseEvent(QMouseEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildMouseEvent(event, HPS::MouseEvent::Action::ButtonUp, 0));
}

void RenderEngine::mouseMoveEvent(QMouseEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildMouseEvent(event, HPS::MouseEvent::Action::Move, 0));
}

void RenderEngine::wheelEvent(QWheelEvent* event) {
    HPS::Point pos(event->position().x(), event->position().y(), 0);
    pos = getPixelToWindowMatrix().Transform(pos);

    HPS::MouseEvent out_event;
    out_event.CurrentAction = HPS::MouseEvent::Action::Scroll;
    out_event.Location = pos;
    out_event.WheelDelta = event->angleDelta().y();

    getModifierKeys(&out_event);

    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(out_event);
}

void RenderEngine::keyPressEvent(QKeyEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildKeyboardEvent(event, HPS::KeyboardEvent::Action::KeyDown));
}

void RenderEngine::keyReleaseEvent(QKeyEvent* event) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        buildKeyboardEvent(event, HPS::KeyboardEvent::Action::KeyUp));
}

void RenderEngine::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

void RenderEngine::focusOutEvent(QFocusEvent*) {
    canvas_.GetWindowKey().GetEventDispatcher().InjectEvent(
        HPS::FocusLostEvent());
}

QPaintEngine* RenderEngine::paintEngine() const {
    return nullptr;
}

HPS::MatrixKit const& RenderEngine::getPixelToWindowMatrix() {
    if (!validPixelToWindowMatrix_) {
        HPS::KeyArray key_array;
        key_array.push_back(canvas_.GetWindowKey());
        HPS::KeyPath(key_array).ComputeTransform(HPS::Coordinate::Space::Pixel,
                                                 HPS::Coordinate::Space::Window,
                                                 pixelToWindowMatrix_);
        validPixelToWindowMatrix_ = true;
    }

    return pixelToWindowMatrix_;
}

void RenderEngine::getModifierKeys(HPS::InputEvent* event) const {
    Qt::KeyboardModifiers modifiers{QApplication::keyboardModifiers()};
    if (modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier)) {
        event->ModifierKeyState.Shift(true);
    }
    if (modifiers.testFlag(Qt::KeyboardModifier::ControlModifier)) {
        event->ModifierKeyState.Control(true);
    }
}

HPS::MouseEvent RenderEngine::buildMouseEvent(QMouseEvent* in_event,
                                              HPS::MouseEvent::Action action,
                                              std::size_t click_count) {
    auto scaleFactor = this->devicePixelRatio();
    HPS::Point pos(in_event->x() * scaleFactor, in_event->y() * scaleFactor, 0);
    pos = getPixelToWindowMatrix().Transform(pos);

    HPS::MouseEvent out_event;
    out_event.Location = pos;
    out_event.CurrentAction = action;
    out_event.ClickCount = click_count;
    if (in_event->button() == Qt::MouseButton::LeftButton) {
        out_event.CurrentButton = HPS::MouseButtons::ButtonLeft();
    } else if (in_event->button() == Qt::MouseButton::RightButton) {
        out_event.CurrentButton = HPS::MouseButtons::ButtonRight();
    } else if (in_event->button() == Qt::MouseButton::MiddleButton) {
        out_event.CurrentButton = HPS::MouseButtons::ButtonMiddle();
    }

    getModifierKeys(&out_event);

    return out_event;
}

HPS::KeyboardEvent RenderEngine::buildKeyboardEvent(
    QKeyEvent* in_event, HPS::KeyboardEvent::Action action) const {
    HPS::KeyboardCodeArray code;
    code.push_back(static_cast<HPS::KeyboardCode>(in_event->key()));

    HPS::KeyboardEvent out_event;
    out_event.KeyboardCodes = code;
    out_event.CurrentAction = action;

    getModifierKeys(&out_event);

    return out_event;
}

void RenderEngine::init() {
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
    QWidget::setAttribute(Qt::WA_OpaquePaintEvent);
    QWidget::setAttribute(Qt::WA_NoSystemBackground);
    QWidget::setAttribute(Qt::WA_StaticContents);
    QWidget::setAttribute(Qt::WA_DontCreateNativeAncestors);
    QWidget::setAttribute(Qt::WA_NativeWindow);
    QWidget::setFocusPolicy(Qt::StrongFocus);
    QWidget::setBackgroundRole(QPalette::NoRole);
    QWidget::setAcceptDrops(true);
    this->resize(1000, 700);

    model_ = HPS::Factory::CreateModel();

    view_ = HPS::Factory::CreateView();
    view_.AttachModel(model_);
    view_.GetOperatorControl()
        .Push(new HPS::MouseWheelOperator(), HPS::Operator::Priority::Low)
        .Push(new HPS::ZoomOperator(HPS::MouseButtons::ButtonMiddle()))
        .Push(new HPS::PanOperator(HPS::MouseButtons::ButtonRight()))
        .Push(new HPS::OrbitOperator(HPS::MouseButtons::ButtonLeft()));
    view_.GetAxisTriadControl().SetVisibility(true).SetLocation(
        HPS::AxisTriadControl::Location::BottomRight);
    view_.GetNavigationCubeControl().SetVisibility(true).SetLocation(
        HPS::NavigationCubeControl::Location::BottomLeft);

    canvas_ = HPS::Factory::CreateCanvas(
        static_cast<HPS::WindowHandle>(this->winId()), "RenderEngine");
    canvas_.AttachViewAsLayout(view_);

    render();
}

void RenderEngine::render() const {
    auto linePort = getLinePort(HPS::Point{0, 0, 0}, HPS::Point{-1, -1, -1});
    linePort.SetName("LinePort");

    auto facePort =
        getQuadrilateralFacePort(HPS::Point{0, 0, 1}, HPS::Point{0, 1, 1},
                                 HPS::Point{1, 0, 1}, HPS::Point{1, 1, 1});
    facePort.SetName("FacePort");

    HPS::SegmentKey root = model_.GetSegmentKey();
    root.IncludeSegment(linePort);
    root.IncludeSegment(facePort);
}

HPS::SegmentKey RenderEngine::getLinePort(HPS::Point p1, HPS::Point p2) const {
    auto line = getLine(p1, p2);
    line.GetMaterialMappingControl().SetLineColor(HPS::RGBColor(0, 0, 1));

    auto cone = getCone(p1, p2);
    cone.GetMaterialMappingControl().SetFaceColor(HPS::RGBColor(1, 0, 0));

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(line);
    root.IncludeSegment(cone);

    return root;
}

HPS::SegmentKey RenderEngine::getQuadrilateralFacePort(HPS::Point p1,
                                                       HPS::Point p2,
                                                       HPS::Point o1,
                                                       HPS::Point o2) const {
    auto transparentFace = getTransparentFace(HPS::PointArray{p1, p2, o2, o1});
    // auto linePort = getLinePort((p1 + p2) / 2, (o1 + o2) / 2);
    // auto line = getLine((p1 + o1) / 2, (p2 + o2) / 2);
    auto linePort = getLinePort(
        HPS::Point{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2},
        HPS::Point{(o1.x + o2.x) / 2, (o1.y + o2.y) / 2, (o1.z + o2.z) / 2});
    auto line = getLine(
        HPS::Point{(p1.x + o1.x) / 2, (p1.y + o1.y) / 2, (p1.z + o1.z) / 2},
        HPS::Point{(p2.x + o2.x) / 2, (p2.y + o2.y) / 2, (p2.z + o2.z) / 2});

    auto root = HPS::Database::CreateRootSegment();
    root.IncludeSegment(transparentFace);
    root.IncludeSegment(linePort);
    root.IncludeSegment(line);

    return root;
}

HPS::SegmentKey RenderEngine::getLine(HPS::Point p1, HPS::Point p2) const {
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

HPS::SegmentKey RenderEngine::getCone(HPS::Point p1, HPS::Point p2) const {
    HPS::Point centerPoint{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2,
                           (p1.z + p2.z) / 2};

    // clang-format off
    HPS::PointArray points{
        getTranslatePoint(centerPoint, HPS::Vector{p1 - p2}, 0.125),
        getTranslatePoint(centerPoint, HPS::Vector{p2 - p1}, 0.125)
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

HPS::SegmentKey RenderEngine::getTransparentFace(
    const HPS::PointArray& points) const {
    HPS::PolygonKit polygon;
    polygon.SetPoints(points);

    auto root = HPS::Database::CreateRootSegment();
    root.InsertPolygon(polygon);
    root.GetMaterialMappingControl().SetFaceAlpha(0.5);

    return root;
}

HPS::Point RenderEngine::getTranslatePoint(HPS::Point p, HPS::Vector v,
                                           double len) const {
    v = v.Normalize();
    return HPS::Point{static_cast<float>(p.x + v.x * len),
                      static_cast<float>(p.y + v.y * len),
                      static_cast<float>(p.z + v.z * len)};
}

// HPS::SegmentKey RenderEngine::getHoopFacePort(types::Circle<float> circle1,
// types::Circle<float> circle2) const {
//     // assert contains otherwise swap
// }

}  // namespace RenderEngine
