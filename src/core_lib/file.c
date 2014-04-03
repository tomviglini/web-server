#include "header/file.h"

void path_free(struct PATH *path) {

	if(path->path_type == 2) {

		//printf("STATIC\n");
		free(((struct STATIC *)path->path)->ptr);

	} else if(path->path_type == 1) {

		//printf("DYNAMIC\n");
		dlclose(((struct DYNAMIC *)path->path)->handle);

	} else if(path->path_type == 3) {

		//printf("ERROR\n");
		//free(((struct ERROR *)path->path)->ptr);
	}

}
