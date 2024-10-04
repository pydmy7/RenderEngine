#ifndef _SAMPLE_COMMON_H_
#define _SAMPLE_COMMON_H_

#include "hps.h"
#include "sprk.h"

#if !defined(__APPLE__) && !defined(TARGET_OS_ANDROID)
#  define SPRK_PUBLISH_SUPPORTED
#endif

using namespace HPS;

ShellKey CreateBox(SegmentKey insertHere, Point min, Point max);
float DegreesToRadians(float degrees);
MeshKey CreateToroid(SegmentKey insertHere);
SegmentKey InsertCross(SegmentKey insertHere);

void describeComponent(Component component, UTF8 * description);

class CustomEvent : public Event
{
public:
	CustomEvent(intptr_t in_channel=0) : Event(in_channel) { channel = in_channel ? in_channel : GetClassID(); }
	CustomEvent(intptr_t in_channel, SegmentKey sKey) : Event(in_channel), textSegmentKey(sKey) {}

	virtual Event * Clone() const;

	SegmentKey textSegmentKey;
};

class CustomEventHandler : public EventHandler
{
public:
	~CustomEventHandler() { Shutdown(); }

	EventHandler::HandleResult Handle(Event const * in_event);
};

#endif // _SAMPLE_COMMON_H_

