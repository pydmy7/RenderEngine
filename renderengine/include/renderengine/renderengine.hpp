#pragma once

#include <QWidget>

#include <hps.h>
#include <sprk.h>

namespace RenderEngine {

class RenderEngine : public QWidget {
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
	HPS::MouseEvent buildMouseEvent(QMouseEvent* in_event, HPS::MouseEvent::Action action, std::size_t click_count);
    HPS::KeyboardEvent buildKeyboardEvent(QKeyEvent* in_event, HPS::KeyboardEvent::Action action) const;

private:
    HPS::World license_;

    HPS::Model model_;
	HPS::View view_;
    HPS::Canvas canvas_;

    bool validPixelToWindowMatrix_;
	HPS::MatrixKit pixelToWindowMatrix_;

    void init();
    void render();
};

}  // namespace RenderEngine
