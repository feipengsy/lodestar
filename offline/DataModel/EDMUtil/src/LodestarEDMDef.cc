#include "EDMUtil/BookEDM.h"

//JUNO_BOOK_EDM(JM::SimHeader, JM::SimEvent, 200, /Event/SimEvent);
//JUNO_BOOK_EDM(JM::TestHeaderB, JM::BTestEventA&JM::BTestEventB, 700, /Event/TestB);
JUNO_BOOK_EDM(LHAASO::CorsikaHeader, LHAASO::CorsikaEvent, 700, /Event/CorsikaEvent);
JUNO_BOOK_EDM(LHAASO::KM2ASimHeader, LHAASO::KM2ASimEvent, 777, /Event/KM2ASimEvent);
