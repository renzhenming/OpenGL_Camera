#include "handler.h"
Handler::Handler(MessageQueue* queue) {
	this->mQueue = queue;
}

Handler::~Handler() {
}

int Handler::postMessage(Message* msg){
	msg->handler = this;
	return mQueue->enqueueMessage(msg);
}
