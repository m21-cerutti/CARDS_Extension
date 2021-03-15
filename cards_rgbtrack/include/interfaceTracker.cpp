#include "interfaceTracker.h"

bool CardsKCF::init(InputArray image, const Rect2d& boundingBox)
{
    return false;
}

bool CardsKCF::update(InputArray image, Rect2d& boundingBox)
{
    return tracker->update(image, boundingBox);
}

Ptr<ITracker> CardsKCF::create()
{
    return Ptr<ITracker>();
}
