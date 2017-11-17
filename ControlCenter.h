//
// Created by lee on 17-11-17.
//

#ifndef HETSNETS_CONTROLCENTER_H
#define HETSNETS_CONTROLCENTER_H

#include "Interface.h"

class ControCenter : public Interface
{
public:
    ControCenter();

    virtual ~ControCenter();

    static Interface * Create();

    void WorkSlot(default_random_engine dre);
};

#endif //HETSNETS_CONTROLCENTER_H
