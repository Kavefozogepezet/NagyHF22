#pragma once

#include "Polygon.h"
#include "general/string.h"

namespace CollSys {
    class RegularPolygon :
        public Polygon
    {
    public:
        RegularPolygon(const glib::string& type);

        virtual bool fromConsole(std::stringstream& buff);
    private:
        void build(size_t pointc, double edgelen);
    };
}
