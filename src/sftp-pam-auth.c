#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <security/pam_appl.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>

/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	printf("Acct mgmt\n");
	return PAM_SUCCESS;
}

/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
	int retval;
   int fd[2];

   // NB - PAM specifically proscribes free()'ing memory returned from pam_* util funcs,
   // so these pointers are intentionally not cleaned up.
   const char* pUsername;
   const char* pAuthTok;

	retval = pam_get_user(pamh, &pUsername, "Username: ");
   if (retval != PAM_SUCCESS)
      return retval;
   
   retval = pam_get_authtok(pamh, PAM_AUTHTOK, &pAuthTok, "Halt! What's the password?: ");
   if (retval != PAM_SUCCESS)
      return retval;


   // Fork and execute our PAM auth extension, in this case, a simple shell script.
   pipe(fd);
   pid_t cpid = fork();
   if (cpid == -1)
   {
      printf("Encountered error processing PAM authen (err 1)");
      return PAM_AUTH_ERR; 
   }

   if (cpid == 0)
   {
      // Child execution path

      // Attach stdout of process we exec to write end of our pipe, hence
      // redirecting PAM auth extension output back to parent.
      close(1);
      dup(fd[1]);

      // TO DO - parameterize the location & name of the PAM auth extension.
      int rc = execlp("/usr/local/sbin/do_pam_auth.sh", "do_pam_auth.sh", pUsername, pAuthTok, NULL); 
   }
   else
   {
      // Parent execution path
      
      char buff[1024];
      memset(buff, 0, sizeof(buff));

      int cnt = read(fd[0], buff, sizeof(buff));
		close(fd[0]);
      
      if (strcmp(buff, "accepted") != 0) {
         return PAM_AUTH_ERR;
      }
   } 
	printf("Welcome %s\n", pUsername);

	return PAM_SUCCESS;
}
