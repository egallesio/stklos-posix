/*
 * stklos-posix.c			-- Interface for POSIX
 * 
 * Copyright © 2005-2007 Erick Gallesio - I3S-CNRS/ESSI <eg@essi.fr>
 * 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, 
 * USA.
 * 
 *           Author: Erick Gallesio [eg@essi.fr]
 *    Creation date: 15-Dec-2005 11:52 (eg)
 * Last file update: 30-Jun-2007 23:52 (eg)
 */
#include <stklos.h>
#include <sys/stat.h>

static void strlower(char *s)
{
  while (*s = tolower(*s)) s++;
}



static void defvar(char *name, long value, SCM module)
{
  char buff[100];
  
  snprintf(buff, 100, "posix/%s", name);
  strlower(buff+6);

  STk_define_variable(STk_intern(buff),
		      MAKE_INT(value), 
		      module);
}

static error_bad_path_name(SCM path)
{
  STk_error("bad path name ~S", path);
}

static error_bad_parameter(SCM obj)
{
  STk_error("bad parameter ~S", obj);
}

static error_user_informations(SCM who)
{
  STk_error("cannot find user informations for ~S", who);
}


/* ----------------------------------------------------------------------
 * 	Directories functions ...
 * ----------------------------------------------------------------------  */
DEFINE_PRIMITIVE("posix-make-directory", posix_make_directory, subr1, (SCM path))
{
  mode_t mask;
  int n;

  if (!STRINGP(path)) error_bad_path_name(path);
  umask(mask = umask(0));
  n = mkdir(STRING_CHARS(path), 0777);
  return MAKE_BOOLEAN( n == 0 );
}


DEFINE_PRIMITIVE("posix-delete-directory", posix_delete_directory, subr1, (SCM path))
{
  int n;

  if (!STRINGP(path)) error_bad_path_name(path);
  n = rmdir(STRING_CHARS(path));
  return MAKE_BOOLEAN( n == 0 );
}

DEFINE_PRIMITIVE("posix-change-directory", change_dir, subr1, (SCM path))
{
  int n;

  if (!STRINGP(path)) error_bad_path_name(path);
  n = chdir(STRING_CHARS(path));
  return MAKE_BOOLEAN( n == 0 );
}

DEFINE_PRIMITIVE("posix-current-directory", get_dir, subr0, (void))
{
  int n;
  char buf[MAX_PATH_LENGTH], *s;

  s = getcwd(buf, MAX_PATH_LENGTH);
  return (s)? STk_Cstring2string(buf) : STk_false;
}





/* ----------------------------------------------------------------------
 * 	Links ...
 * ----------------------------------------------------------------------  */
DEFINE_PRIMITIVE("posix-make-link", make_link, subr2, (SCM old, SCM new))
{
  int n;

  if (!STRINGP(old)) error_bad_path_name(old);
  if (!STRINGP(new)) error_bad_path_name(new);
  
  n = link(STRING_CHARS(old), STRING_CHARS(new));
 return MAKE_BOOLEAN( n == 0 );
}

DEFINE_PRIMITIVE("posix-make-symbolic-link", make_slink, subr2, (SCM old, SCM new))
{
  int n;

  if (!STRINGP(old)) error_bad_path_name(old);
  if (!STRINGP(new)) error_bad_path_name(new);
  
  n = symlink(STRING_CHARS(old), STRING_CHARS(new));
  return MAKE_BOOLEAN( n == 0 );
}

DEFINE_PRIMITIVE("posix-file-is-symbolic-link?", symlinkp, subr1, (SCM path))
{
  int n;
  struct stat info;
  SCM z;

  if (!STRINGP(path)) error_bad_path_name(path);
  n = stat(STRING_CHARS(path), &info);
  if (n) return STk_false;
  return MAKE_BOOLEAN(S_ISLNK(info.st_mode));
}

DEFINE_PRIMITIVE("posix-read-symbolic-link", readslink, subr1, (SCM path))
{
  int n;
  char buff[MAX_PATH_LENGTH];
  
  if (!STRINGP(path)) error_bad_path_name(path);
  n = readlink(STRING_CHARS(path), buff, MAX_PATH_LENGTH);
  
  if (n == -1) 
    return STk_false;
  else {
    SCM z = STk_makestring(n, NULL);
    
    memcpy(STRING_CHARS(z), buff, n);
    return z;
  }
}
  


/* ----------------------------------------------------------------------
 * 	Errno ...
 * ----------------------------------------------------------------------  */
#define DEF_ERRNO(x)     {defvar(#x, x, module);}

static SCM posix_errno_conv(SCM value)
{
  long val = STk_integer_value(value);
  
  if (val == LONG_MIN) error_bad_parameter(value);
  errno = val;
  return value;
}

static SCM posix_errno_reader(void)
{
  return STk_long2integer((long) errno);
}

static void init_posix_errno(SCM module)
{
#ifdef E2BIG
  DEF_ERRNO(E2BIG);
#endif
#ifdef EACCES
  DEF_ERRNO(EACCES);
#endif
#ifdef EADDRINUSE
  DEF_ERRNO(EADDRINUSE);
#endif
#ifdef EADDRNOTAVAIL
  DEF_ERRNO(EADDRNOTAVAIL);
#endif
#ifdef EAFNOSUPPORT
  DEF_ERRNO(EAFNOSUPPORT);
#endif
#ifdef EAGAIN
  DEF_ERRNO(EAGAIN);
#endif
#ifdef EALREADY
  DEF_ERRNO(EALREADY);
#endif
#ifdef EBADF
  DEF_ERRNO(EBADF);
#endif
#ifdef EBADMSG
  DEF_ERRNO(EBADMSG);
#endif
#ifdef EBUSY
  DEF_ERRNO(EBUSY);
#endif
#ifdef ECANCELED
  DEF_ERRNO(ECANCELED);
#endif
#ifdef ECHILD
  DEF_ERRNO(ECHILD);
#endif
#ifdef ECONNABORTED
  DEF_ERRNO(ECONNABORTED);
#endif
#ifdef ECONNREFUSED
  DEF_ERRNO(ECONNREFUSED);
#endif
#ifdef ECONNRESET
  DEF_ERRNO(ECONNRESET);
#endif
#ifdef EDEADLK
  DEF_ERRNO(EDEADLK);
#endif
#ifdef EDESTADDRREQ
  DEF_ERRNO(EDESTADDRREQ);
#endif
#ifdef EDOM
  DEF_ERRNO(EDOM);
#endif
#ifdef EDQUOT
  DEF_ERRNO(EDQUOT);
#endif
#ifdef EEXIST
  DEF_ERRNO(EEXIST);
#endif
#ifdef EFAULT
  DEF_ERRNO(EFAULT);
#endif
#ifdef EFBIG
  DEF_ERRNO(EFBIG);
#endif
#ifdef EHOSTUNREACH
  DEF_ERRNO(EHOSTUNREACH);
#endif
#ifdef EIDRM
  DEF_ERRNO(EIDRM);
#endif
#ifdef EILSEQ
  DEF_ERRNO(EILSEQ);
#endif
#ifdef EINPROGRESS
  DEF_ERRNO(EINPROGRESS);
#endif
#ifdef EINTR
  DEF_ERRNO(EINTR);
#endif
#ifdef EINVAL
  DEF_ERRNO(EINVAL);
#endif
#ifdef EIO
  DEF_ERRNO(EIO);
#endif
#ifdef EISCONN
  DEF_ERRNO(EISCONN);
#endif
#ifdef EISDIR
  DEF_ERRNO(EISDIR);
#endif
#ifdef ELOOP
  DEF_ERRNO(ELOOP);
#endif
#ifdef EMFILE
  DEF_ERRNO(EMFILE);
#endif
#ifdef EMLINK
  DEF_ERRNO(EMLINK);
#endif
#ifdef EMSGSIZE
  DEF_ERRNO(EMSGSIZE);
#endif
#ifdef EMULTIHOP
  DEF_ERRNO(EMULTIHOP);
#endif
#ifdef ENAMETOOLONG
  DEF_ERRNO(ENAMETOOLONG);
#endif
#ifdef ENETDOWN
  DEF_ERRNO(ENETDOWN);
#endif
#ifdef ENETRESET
  DEF_ERRNO(ENETRESET);
#endif
#ifdef ENETUNREACH
  DEF_ERRNO(ENETUNREACH);
#endif
#ifdef ENFILE
  DEF_ERRNO(ENFILE);
#endif
#ifdef ENOBUFS
  DEF_ERRNO(ENOBUFS);
#endif
#ifdef ENODATA
  DEF_ERRNO(ENODATA);
#endif
#ifdef ENODEV
  DEF_ERRNO(ENODEV);
#endif
#ifdef ENOENT
  DEF_ERRNO(ENOENT);
#endif
#ifdef ENOEXEC
  DEF_ERRNO(ENOEXEC);
#endif
#ifdef ENOLCK
  DEF_ERRNO(ENOLCK);
#endif
#ifdef ENOLINK
  DEF_ERRNO(ENOLINK);
#endif
#ifdef ENOMEM
  DEF_ERRNO(ENOMEM);
#endif
#ifdef ENOMSG
  DEF_ERRNO(ENOMSG);
#endif
#ifdef ENOPROTOOPT
  DEF_ERRNO(ENOPROTOOPT);
#endif
#ifdef ENOSPC
  DEF_ERRNO(ENOSPC);
#endif
#ifdef ENOSR
  DEF_ERRNO(ENOSR);
#endif
#ifdef ENOSTR
  DEF_ERRNO(ENOSTR);
#endif
#ifdef ENOSYS
  DEF_ERRNO(ENOSYS);
#endif
#ifdef ENOTCONN
  DEF_ERRNO(ENOTCONN);
#endif
#ifdef ENOTDIR
  DEF_ERRNO(ENOTDIR);
#endif
#ifdef ENOTEMPTY
  DEF_ERRNO(ENOTEMPTY);
#endif
#ifdef ENOTSOCK
  DEF_ERRNO(ENOTSOCK);
#endif
#ifdef ENOTSUP
  DEF_ERRNO(ENOTSUP);
#endif
#ifdef ENOTTY
  DEF_ERRNO(ENOTTY);
#endif
#ifdef ENXIO
  DEF_ERRNO(ENXIO);
#endif
#ifdef EOPNOTSUPP
  DEF_ERRNO(EOPNOTSUPP);
#endif
#ifdef EOVERFLOW
  DEF_ERRNO(EOVERFLOW);
#endif
#ifdef EPERM
  DEF_ERRNO(EPERM);
#endif
#ifdef EPIPE
  DEF_ERRNO(EPIPE);
#endif
#ifdef EPROTO
  DEF_ERRNO(EPROTO);
#endif
#ifdef EPROTONOSUPPORT
  DEF_ERRNO(EPROTONOSUPPORT);
#endif
#ifdef EPROTOTYPE
  DEF_ERRNO(EPROTOTYPE);
#endif
#ifdef ERANGE
  DEF_ERRNO(ERANGE);
#endif
#ifdef EROFS
  DEF_ERRNO(EROFS);
#endif
#ifdef ESPIPE
  DEF_ERRNO(ESPIPE);
#endif
#ifdef ESRCH
  DEF_ERRNO(ESRCH);
#endif
#ifdef ESTALE
  DEF_ERRNO(ESTALE);
#endif
#ifdef ETIME
  DEF_ERRNO(ETIME);
#endif
#ifdef ETIMEDOUT
  DEF_ERRNO(ETIMEDOUT);
#endif
#ifdef ETXTBSY
  DEF_ERRNO(ETXTBSY);
#endif
#ifdef EWOULDBLOCK
  DEF_ERRNO(EWOULDBLOCK);
#endif
#ifdef EXDEV
  DEF_ERRNO(EXDEV);
#endif

  STk_make_C_parameter2("posix-errno", posix_errno_reader, posix_errno_conv, module);
  errno = 0;
}


DEFINE_PRIMITIVE("posix-error", posix_error, subr0, (void))
{
  return (errno > sys_nerr) ?
    "Unknown error":
    STk_Cstring2string((unsigned char *) sys_errlist[errno]);
}

/* ----------------------------------------------------------------------
 * 	Informations ...
 * ----------------------------------------------------------------------  */
#include <pwd.h>
#include <grp.h>

DEFINE_PRIMITIVE("%posix-file-informations", file_stat, subr1, (SCM path))
{
  int n;
  struct stat info;
  SCM z;

  if (!STRINGP(path)) error_bad_path_name(path);
  n = lstat(STRING_CHARS(path), &info);
  if (n) return STk_false;
  
  z = STk_makevect(10, NULL);	/* NIL because some integers are bignums */
  VECTOR_DATA(z)[0] = STk_long2integer((long)info.st_dev);
  VECTOR_DATA(z)[1] = STk_long2integer((long)info.st_ino);
  VECTOR_DATA(z)[2] = STk_long2integer((long)info.st_mode);
  VECTOR_DATA(z)[3] = STk_long2integer((long)info.st_nlink);
  VECTOR_DATA(z)[4] = STk_long2integer((long)info.st_uid);
  VECTOR_DATA(z)[5] = STk_long2integer((long)info.st_gid);
  VECTOR_DATA(z)[6] = STk_long2integer((long)info.st_size);
  VECTOR_DATA(z)[7] = STk_long2integer((long)info.st_atime);
  VECTOR_DATA(z)[8] = STk_long2integer((long)info.st_mtime);
  VECTOR_DATA(z)[9] = STk_long2integer((long)info.st_ctime);
  return z;
}
  

DEFINE_PRIMITIVE("posix-file-permissions", file_permissions, subr1, (SCM path))
{
  int n;
  struct stat S;

  if (!STRINGP(path)) error_bad_path_name(path);
  n = stat(STRING_CHARS(path), &S);
  STk_error("cannot find file permisssions of ~S", path);

  return (n) ? STk_false : STk_long2integer( S.st_mode & 0777);
}


DEFINE_PRIMITIVE("posix-umask", umask, subr01, (SCM cmask))
{
  long mask;

  if (cmask) {
    mask = STk_integer_value(cmask);
    if (mask == LONG_MIN) error_bad_parameter(cmask);
    mask = (mode_t) umask((mode_t) mask);
  } else {
    mask = (long) umask(0);
    umask((mode_t) mask);
  }

  return STk_long2integer(mask);
}


DEFINE_PRIMITIVE("posix-uid->string", uid2string, subr1, (SCM uid))
{
  long n = STk_integer_value(uid);
  struct passwd *info;

  if (n == LONG_MIN) error_bad_parameter(uid);
  info = getpwuid((uid_t) n);
  if (!info) error_user_informations(uid);
  return STk_Cstring2string(info->pw_name);
}


DEFINE_PRIMITIVE("posix-string->uid", string2uid, subr1, (SCM who))
{
  struct passwd *info;

  if (!STRINGP(who)) error_bad_parameter(who);
  info = getpwnam(STRING_CHARS(who));
  if (!info) error_user_informations(who);
  return STk_long2integer(info->pw_uid);
}


DEFINE_PRIMITIVE("posix-gid->string", gid2string, subr1, (SCM gid))
{
  long n = STk_integer_value(gid);
  struct group *info;

  if (n == LONG_MIN) error_bad_parameter(gid);
  info = getgrgid((gid_t) n);
  if (!info) error_user_informations(gid);
  return STk_Cstring2string(info->gr_name);
}

DEFINE_PRIMITIVE("posix-string->gid", string2gid, subr1, (SCM who))
{
  struct group *info;

  if (!STRINGP(who)) error_bad_parameter(who);
  info = getgrnam(STRING_CHARS(who));
  if (!info) error_user_informations(who);
  return STk_long2integer(info->gr_gid);
}


DEFINE_PRIMITIVE("posix-user-id", getuid, subr0, (void))
{
  return STk_long2integer(getuid());
}


DEFINE_PRIMITIVE("posix-group-id", getgid, subr0, (void))
{
  return STk_long2integer(getgid());
}


DEFINE_PRIMITIVE("posix-effective-user-id", geteuid, subr0, (void))
{
  return STk_long2integer(geteuid());
}


DEFINE_PRIMITIVE("posix-effective-group-id", getegid, subr0, (void))
{
  return STk_long2integer(getegid());
}


/* ====================================================================== */
#define DEF_MODE(x)     {defvar(#x, S_##x, module);}


static void init_posix_modes(SCM module)
{
  DEF_MODE(IFSOCK); DEF_MODE(IFLNK);  DEF_MODE(IFREG);  DEF_MODE(IFBLK);
  DEF_MODE(IFDIR);  DEF_MODE(IFCHR);  DEF_MODE(IFIFO);  DEF_MODE(ISUID);
  DEF_MODE(ISGID);  DEF_MODE(ISVTX);  DEF_MODE(IRWXU);  DEF_MODE(IRUSR);
  DEF_MODE(IWUSR);  DEF_MODE(IXUSR);  DEF_MODE(IRWXG);  DEF_MODE(IRGRP);
  DEF_MODE(IWGRP);  DEF_MODE(IXGRP);  DEF_MODE(IRWXO);  DEF_MODE(IROTH);
  DEF_MODE(IWOTH);  DEF_MODE(IXOTH);
}


/* ----------------------------------------------------------------------
 * 
 * 	Module posix starts here
 *
 * ---------------------------------------------------------------------- 
 */
#include "stklos-posix-inc.c"

MODULE_ENTRY_START("stklos-posix")
{
  SCM module = STk_create_module(STk_intern("stklos-posix"));

  init_posix_modes(module);
  init_posix_errno(module);

  ADD_PRIMITIVE_IN_MODULE(posix_error, module);

  ADD_PRIMITIVE_IN_MODULE(posix_make_directory, module);
  ADD_PRIMITIVE_IN_MODULE(posix_delete_directory, module);
  ADD_PRIMITIVE_IN_MODULE(change_dir, module);
  ADD_PRIMITIVE_IN_MODULE(get_dir, module);

  ADD_PRIMITIVE_IN_MODULE(make_link, module);
  ADD_PRIMITIVE_IN_MODULE(make_slink, module);
  ADD_PRIMITIVE_IN_MODULE(symlinkp, module);
  ADD_PRIMITIVE_IN_MODULE(readslink, module);
    

  ADD_PRIMITIVE_IN_MODULE(file_permissions, module);
  ADD_PRIMITIVE_IN_MODULE(file_stat, module);
  ADD_PRIMITIVE_IN_MODULE(uid2string, module);
  ADD_PRIMITIVE_IN_MODULE(string2uid, module);
  ADD_PRIMITIVE_IN_MODULE(gid2string, module);
  ADD_PRIMITIVE_IN_MODULE(string2gid, module);
  ADD_PRIMITIVE_IN_MODULE(getuid, module);
  ADD_PRIMITIVE_IN_MODULE(getgid, module);
  ADD_PRIMITIVE_IN_MODULE(geteuid, module);
  ADD_PRIMITIVE_IN_MODULE(getegid, module);

  ADD_PRIMITIVE_IN_MODULE(umask, module);

  /* Execute Scheme code */
  STk_execute_C_bytecode(__module_consts, __module_code);
} MODULE_ENTRY_END 


MODULE_ENTRY_INFO() {
   return STk_read_from_C_string(__module_infos);
}
