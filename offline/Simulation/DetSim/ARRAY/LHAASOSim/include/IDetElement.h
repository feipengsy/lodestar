#ifndef I_DET_ELEMENT_H
#define I_DET_ELEMENT_H 1

#include <string>

class IDetElementPos;
class G4LogicalVolume;

class IDetElement {

    public:
        IDetElement() {}
        virtual ~IDetElement() {}

        virtual G4LogicalVolume* getLV() = 0;
        virtual bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {return true;}

        virtual double geom_info(const std::string& param) {return 0.0;}
        virtual double geom_info(const std::string& param, int idx) {return 0.0;}
};

#endif
