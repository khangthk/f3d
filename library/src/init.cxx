#include "init.h"

#include "log.h"

#include "F3DReaderInstantiator.h"
#include "vtkF3DObjectFactory.h"

#include <vtkLogger.h>
#include <vtkNew.h>
#include <vtkVersion.h>

namespace f3d::detail
{
class init::internals
{
public:
  F3DReaderInstantiator ReaderInstantiator;
};

//----------------------------------------------------------------------------
init::init()
  : Internals(new init::internals())
{
#if NDEBUG
  vtkObject::GlobalWarningDisplayOff();
#endif

  // Disable vtkLogger in case VTK was built with log support
  vtkLogger::SetStderrVerbosity(vtkLogger::VERBOSITY_OFF);
// SetInternalVerbosityLevel needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/7078
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 0, 20200701)
  vtkLogger::SetInternalVerbosityLevel(vtkLogger::VERBOSITY_OFF);
#endif

  // instanciate our own polydata mapper and output windows
  vtkNew<vtkF3DObjectFactory> factory;
  vtkObjectFactory::RegisterFactory(factory);
  vtkObjectFactory::SetAllEnableFlags(0, "vtkPolyDataMapper", "vtkOpenGLPolyDataMapper");

  // Make sure to initialize the output window
  // after the object factory and before the first usage.
  log::setVerboseLevel(log::VerboseLevel::INFO);
}

//----------------------------------------------------------------------------
init::~init() = default;

}
