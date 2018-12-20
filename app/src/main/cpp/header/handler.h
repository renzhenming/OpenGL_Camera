#ifndef rzm_opengl_camera_manager_message_queue_handler
#define rzm_opengl_camera_manager_message_queue_handler

#include "message_queue.h"

class Handler {
private:
	MessageQueue* mQueue;

public:
	Handler(MessageQueue* mQueue);
	~Handler();

	int postMessage(Message* msg);
	virtual void handleMessage(Message* msg){};
};

#endif // rzm_opengl_camera_manager_message_queue_handler
