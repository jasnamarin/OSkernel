#include "event.h"

Event::Event(IVTNo _ivtNo) { myImpl = new KernelEv(_ivtNo); }

void Event::wait() { myImpl->wait(); }

void Event::signal() { myImpl->signal(); }

Event::~Event() { delete myImpl; }
