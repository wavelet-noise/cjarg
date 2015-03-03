#ifndef STATICITEM_H
#define STATICITEM_H
#include "item.h"
#include <memory>
#include <string>

class StaticItem
{
public:
    StaticItem();
    ~StaticItem();

    std::unique_ptr<Item> etalon;
    std::string full_id;
    int id;
};

#endif // STATICITEM_H