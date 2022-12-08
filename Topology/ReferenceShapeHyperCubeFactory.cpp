#include "ReferenceShapeHyperCubeFactory.h"
#include <memory>

namespace
{
   class ReferenceShapeHyperCube : public IReferenceShapeHyperCube
   {
   public:
   private:

   };
}

const IReferenceShapeHyperCube& ReferenceShapeHyperCubeFactory::getEdge()
{
   static std::unique_ptr<IReferenceShapeHyperCube> EdgeShape;
   if (!EdgeShape)
   {

   }
   return *EdgeShape;
}