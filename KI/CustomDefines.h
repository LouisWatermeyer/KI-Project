#ifndef CUSTOMDEFINES_H
#define CUSTOMDEFINES_H

// This define allows us to satisfy compiler warnings about unused parameters. This should only be used, if the 
// parameter must be there (e.g some std stuff forces it) and re have no use for the parameter.
#define UNUSED(expression) do { (void)(expression); } while (0)

#endif