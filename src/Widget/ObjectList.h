#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

#include "../GUI/Widget/ElementList.h"

class ObjectList : public ElementList {
    public:
    ObjectList(int x, int y, int w, int h);

    void RightClickOn(int clickindex) override;
    void LeftClickOn(int clickindex) override;
    void LeftReleaseOn(int releaseindex) override;

    void DrawElement(int painterx, int paintery, int elementindex) override;

    int GetElementCount() override;
};

#endif