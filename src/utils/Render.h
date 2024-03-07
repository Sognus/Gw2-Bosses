#include "../Globals.h"
#include "Event.h"
#include "PeriodicEvent.h"

#ifndef RENDER_H
#define RENDER_H

void render_debug_crosshair();

void render_base_event(Event event);

void render_periodic_event(PeriodicEvent event);

void render_periodic_circular_event(PeriodicEvent event);

void render_periodic_circular_event_convergences(PeriodicEvent event);

void render_map_notification_upcoming(Event* notificationEvent);

void render_map_notification_in_progress(Event* notificationEvent);

#endif //RENDER_H