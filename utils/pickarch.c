#include <mach-o/arch.h>
#include <spawn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__attribute__((weak)) int posix_spawnattr_setarchpref_np(posix_spawnattr_t *restrict, size_t, cpu_type_t *, cpu_subtype_t *, size_t *restrict);

int main(int argc, char **argv, char **envp) {
	if (argc < 3 || !++argv) {
		puts("Usage: pickarch arch_name program <args...>");
		exit(EXIT_FAILURE);
	}

	char *arch_name = *argv++;
	posix_spawnattr_t spawnattrs;
	posix_spawnattr_init(&spawnattrs);
	posix_spawnattr_setflags(&spawnattrs, POSIX_SPAWN_SETEXEC);

	for (const NXArchInfo *architecture = NXGetAllArchInfos(); architecture->name; ++architecture) {
		if (!strcmp(arch_name, architecture->name)) {
			// Exists on macOS 10.16, even though it's annotated that it doesn't.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunguarded-availability-new"
			if (posix_spawnattr_setarchpref_np) {
				posix_spawnattr_setarchpref_np(&spawnattrs, 1, (cpu_type_t[]){architecture->cputype}, (cpu_subtype_t[]){architecture->cpusubtype}, NULL);
#pragma clang diagnostic pop
			} else {
				posix_spawnattr_setbinpref_np(&spawnattrs, 1, (cpu_type_t[]){architecture->cputype}, NULL);
			}
			break;
		}
	}
	posix_spawn(NULL, *argv, NULL, &spawnattrs, argv, envp);
	posix_spawnattr_destroy(spawnattrs);
}
