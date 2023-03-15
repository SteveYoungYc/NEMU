#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;
  Elf_Phdr program_header_table[4];
  // Elf_Phdr phdr;
  ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
  printf("ELF Header:\n");
  printf("  Magic:   ");
  for (int i = 0; i < EI_NIDENT; i++) {
      printf("%x ", elf_header.e_ident[i]);
  }
  printf("\n");
  printf("  Class:                             %d\n", elf_header.e_ident[EI_CLASS]);
  printf("  Data:                              %d\n", elf_header.e_ident[EI_DATA]);
  printf("  Version:                           %d\n", elf_header.e_ident[EI_VERSION]);
  printf("  OS/ABI:                            %d\n", elf_header.e_ident[EI_OSABI]);
  printf("  ABI Version:                       %d\n", elf_header.e_ident[EI_ABIVERSION]);
  printf("  Type:                              %d\n", elf_header.e_type);
  printf("  Machine:                           %d\n", elf_header.e_machine);
  printf("  Version:                           %d\n", elf_header.e_version);
  printf("  Entry point address:               0x%x\n", elf_header.e_entry);
  printf("  Start of program headers:          %d (bytes into file)\n", elf_header.e_phoff);
  printf("  Start of section headers:          %d (bytes into file)\n", elf_header.e_shoff);
  printf("  Flags:                             0x%x\n", elf_header.e_flags);
  printf("  Size of this header:               %d (bytes)\n", elf_header.e_ehsize);
  printf("  Size of program headers:           %d (bytes)\n", elf_header.e_phentsize);
  printf("  Number of program headers:         %d\n", elf_header.e_phnum);
  printf("  Size of section headers:           %d (bytes)\n", elf_header.e_shentsize);
  printf("  Number of section headers:         %d\n", elf_header.e_shnum);
  printf("  Section header string table index: %d\n", elf_header.e_shstrndx);

  assert(*(uint32_t *)elf_header.e_ident == 0x464c457f);
  assert(elf_header.e_phnum == 4);

  ramdisk_read(program_header_table, elf_header.e_phoff, elf_header.e_phnum * sizeof(Elf_Phdr));
  printf("Program Header Table:\n");
  for (int i = 0; i < elf_header.e_phnum; i++) {
    printf("  [%d] Type:    %x\n", i, program_header_table[i].p_type);
    printf("       Offset:  %d\n", program_header_table[i].p_offset);
    printf("       Vaddr:   0x%x\n", program_header_table[i].p_vaddr);
    printf("       Paddr:   0x%x\n", program_header_table[i].p_paddr);
    printf("       FileSz:  %d\n", program_header_table[i].p_filesz);
    printf("       MemSz:   %d\n", program_header_table[i].p_memsz);
    printf("       Flags:   %d\n", program_header_table[i].p_flags);
    printf("       Align:   %d\n", program_header_table[i].p_align);
  }

  for (int i = 0; i < elf_header.e_phnum; i++) {
    Elf_Phdr program_header = program_header_table[i];
    if (program_header.p_type == PT_LOAD) {
      memset((void *)program_header.p_vaddr, 0, program_header.p_memsz);
      ramdisk_read((void *)program_header.p_vaddr, program_header.p_offset, program_header.p_filesz);
    }
  }
  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

