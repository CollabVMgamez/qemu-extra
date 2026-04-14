
#ifndef HW_I386_ACPI_BUILD_H
#define HW_I386_ACPI_BUILD_H
#include "hw/acpi/acpi-defs.h"
#include "hw/acpi/aml-build.h"

extern const struct AcpiGenericAddress x86_nvdimm_acpi_dsmio;

void acpi_setup(void);
Object *acpi_get_i386_pci_host(void);

GArray *acpi_build_madt_standalone(MachineState *machine);
void acpi_build(AcpiBuildTables *tables, MachineState *machine);

#endif
