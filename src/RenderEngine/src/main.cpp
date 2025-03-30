#include <vsg/all.h>
#include <vsgXchange/all.h>

#include <iostream>

int main() {
    auto options = vsg::Options::create();
    options->add(vsgXchange::all::create());

    vsg::Path filename = R"(D:\Code\RenderEngine\assets\models\glider.vsgt)";
    auto loaded_scene = vsg::read_cast<vsg::Node>(filename, options);

    auto scene = vsg::Group::create();
    scene->addChild(loaded_scene);
    if (scene->children.empty()) {
        std::cout<<"No scene loaded."<<std::endl;
        return 1;
    }

    auto window(vsg::Window::create(vsg::WindowTraits::create()));
    if (!window) {
        std::cout<<"Could not create window."<<std::endl;
        return 1;
    }

    vsg::ComputeBounds computeBounds;
    scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
    double nearFarRatio = 0.0001;

    auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
    perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio*radius, radius * 4.5);

    auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

    auto rendergraph = vsg::createRenderGraphForView(window, camera, scene);
    rendergraph->setClearValues(vsg::sRGB_to_linear(24 / 255.0f, 24 / 255.0f, 24 / 255.0f, 1.0f));

    auto commandGraph = vsg::CommandGraph::create(window, rendergraph);

    auto viewer = vsg::Viewer::create();
    viewer->addWindow(window);
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));
    viewer->addEventHandler(vsg::Trackball::create(camera));
    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});
    viewer->compile();

    while (viewer->advanceToNextFrame()) {
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    return 0;
}
