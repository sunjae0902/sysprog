#include "20212277.h"

int32_t main(int32_t argc, char *argv[])
{
	int32_t fd;

	if(argc!=2) {
		printf("Usage: editro <file>\n");
		return 0;
	}

	fd = open(argv[1], O_RDWR|O_SYNC); // 입력 파일에 파일 디스크립터 할당, 읽기 쓰기 모두 가능 , 동기화 
	if(fd<0) {
		printf("Error %d Unable to open %s\n", fd, argv[1]);
		return 0;
	}

	Elf64_Ehdr ehdr;
	Elf64_Shdr* sh_tbl;
	
	read_elf_header(fd, &ehdr); // 헤더 읽음 
	
    if(!is_elf(ehdr)) {
		return 0;
	}
	
	print_elf_header(ehdr); // 헤더 출력

	sh_tbl = malloc(ehdr.e_shentsize * ehdr.e_shnum); // 섹션 헤더 엔트리 사이즈 * 수 만큼의 메모리 할당 
	if(!sh_tbl) {
		printf("Failed to allocate %d bytes\n", (ehdr.e_shentsize * ehdr.e_shnum));
	}
	print_section_headers(fd, ehdr, sh_tbl); // 섹션 헤더 출력 

	return 0;

}
