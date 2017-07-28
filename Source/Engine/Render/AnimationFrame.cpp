
#include <giga-engine.h>

AnimationFrame::AnimationFrame() {
    time = 0;
}

AnimationFrame::~AnimationFrame() {
    for(size_t i = 0; i < transforms.size(); i++) {
        delete transforms[i];
    }
}
