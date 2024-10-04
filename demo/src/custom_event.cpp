#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Custom_Event(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{
    /* Sample: Custom_Event
     * This sample will show you how to:
     * - Create a custom event
     * - Create a custom event handler
     * - Inject an event in HPS
     * - Obtain an event dispatcher and subscribe to event notifications
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 10.1:	Event Handling
     * - section 10.2:	Custom Events */

    /* The first necessary step to create a custom event is to register it with
     * HPS. This allows for the creation of an event type */
    intptr_t customEventType = HPS::Object::ClassID<CustomEvent>();

    /* an Event Handler class is responsible for handling particular events.
     * The CustomEventHandler class is implemented in sample_commom.[cpp|h]
     *
     * In order to create a class to be used for event handling, the class will need
     * to derive from HPS::EventHandler, and will need to override this method:
     * EventHandler::HandleResult Handle(Event const * in_event);
     *
     * The CustomEventHandler class responds to events by printing out a message
     * on the screen */
    CustomEventHandler* customEventHandler = new CustomEventHandler();

    /* The next two lines of code show you how to obtain an EventDispatcher
     * and how to subscribe it to a particular event.
     * In this case the second line tells the dispatcher to handle events of
     * type 'customEventType' by using the 'customEventHandler' handler */
    EventDispatcher dispatcher = wk.GetEventDispatcher();
    dispatcher.Subscribe(*customEventHandler, customEventType);

    /* The next block of code creates a CustomEvent object.
     * The CustomEvent class is implemented in sample_common.[cpp|h]
     *
     * In order to create a class to be used as an Event, the class will need
     * to derive from HPS::Event, and will need to override the following method:
     * virtual Event * CustomEvent::Clone() const;
     *
     * The class will also need to call the base constructor from its own constructor
     *
     * Once the event is created, the dispatcher is used to inject it into HPS.
     * Events are handled on separate threads.
     * Events can be dispatched both with or without a notifier.
     * Using a notifier allows you to wait for the Event Handling thread to return
     * before continuing with the execution, as shown below.
     * */
    CustomEvent customEvent(customEventType, modelKey);
    EventNotifier eventNotifier = dispatcher.InjectEventWithNotifier(customEvent);
    eventNotifier.Wait();

    view.FitWorld();
    view.Update();

    delete customEventHandler;
}
