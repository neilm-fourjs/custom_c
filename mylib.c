
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "f2c/fglExt.h"

int c_lock(int);
int c_unlock(int);

UsrFunction usrFunctions[]={
  { "lock",         c_lock,         1, 2 },
  { "unlock",       c_unlock,       1, 2 },
  { NULL,           NULL,           0, 0 }
};

int c_lock(int n)
{
	int sz;
	char *lfname;
	char *lerr;
	int fh;
	int ret;

	lerr = malloc(80);
	if ( n < 1 ) {
		pushshort(0);
		strcpy(lerr, "invalid num of args");
		pushvchar(lerr, strlen(lerr));
		return 2;
	}

	sz = fglcapi_peekStackBufferSize();
	lfname = malloc(sz);
	popstring(lfname, sz);
// ...
	printf("Arg 1: '%s'\n", lfname);
  fh = open(lfname,O_RDWR | O_CREAT ,S_IRWXU | S_IRWXG);
  if ( fh == -1 ) {
		switch( errno ) {
			case EEXIST: strcpy(lerr,"File exists");
      case EISDIR: strcpy(lerr,"Directory, not file");
      case EACCES: strcpy(lerr,"Access denied");
      case ENAMETOOLONG: strcpy(lerr,"Name too long");
      case ENOENT: strcpy(lerr,"No entry and not create");
      case ENOTDIR: strcpy(lerr,"Bad path name");
      case ENXIO: strcpy(lerr,"Not a readable device");
      case ENODEV: strcpy(lerr,"Bad device");
		}
		ret = 0;
	} else {
  	if ( lockf(fh, F_TLOCK, 0 ) != 0 ) {
    	switch ( errno ) {
    		case EBADF: strcpy(lerr,"Bad file descriptor passed to lockf");
    		case EINVAL: strcpy(lerr,"Oops I specified an invalid operation");
				default: strcpy(lerr,"Lock denied");
			}
    	close(fh);  /* I've got no need for it */
			ret = 0;
		} else {
			sprintf(lerr, "Lock file '%s' created ( %d )", lfname, fh);
			ret = 1;
		}
	}

	free(lfname);

// return true
	pushshort(ret);
// return msg
	pushvchar(lerr, strlen(lerr));
	return 2;
}

int c_unlock(int n)
{
	int sz;
	char *lfname;
	char *lerr;
	int fh;
	int ret;

	lerr = malloc(80);
	if ( n < 1 ) {
		pushshort(0);
		strcpy(lerr, "invalid num of args");
		pushvchar(lerr, strlen(lerr));
		return 2;
	}

	sz = fglcapi_peekStackBufferSize();
	lfname = malloc(sz);
	popstring(lfname, sz);
// ...
	printf("Arg 1: '%s'\n", lfname);
  if ( unlink(lfname) == -1 ) {
		switch (errno) {
			case EACCES: strcpy(lerr,"EACCES: No rights to delete");
			case EPERM: strcpy(lerr,"EPERM: No permission to delete");
			case ENOENT: sprintf(lerr,"ENOENT: Bad path (%s)",lfname);
			case ENOTDIR: sprintf(lerr,"ENOTDIR Bad path (%s)",lfname);
    }
		ret = 0;
	} else {
		sprintf(lerr, "Lock file %s removed", lfname);
		ret = 1;
	}

	free(lfname);

// return true
	pushshort(ret);
// return msg
	pushvchar(lerr, strlen(lerr));
	return 2;
}
