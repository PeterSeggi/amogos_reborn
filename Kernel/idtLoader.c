/*
#pragma pack(push)
#pragma pack (1)

typedef struct {
	uint16_t offset_l;
	uint16_t selector;
	uint8_t cero;
	uint8_t access;
	uint16_t offset_m;
	uint32_t offset_h;
	uint32_t other_cero;
} DESCR_INT;

#pragma pack(pop)

DESCR_INT * idt = (DESCR_INT *) 0;	//la IDT cargada en 0x00

void setup_IDT_entry(int index, uint64_t offset){
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].selector = 0x08;
	idt[index].cero = 0;
	idt[index].access = ACS_INT;
	idt[index].offset_m = (offset >> 16) & 0xFFFF;
	idt[index].ofset_h = (offset >>32) & 0xFFFFFFFF;
	idt[index].other_cero = 0;
}

void load_idt(){
	_cli();
	
	setup_IDT_entry(0x20, (uint64_t) &_irq00Handler);

	//Interrupción de timer tick habilitada
	picMasterMask(0xFE);
	picSlaveMask(0xFF);
	_sti();
}*/