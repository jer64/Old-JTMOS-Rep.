#ifndef __INCLUDED_PCI_H__
#define __INCLUDED_PCI_H__

// globals
typedef struct
{
 //
 int type;
}PCIBRIDGE;

//
extern PCIBRIDGE pci;
void pci_init(void);
int pci_device_call(DEVICE_CALL_PARS);
int pci_register_device(void);
void pci_probe(void);

#endif
