/**
 * @file
 * @brief This API describes function for work with asynchronous event in
 *      threads
 *
 * @date 08.04.11
 * @author Alina Kramar
 */

#ifndef KERNEL_THREAD_EVENT_H_
#define KERNEL_THREAD_EVENT_H_

#include <kernel/thread/sched_strategy.h>

#define EVENT_TIMEOUT_INFINITE ((unsigned int)(-1))

struct event {
	struct sleepq sleepq;
	const char *name;
};

extern void event_init(struct event *event, const char *name);
extern int event_wait_ns(struct event *event, unsigned long timeout);
extern int event_wait_us(struct event *event, unsigned long timeout);
extern int event_wait_ms(struct event *event, unsigned long timeout);
extern void event_notify(struct event *event);

#if 0
extern const char *event_name(struct event *event);
extern void event_notify_one(struct event *event);
#endif

#endif /* KERNEL_THREAD_EVENT_H_ */
