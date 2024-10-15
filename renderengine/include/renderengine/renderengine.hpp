#pragma once

#include <QWidget>

#include <hps.h>
#include <sprk.h>

#include "renderengine/types.hpp"

namespace RenderEngine {

class RenderEngine final : public QWidget {
    Q_OBJECT

public:
    explicit RenderEngine(QWidget* parent = nullptr);
    ~RenderEngine() override;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    QPaintEngine* paintEngine() const override;

private:
    HPS::MatrixKit const& getPixelToWindowMatrix();
    void getModifierKeys(HPS::InputEvent* event) const;
    HPS::MouseEvent buildMouseEvent(QMouseEvent* in_event,
                                    HPS::MouseEvent::Action action,
                                    std::size_t click_count);
    HPS::KeyboardEvent buildKeyboardEvent(
        QKeyEvent* in_event, HPS::KeyboardEvent::Action action) const;

private:
    HPS::World license_;

    HPS::Model model_;
    HPS::View view_;
    HPS::Canvas canvas_;

    bool validPixelToWindowMatrix_;
    HPS::MatrixKit pixelToWindowMatrix_;

    void init();

    void render() const;

    HPS::SegmentKey getLinePort(HPS::Point p1, HPS::Point p2) const;
    HPS::SegmentKey getQuadrilateralFacePort(HPS::Point p1, HPS::Point p2,
                                             HPS::Point o1,
                                             HPS::Point o2) const;
    HPS::SegmentKey getHoopFacePort(types::Ellipse ellipse1,
                                    types::Ellipse ellipse2) const;

    HPS::SegmentKey getLine(HPS::Point p1, HPS::Point p2) const;
    HPS::SegmentKey getCone(HPS::Point p1, HPS::Point p2) const;
    HPS::SegmentKey getAverageLines(types::Ellipse ellipse1,
                                    types::Ellipse ellipse2) const;
    HPS::SegmentKey getTransparentFace(HPS::PointArray points) const;
    HPS::SegmentKey getTransparentFace(types::Ellipse ellipse1,
                                       types::Ellipse ellipse2) const;

    std::vector<QVector3D> getEllipsePolygonPoints(
        types::Ellipse ellipse) const;
    std::pair<HPS::Point, HPS::Point> getClosestPointPair(
        types::Ellipse ellipse1, types::Ellipse ellipse2) const;
};

}  // namespace RenderEngine
