#ifndef idtLoader
#define idtLoader

void load_idt();
static void setup_IDT_entry (int index, uint64_t offset);


#endif idtLoader